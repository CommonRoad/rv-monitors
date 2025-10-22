#include "monitors/rv_algorithms/hydra_2/hydra_monitors/quantifier_monitor.h"

QuantifierMonitor::QuantifierMonitor(Formula formula, std::shared_ptr<Hydra2Monitor> subMonitor, const size_t monitorId,
                                     std::map<std::string, std::shared_ptr<DomainStorage>> &variableValues,
                                     const bool shortCircuiting, const double cost,
                                     const double satisfactionProbability)
    : Hydra2Monitor(std::move(formula), monitorId, cost, satisfactionProbability), subMonitor(std::move(subMonitor)),
      shortCircuiting(shortCircuiting), variableValues(variableValues) {

    for (auto &[varName, domain] : variableValues)
        if (!domain->empty())
            iters.push_back(domain->begin());
}

std::string QuantifierMonitor::getUniqueID() const {
    std::string tmp;
    for (const auto &val : currentDomainValues)
        tmp += val.getUniqueID();
    return tmp;
}

std::shared_ptr<Hydra2Monitor> QuantifierMonitor::nextEvalMonitor(const size_t stepNumber) {
    std::shared_ptr<Hydra2Monitor> evalMonitor;
    currentDomainValues = nextCombination();
    if (evalMonitors.find(getUniqueID()) == evalMonitors.end()) // if there is no monitor for the current
                                                                // combination, then a new monitor will be cloned
    {
        std::vector<std::string> domValueNames;
        domValueNames.reserve(currentDomainValues.size());
        for (const auto &val : currentDomainValues)
            domValueNames.push_back(val.getUniqueID());
        evalMonitor = subMonitor->clone(domValueNames);
        evalMonitor->addDomainValues(currentDomainValues);
        evalMonitors[getUniqueID()] = evalMonitor;
        evalMonitor->setStepNumber(stepNumber, monitorId);
    } else {
        evalMonitor = evalMonitors[getUniqueID()];
        evalMonitor->addDomainValues(currentDomainValues); // required (see testExistentialStepLoopingDynamicDomain)
        evalMonitors[getUniqueID()] = evalMonitor;
    }

    shiftCombination();
    return evalMonitor;
}

std::vector<DomainValue> QuantifierMonitor::nextCombination() {
    // store for each quantified variable value which is used
    std::vector vals{domainValues};
    size_t idx{0};
    for (const auto &[varName, domainStorage] : variableValues) {
        if (domainStorage->isConstDomain())
            vals.emplace_back(varName, *iters[idx], domainStorage->isConstDomain(), domainStorage->getIdFunction());
        else
            vals.emplace_back(varName, *iters[idx], domainStorage->isConstDomain(), domainStorage->getIdFunction(),
                              domainStorage->getValidityCondition());
        ++idx;
    }
    return vals;
}

void QuantifierMonitor::shiftCombination() {
    // prepare next combination of variable values by always propagating from last one, e.g., domains A={a, b, c}, B={1,
    // 2, 3}, we have first <a,1>, <a,2>, <a,3>, <b,1>, <b,2>, <b,3>, <c,1>, ...
    size_t idx{variableValues.size() - 1};
    ++iters[idx];
    if (variableValues.rbegin()->second->end() == iters[idx])
        for (auto iter = variableValues.rbegin(); iter != variableValues.rend() and idx > 0; ++iter, --idx) {
            iters[idx] = iter->second->begin();
            ++iters[idx - 1];
        }
}

void QuantifierMonitor::resetIterators() {
    iters.clear();
    for (auto &[_, domain] : variableValues)
        iters.push_back(domain->begin());
}

size_t QuantifierMonitor::numCombinations() const {
    size_t numCombinations{1};
    bool nonEmpty{false};
    for (const auto &[_, domain] : variableValues) {
        if (!domain->empty() or domain->isConstDomain())
            numCombinations *= domain->size();
        if (!domain->empty())
            nonEmpty = true;
    }
    if (!nonEmpty)
        numCombinations = 0;
    return numCombinations;
}

