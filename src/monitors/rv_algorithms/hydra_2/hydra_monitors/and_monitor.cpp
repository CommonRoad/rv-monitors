#include "monitors/rv_algorithms/hydra_2/hydra_monitors/and_monitor.h"

Verdict AndMonitor::concreteStep(size_t parentId, const size_t stepNumber) {
    Verdict verdict{subMonitors.at(0)->step(monitorId, stepNumber)};
    if (shortCircuiting) {
        if (!verdict.value) {
            if (!verdict.timeStamp.has_value()) // happens when we are behind last trace element with submonitor
                return {false};
            return {verdict.timeStamp.value(), false};
        }
        for (size_t i{1}; i < subMonitors.size(); ++i) // verdict of first monitor is evaluated above
            if (!subMonitors.at(i)->step(monitorId, stepNumber).value) {
                if (!verdict.timeStamp.has_value())
                    return {false};
                return {verdict.timeStamp.value(), false};
            }
    } else {
        for (size_t i{1}; i < subMonitors.size(); ++i)
            verdict = verdict && subMonitors.at(i)->step(monitorId, stepNumber);
    }
    return verdict;
}

void AndMonitor::setTrace(const std::shared_ptr<Trace> &tra,
                          const std::vector<std::shared_ptr<DomainStorage>> &domains) {
    for (const auto &mon : subMonitors)
        mon->setTrace(tra, domains);
}

void AndMonitor::reset() {
    Hydra2Monitor::reset();
    for (const auto &mon : subMonitors)
        mon->reset();
}

std::shared_ptr<Hydra2Monitor> AndMonitor::clone(const std::vector<std::string> &domValNames) {
    if (auto tmpCopy{Hydra2Monitor::clone(domValNames)}; tmpCopy != nullptr)
        return tmpCopy;

    const auto clonedMonitor = std::make_shared<AndMonitor>(*this);
    std::vector<std::shared_ptr<Hydra2Monitor>> clonedSubMonitors;
    clonedSubMonitors.reserve(subMonitors.size());
    for (const auto &mon : subMonitors)
        clonedSubMonitors.push_back(mon->clone(domValNames));
    clonedMonitor->subMonitors = clonedSubMonitors;

    return resetClonedMonitor(clonedMonitor, domValNames);
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
    // accumulate the costs of each monitor in the vector
    double cost{0.0};
    for (const auto &mon : monitors)
        cost += mon->getEvalCost();
    return cost;
}

double AndMonitor::computeSatisfactionProbability(const std::vector<std::shared_ptr<Hydra2Monitor>> &monitors) {
    double prob{monitors.at(0)->getSatisfactionProbability()};
    for (size_t idx{1}; idx < monitors.size(); ++idx)
        prob *= monitors.at(idx)->getSatisfactionProbability();
    return prob;
}

void AndMonitor::addDomainValues(const std::vector<DomainValue> &vals) {
    for (size_t idx{0}; idx < subMonitors.size(); ++idx)
        subMonitors.at(idx)->addDomainValues(vals);
}

void AndMonitor::setStepNumber(const size_t stepNumber, const size_t parentId) {
    Hydra2Monitor::setStepNumber(stepNumber, parentId);
    if (!stepNumberValid(stepNumber, parentId))
        return;
    for (const auto &sub : subMonitors)
        sub->setStepNumber(stepNumber, monitorId);
}

void AndMonitor::to_tree(std::map<size_t, std::tuple<std::string, std::vector<size_t>>> &tree) {
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
