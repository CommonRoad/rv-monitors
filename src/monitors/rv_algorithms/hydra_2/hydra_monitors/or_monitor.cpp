#include "monitors/rv_algorithms/hydra_2/hydra_monitors/or_monitor.h"

Verdict OrMonitor::concreteStep(size_t parentId, const size_t stepNumber) {
    Verdict verdict{subMonitors.at(0)->step(monitorId, stepNumber)};
    if (shortCircuiting) {
        if (verdict.value) {
            if (!verdict.timeStamp.has_value()) // happens when we are behind last trace element with submonitor
                return {true};
            return {verdict.timeStamp.value(), true};
        }
        for (size_t i{1}; i < subMonitors.size(); ++i) { // verdict of first monitor is evaluated above
            if (subMonitors.at(i)->step(monitorId, stepNumber).value) {
                if (!verdict.timeStamp.has_value())
                    return {true};
                return {verdict.timeStamp.value(), true};
            }
        }
    } else {                                           // if no short-circuiting, check all verdicts of sub-monitors
        for (size_t i{1}; i < subMonitors.size(); ++i) // verdict of first monitor is evaluated above
            verdict = verdict || subMonitors.at(i)->step(monitorId, stepNumber);
    }
    return verdict;
}

void OrMonitor::setTrace(const std::shared_ptr<Trace> &tra,
                         const std::vector<std::shared_ptr<DomainStorage>> &domains) {
    for (const auto &mon : subMonitors)
        mon->setTrace(tra, domains);
    trace = tra;
}

void OrMonitor::reset() {
    Hydra2Monitor::reset();
    for (const auto &mon : subMonitors)
        mon->reset();
}

std::shared_ptr<Hydra2Monitor> OrMonitor::clone(const std::vector<std::string> &domValNames) {
    if (auto tmpCopy{Hydra2Monitor::clone(domValNames)}; tmpCopy != nullptr)
        return tmpCopy;
    const auto clonedMonitor = std::make_shared<OrMonitor>(*this);
    std::vector<std::shared_ptr<Hydra2Monitor>> clonedSubMonitors;
    clonedSubMonitors.reserve(subMonitors.size());
    for (const auto &mon : subMonitors)
        clonedSubMonitors.push_back(mon->clone(domValNames));
    clonedMonitor->subMonitors = clonedSubMonitors;
    return resetClonedMonitor(clonedMonitor, domValNames);
}

std::string OrMonitor::to_string() {
    std::string str{"("};
    for (const auto &mon : subMonitors)
        str += mon->to_string() + " | ";
    str.erase(str.size() - 3);
    str += ")";
    return str;
}

double OrMonitor::computeEvaluationCost(const std::vector<std::shared_ptr<Hydra2Monitor>> &monitors) {
    // accumulate the costs of each monitor in the vector
    double cost{0.0};
    for (const auto &mon : monitors)
        cost += mon->getEvalCost();
    return cost;
}

double OrMonitor::computeSatisfactionProbability(const std::vector<std::shared_ptr<Hydra2Monitor>> &monitors) {
    double prob{1 - monitors.at(0)->getSatisfactionProbability()};
    for (size_t i{1}; i < monitors.size(); ++i) {
        prob *= 1 - monitors.at(i)->getSatisfactionProbability();
    }
    return 1 - prob;
}

void OrMonitor::addDomainValues(const std::vector<DomainValue> &vals) {
    for (const auto &sub : subMonitors)
        sub->addDomainValues(vals);
}

void OrMonitor::setStepNumber(const size_t stepNumber, const size_t parentId) {
    Hydra2Monitor::setStepNumber(stepNumber, parentId);
    if (!stepNumberValid(stepNumber, parentId))
        return;
    for (const auto &sub : subMonitors)
        sub->setStepNumber(stepNumber, monitorId);
}

void OrMonitor::to_tree(std::map<size_t, std::tuple<std::string, std::vector<size_t>>> &tree) {
    if (tree.find(monitorId) != tree.end())
        return;
    std::vector<size_t> ids;
    ids.reserve(subMonitors.size());
    for (const auto &mon : subMonitors) {
        ids.push_back(mon->getMonitorID());
        mon->to_tree(tree);
    }
    tree.insert({monitorId, {formula.getExpr()->getSymbol(), ids}});
}
