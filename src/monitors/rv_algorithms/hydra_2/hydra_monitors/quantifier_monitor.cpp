#include "quantifier_monitor.h"

QuantifierMonitor::QuantifierMonitor(Formula formula, std::shared_ptr<HydraMonitor> subMonitor, size_t monitorId,
                                     std::map<std::string, std::string> variables, DomainCollection &domains,
                                     bool connectedEval, bool shortCircuiting, double cost, double satisfactionProbability)
    : HydraMonitor(std::move(formula), monitorId, connectedEval, cost, satisfactionProbability),
      subMonitor(std::move(subMonitor)), shortCircuiting(shortCircuiting) {

    for (auto &[varName, domainName] : variables)
        varDomains.emplace_back(varName, domains.getDomain(domainName));

    for (auto &varDomain : varDomains)
        iters.push_back(varDomain.second.begin());
}

bool QuantifierMonitor::nextCombination() {
    if (iters[0] == varDomains[0].second.end()) {
        resetIterators();
        return false;
    }

    size_t numVars = varDomains.size();
    for (size_t i = 0; i < numVars; i++) {
        auto varIter{variableValues.find(varDomains[i].first)};
        if (varIter != variableValues.end())
            varIter->second = *iters[i];
        else
            variableValues.insert(std::make_pair(varDomains[i].first, *iters[i]));
    }

    subMonitor->setVariableValues(variableValues);

    ++iters[numVars - 1];
    for (size_t i = numVars - 1; i > 0 and iters[i] == varDomains[i].second.end(); i--) {
        iters[i] = varDomains[i].second.begin();
        ++iters[i - 1];
    }

    return true;
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

void QuantifierMonitor::reset(std::optional<size_t> stepNumber) {
    HydraMonitor::reset(stepNumber);
    subMonitor->reset(stepNumber);
    variableValues.clear();
    resetIterators();
}
