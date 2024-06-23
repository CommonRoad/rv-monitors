#include "monitors/rv_algorithms/hydra_2/hydra_monitors/quantifier_monitor.h"

QuantifierMonitor::QuantifierMonitor(Formula formula, std::shared_ptr<Hydra2Monitor> subMonitor, size_t monitorId,
                                     std::map<std::string, std::shared_ptr<DomainStorage>> &variableValues,
                                     bool connectedEval, bool shortCircuiting, double cost,
                                     double satisfactionProbability)
    : Hydra2Monitor(std::move(formula), monitorId, connectedEval, cost, satisfactionProbability),
      subMonitor(std::move(subMonitor)), shortCircuiting(shortCircuiting), variableValues(variableValues) {

    for (auto &[varName, domain] : variableValues)
        if (!domain->empty())
            iters.push_back(domain->begin());
}

std::string QuantifierMonitor::getUniqueID() {
    std::string tmp;
    for (const auto &val : currentDomainValues)
        tmp += val.getUniqueID();
    return tmp;
}

std::shared_ptr<Hydra2Monitor> QuantifierMonitor::nextEvalMonitor(size_t stepNumber) {
    std::shared_ptr<Hydra2Monitor> evalMonitor;
    currentDomainValues = nextCombination();
    if (evalMonitors.find(getUniqueID()) == evalMonitors.end()) // if there is no monitor for the current
                                                                // combination, then a new monitor will be cloned
    {
        evalMonitor = subMonitor->clone();
        evalMonitors[getUniqueID()] = evalMonitor;
        evalMonitor->addDomainValues(currentDomainValues);
        evalMonitor->setStepNumber(stepNumber, monitorId);
    } else {
        evalMonitor = evalMonitors[getUniqueID()];
        evalMonitor->addDomainValues(currentDomainValues);
    }

    shiftCombination();
    return evalMonitor;
}

std::vector<DomainValue> QuantifierMonitor::nextCombination() {
    // store for each quantified variable value which is used
    std::vector<DomainValue> vals{domainValues};
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
        if (!domain->empty())
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

void QuantifierMonitor::setTrace(const Trace &tra, const std::vector<std::shared_ptr<DomainStorage>> &domains) {
    trace = tra;

    for (const auto &var : variableValues)
        for (const auto &dom : domains)
            if (dom->getName() == var.second->getName()) {
                variableValues[var.first] = dom;
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

double QuantifierMonitor::computeCost(const std::shared_ptr<Hydra2Monitor> &monitor, size_t numEval) {
    return monitor->getEvalCost() * static_cast<double>(numEval);
}

double QuantifierMonitor::computeSatisfactionProbability(const std::shared_ptr<Hydra2Monitor> &monitor,
                                                         size_t numEval) {
    double prob{monitor->getSatisfactionProbability()};
    for (size_t num{1}; num < numEval; num++)
        prob *= monitor->getSatisfactionProbability();
    return prob;
}

Verdict QuantifierMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    size_t idx{0};
    for (const auto &[_, dom] : variableValues) {
        iters[idx] = dom->updateDynamicDomain(stepNumber);
        ++idx;
    }
    auto numberCombinations{numCombinations()};

    auto verdict{quantifierStep(parentId, stepNumber, numberCombinations)};
    resetIterators();
    for (const auto &[_, dom] : variableValues)
        dom->updateDynamicDomain(stepNumber);
    return verdict;
}

Verdict QuantifierMonitor::initializeStepTrue(size_t parentId) {
    Verdict verdict{true};
    if (traceIdx[parentId] < trace.getLength())
        verdict.timeStamp = trace.getEvent(traceIdx[parentId]).getTimeStep();
    else
        return Verdict{false};

    if (!checkNonEmptiness()) // if there is an empty domain, then a true verdict will be returned by definition
        return verdict;
    return verdict;
}

Verdict QuantifierMonitor::initializeStepFalse(size_t parentId) {
    Verdict verdict{false};
    if (traceIdx[parentId] < trace.getLength())
        verdict.timeStamp = trace.getEvent(traceIdx[parentId]).getTimeStep();
    else
        return Verdict{false};

    if (!checkNonEmptiness()) // if there is an empty domain, then a false verdict will be returned by definition
        return verdict;
    return verdict;
}

std::map<size_t, std::vector<std::vector<DomainValue>>> QuantifierMonitor::getViolatingDomainValues() const {
    return {violations};
}
void QuantifierMonitor::setStepNumber(size_t stepNumber, size_t parentId) {
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
