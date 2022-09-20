#include "quantifier_monitor.h"

QuantifierMonitor::QuantifierMonitor(Formula formula, std::shared_ptr<Hydra2Monitor> subMonitor, size_t monitorId,
                                     std::map<std::string, std::string> variables, DomainCollection &domains,
                                     bool connectedEval, bool shortCircuiting, double cost, double satisfactionProbability)
    : Hydra2Monitor(std::move(formula), monitorId, connectedEval, cost, satisfactionProbability),
      subMonitor(std::move(subMonitor)), shortCircuiting(shortCircuiting) {

    for (auto &[varName, domainName] : variables)
        varDomains.emplace_back(varName, domains.getDomain(domainName));

    for (auto &varDomain : varDomains)
        iters.push_back(varDomain.second.begin());

    numberCombinations = numCombinations();
}

std::shared_ptr<Hydra2Monitor> QuantifierMonitor::nextEvalMonitor(size_t parentId, size_t count) {
    std::shared_ptr<Hydra2Monitor> evalMonitor;
    if (traceIdx[parentId] == 0 or count > evalMonitors.size() - 1) { // if there is no monitor for the current combination,
                                                                      // then a new monitor will be cloned
        nextCombination();
        evalMonitor = subMonitor->clone();
        evalMonitors.push_back(evalMonitor);
    } else
        evalMonitor = evalMonitors[count];

    return evalMonitor;
}

void QuantifierMonitor::nextCombination() {
    size_t numVars = varDomains.size();
    for (size_t i = 0; i < numVars; i++) {
        auto varIter{variableValues.find(varDomains[i].first)};
        if (varIter != variableValues.end())
            varIter->second = *iters[i];
        else
            variableValues.insert(std::make_pair(varDomains[i].first, *iters[i]));
    }

    subMonitor->setVariableValues(variableValues); // the values of the new combination are inserted into the child monitor

    ++iters[numVars - 1];
    for (size_t i = numVars - 1; i > 0 and iters[i] == varDomains[i].second.end(); i--) { // iterators are shifted to form a new combination
        iters[i] = varDomains[i].second.begin();
        ++iters[i - 1];
    }
}

void QuantifierMonitor::resetIterators() {
    iters.clear();
    for (auto &varDomain : varDomains)
        iters.push_back(varDomain.second.begin());
}

size_t QuantifierMonitor::numCombinations() const {
    size_t numCombinations{1};
    for (const auto &varDomain : varDomains)
        numCombinations *= varDomain.second.size();
    return numCombinations;
}

bool QuantifierMonitor::checkNonEmptiness() const {
    return std::all_of(varDomains.begin(), varDomains.end(),
                       [](const std::pair<std::string, Domain> &varDomain) { return varDomain.second.size() != 0; });
}

void QuantifierMonitor::setTrace(const Trace &tra) {
    trace = tra;
    subMonitor->setTrace(trace);
}

void QuantifierMonitor::setVariableValues(std::map<std::string, Value> values) {
    variableValues = values;
    subMonitor->setVariableValues(values);
}

void QuantifierMonitor::reset() {
    Hydra2Monitor::reset();
    subMonitor->reset();
    evalMonitors.clear();
    variableValues.clear();
    resetIterators();
}

double QuantifierMonitor::computeCost(const std::shared_ptr<Hydra2Monitor> &monitor, size_t numEval) {
    return monitor->getEvalCost() * static_cast<double>(numEval);
}

double QuantifierMonitor::computeSatisfactionProbability(const std::shared_ptr<Hydra2Monitor> &monitor, size_t numEval) {
    double prob{monitor->getSatisfactionProbability()};
    for (size_t num{1}; num < numEval; num++)
        prob *= monitor->getSatisfactionProbability();
    return prob;
}
