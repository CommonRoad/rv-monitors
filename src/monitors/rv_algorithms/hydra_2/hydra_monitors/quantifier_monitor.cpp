#include "monitors/rv_algorithms/hydra_2/hydra_monitors/quantifier_monitor.h"

#include <algorithm>
#include <cmath>

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
    // Monitors cache their verdict per step, so each combination evaluated in a step needs its own monitor. If the ID
    // function of a domain maps different values to the same ID, the repeated IDs are made unique by their occurrence.
    std::string uniqueId{getUniqueID()};
    std::string occurrenceSuffix;
    if (const size_t occurrence{idOccurrences[uniqueId]++}; occurrence > 0)
        occurrenceSuffix = "#" + std::to_string(occurrence);
    uniqueId += occurrenceSuffix;
    if (evalMonitors.find(uniqueId) == evalMonitors.end()) // if there is no monitor for the current
                                                           // combination, then a new monitor will be cloned
    {
        // The clones of a sub-monitor are cached by these names. They contain the variable since a shared sub-monitor
        // can be cloned by quantifiers over different variables, e.g., rel(x, y) in E x. A y. rel(x, y) and
        // E y. A x. rel(x, y).
        std::vector<std::string> domValueNames;
        domValueNames.reserve(currentDomainValues.size());
        for (const auto &val : currentDomainValues)
            domValueNames.push_back(val.getVariable() + "=" + val.getUniqueID());
        if (!domValueNames.empty())
            domValueNames.back() += occurrenceSuffix;
        evalMonitor = subMonitor->clone(domValueNames);
        evalMonitor->addDomainValues(currentDomainValues);
        evalMonitors[uniqueId] = evalMonitor;
        evalMonitor->setStepNumber(stepNumber, monitorId);
    } else {
        evalMonitor = evalMonitors[uniqueId];
        evalMonitor->addDomainValues(currentDomainValues); // required (see testExistentialStepLoopingDynamicDomain)
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
    // Carry over to the previous variable only if the iterator of a variable reached the end of its domain.
    auto domain{variableValues.rbegin()};
    for (size_t idx{variableValues.size()}; idx-- > 0; ++domain) {
        ++iters[idx];
        if (iters[idx] != domain->second->end())
            return;
        if (idx > 0)
            iters[idx] = domain->second->begin();
    }
}

void QuantifierMonitor::resetIterators() {
    iters.clear();
    for (auto &[_, domain] : variableValues)
        iters.push_back(domain->begin());
}

size_t QuantifierMonitor::numCombinations() const {
    size_t numCombinations{1};
    // there is no combination as soon as one domain is empty, e.g., for A x in D, y in D2 with an empty dynamic D
    for (const auto &[_, domain] : variableValues)
        numCombinations *= domain->size();
    return numCombinations;
}

size_t QuantifierMonitor::estimatedNumCombinations() const {
    size_t numCombinations{1};
    for (const auto &[_, domain] : variableValues)
        numCombinations *= domain->estimatedSize();
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

double QuantifierMonitor::computeBinomialProbability(const size_t numEval, const double probability,
                                                     const std::function<bool(size_t)> &accepted) {
    double probabilitySum{0.0};
    double binom{1.0}; // n choose k, computed in floating point to avoid integer overflow
    for (size_t k{0}; k <= numEval; ++k) {
        if (k > 0)
            binom = binom * static_cast<double>(numEval - k + 1) / static_cast<double>(k);
        if (accepted(k))
            probabilitySum += binom * std::pow(probability, static_cast<double>(k)) *
                              std::pow(1 - probability, static_cast<double>(numEval - k));
    }
    return std::clamp(probabilitySum, 0.0, 1.0); // the sum can slightly exceed 1 due to rounding
}

Verdict QuantifierMonitor::concreteStep(const size_t parentId, const size_t stepNumber) {
    if (!updateDynamicDomains(stepNumber))
        return {false};
    const auto numberCombinations{numCombinations()};
    idOccurrences.clear();
    const auto verdict{quantifierStep(parentId, stepNumber, numberCombinations)};
    if (!updateDynamicDomains(stepNumber))
        return {false};
    return verdict;
}

bool QuantifierMonitor::updateDynamicDomains(const size_t stepNumber) {
    const auto tracePtr{trace.lock()};
    if (!tracePtr or stepNumber >= tracePtr->getLength())
        return false;
    for (const auto &[_, dom] : variableValues)
        dom->updateDynamicDomain(tracePtr->getEvent(stepNumber).getTimeStep(), domainValues,
                                 tracePtr->getConstantCollection());
    // Updating a domain replaces its values, which invalidates all iterators. Several variables can share a domain, so
    // the iterators are only set after all domains are updated.
    resetIterators();
    return true;
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

void QuantifierMonitor::setStoreVariableResults(const bool store) {
    subMonitor->setStoreVariableResults(store);
    for (auto &[id, mon] : evalMonitors)
        mon->setStoreVariableResults(store);
}