bool QuantifierMonitor::checkNonEmptiness() const {
    return std::all_of(variableValues.begin(), variableValues.end(),
                       [](const std::pair<std::string, std::shared_ptr<DomainStorage>> &varDomain) {
                           return !varDomain.second->empty();
                       });
}

void QuantifierMonitor::setTrace(const std::shared_ptr<Trace> &tra,
                                 const std::vector<std::shared_ptr<DomainStorage>> &domains) {
    trace = tra;

    for (const auto &[fst, snd] : variableValues)
        for (const auto &dom : domains)
            if (dom->getName() == snd->getName()) {
                variableValues[fst] = dom;
                break;
            }
    iters.clear();
    for (auto &[varName, domain] : variableValues)
        iters.push_back(domain->begin());
    subMonitor->setTrace(tra, domains);
}

void QuantifierMonitor::addDomainValues(const std::vector<DomainValue> &vals) { domainValues = vals; }

void QuantifierMonitor::reset() {
    Hydra2Monitor::reset();
    subMonitor->reset();
    evalMonitors.clear();
    resetIterators();
    violations.clear();
}

double QuantifierMonitor::computeCost(const std::shared_ptr<Hydra2Monitor> &monitor, const size_t numEval) {
    return monitor->getEvalCost() * static_cast<double>(numEval);
}

Verdict QuantifierMonitor::concreteStep(const size_t parentId, const size_t stepNumber) {
    size_t idx{0};
    for (const auto &[_, dom] : variableValues) {
        if (const auto tracePtr = trace.lock(); tracePtr and stepNumber < tracePtr->getLength())
            iters[idx] = dom->updateDynamicDomain(tracePtr->getEvent(stepNumber).getTimeStep(), domainValues,
                                                  tracePtr->getConstantCollection());
        else
            return {false};
        ++idx;
    }
    const auto numberCombinations{numCombinations()};
    const auto verdict{quantifierStep(parentId, stepNumber, numberCombinations)};
    resetIterators();
    for (const auto &[_, dom] : variableValues)
        if (const auto tracePtr = trace.lock(); tracePtr and stepNumber < tracePtr->getLength())
            dom->updateDynamicDomain(tracePtr->getEvent(stepNumber).getTimeStep(), domainValues,
                                     tracePtr->getConstantCollection());
        else
            return {false};
    return verdict;
}

Verdict QuantifierMonitor::initializeStepTrue(const size_t stepNumber) const {
    Verdict verdict{true};
    if (const auto tracePtr = trace.lock(); tracePtr && stepNumber < tracePtr->getLength())
        verdict.timeStamp = tracePtr->getEvent(stepNumber).getTimeStep();
    else
        return Verdict{false};

    if (!checkNonEmptiness()) // if there is an empty domain, then a true verdict will be returned by definition
        return verdict;
    return verdict;
}

Verdict QuantifierMonitor::initializeStepFalse(const size_t stepNumber) const {
    Verdict verdict{false};
    if (const auto tracePtr = trace.lock(); tracePtr && stepNumber < tracePtr->getLength())
        verdict.timeStamp = tracePtr->getEvent(stepNumber).getTimeStep();
    else
        return Verdict{false};

    if (!checkNonEmptiness()) // if there is an empty domain, then a false verdict will be returned by definition
        return verdict;
    return verdict;
}

std::map<size_t, std::vector<std::vector<DomainValue>>> QuantifierMonitor::getViolatingDomainValues() const {
    return {violations};
}
void QuantifierMonitor::setStepNumber(const size_t stepNumber, const size_t parentId) {
    Hydra2Monitor::setStepNumber(stepNumber, parentId);
    subMonitor->setStepNumber(stepNumber, parentId);
    for (const auto &[name, sub] : evalMonitors)
        sub->setStepNumber(stepNumber, parentId);
}

void QuantifierMonitor::to_tree(std::map<size_t, std::tuple<std::string, std::vector<size_t>>> &tree) {
    if (tree.find(monitorId) != tree.end())
        return;
    tree.insert({monitorId, {formula.getExpr()->getSymbol(), {subMonitor->getMonitorID()}}});
    subMonitor->to_tree(tree);
}
