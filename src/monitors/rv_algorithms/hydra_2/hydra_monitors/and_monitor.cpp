#include "monitors/rv_algorithms/hydra_2/hydra_monitors/and_monitor.h"
#include <iostream>

Verdict AndMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    Verdict verdict{subMonitors.at(0)->step(monitorId, stepNumber)};
    if (shortCircuiting) {
        if (!verdict.value) {
            if (!verdict.timeStamp.has_value()) // happens when we are behind last trace element with submonitor
                return {false};
            else
                return {verdict.timeStamp.value(), false};
        }
        for (size_t i{1}; i < subMonitors.size(); ++i) // verdict of first monitor is evaluated above
            if (!subMonitors.at(i)->step(monitorId, stepNumber).value) {
                if (!verdict.timeStamp.has_value())
                    return {false};
                else
                    return {verdict.timeStamp.value(), false};
            }
    } else {
        for (size_t i{1}; i < subMonitors.size(); ++i)
            verdict = verdict && subMonitors.at(i)->step(monitorId, stepNumber);
    }
    return verdict;
}

void AndMonitor::setTrace(const Trace &tra, const std::vector<std::shared_ptr<DomainStorage>> &domains) {
    for (const auto &mon : subMonitors)
        mon->setTrace(tra, domains);
}

void AndMonitor::reset() {
    Hydra2Monitor::reset();
    for (const auto &mon : subMonitors)
        mon->reset();
}

std::shared_ptr<Hydra2Monitor> AndMonitor::clone() {
    auto clonedMonitor = std::make_shared<AndMonitor>(*this);
    std::vector<std::shared_ptr<Hydra2Monitor>> clonedSubMonitors;
    for (const auto &mon : subMonitors)
        clonedSubMonitors.push_back(mon->clone());
    clonedMonitor->subMonitors = clonedSubMonitors;
    return clonedMonitor;
}

std::string AndMonitor::to_string() {
    std::string str{"("};
    for (const auto &mon : subMonitors)
        str += mon->to_string() + " & ";
    str.erase(str.size() - 3);
    str += ")";
    return str;
}

double AndMonitor::computeCost(const std::vector<std::shared_ptr<Hydra2Monitor>> &monitors) {
    double cost{monitors.at(0)->getEvalCost()};
    for (size_t idx{0}; idx < monitors.size(); ++idx)
        cost += monitors.at(idx)->getEvalCost() * (1 - monitors.at(idx)->getSatisfactionProbability());
    return cost / static_cast<double>(monitors.size()); // normalize value
}

double AndMonitor::computeSatisfactionProbability(const std::vector<std::shared_ptr<Hydra2Monitor>> &monitors) {
    double prob{monitors.at(0)->getSatisfactionProbability()};
    for (size_t idx{1}; idx < monitors.size(); ++idx)
        prob *= monitors.at(idx)->getSatisfactionProbability();
    return prob;
}

void AndMonitor::addDomainValues(const std::vector<DomainValue> &vals) {
    for (const auto &sub : subMonitors)
        sub->addDomainValues(vals);
}
void AndMonitor::setStepNumber(size_t stepNumber, size_t parentId) {
    Hydra2Monitor::setStepNumber(stepNumber, parentId);
    for (const auto &sub : subMonitors)
        sub->setStepNumber(stepNumber, monitorId);
}
