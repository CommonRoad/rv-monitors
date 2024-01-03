#include "monitors/rv_algorithms/hydra_2/hydra_monitors/quantifier_monitor.h"

QuantifierMonitor::QuantifierMonitor(Formula formula, std::shared_ptr<Hydra2Monitor> subMonitor, size_t monitorId,
                                     std::map<std::string, std::shared_ptr<DomainStorage>> &variableValues,
                                     bool connectedEval, bool shortCircuiting, double cost,
                                     double satisfactionProbability)
    : Hydra2Monitor(std::move(formula), monitorId, connectedEval, cost, satisfactionProbability),
      subMonitor(std::move(subMonitor)), shortCircuiting(shortCircuiting), variableValues(variableValues) {

    for (auto &[varName, domain] : variableValues)
        iters.push_back(domain->begin());
}

std::shared_ptr<Hydra2Monitor> QuantifierMonitor::nextEvalMonitor(size_t parentId, size_t count) {
    std::shared_ptr<Hydra2Monitor> evalMonitor;
    if (traceIdx[parentId] == 0 or count > evalMonitors.size() - 1) { // if there is no monitor for the current
                                                                      // combination, then a new monitor will be cloned
        evalMonitor = subMonitor->clone();
        evalMonitors.push_back(evalMonitor);
    } else
        evalMonitor = evalMonitors[count];

    // todo check if domain values have same "name" for A x. A x. otherwise there could be confusion in sub-monitors
    evalMonitor->addDomainValues(nextCombination());
    shiftCombination();
    return evalMonitor;
}

std::vector<DomainValue> QuantifierMonitor::nextCombination() {
    // store for each quantified variable value which is used
    std::vector<DomainValue> vals{domainValues};
    size_t idx{0};
    for (const auto &[varName, _] : variableValues) {
        vals.emplace_back(varName, *iters[idx]);
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
    for (const auto &[_, domain] : variableValues)
        numCombinations *= domain->size();
    return numCombinations;
}

bool QuantifierMonitor::checkNonEmptiness() const {
    return std::all_of(variableValues.begin(), variableValues.end(),
                       [](const std::pair<std::string, std::shared_ptr<DomainStorage>> &varDomain) {
                           return !varDomain.second->empty();
                       });
}

void QuantifierMonitor::setTrace(const Trace &tra) {
    trace = tra;
    subMonitor->setTrace(tra);
}

void QuantifierMonitor::addDomainValues(const std::vector<DomainValue> &vals) { domainValues = vals; }

void QuantifierMonitor::reset() {
    Hydra2Monitor::reset();
    subMonitor->reset();
    evalMonitors.clear();
    resetIterators();
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
    evalMonitors.reserve(numberCombinations);
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
