#include "monitors/rv_algorithms/hydra_2/hydra_monitors/previous_monitor.h"

Verdict PreviousMonitor::concreteStep(size_t /*parentId*/, const size_t stepNumber) {
    if (stepNumber >= timeStepsTrace.size())
        return {false};
    const size_t timeStamp{timeStepsTrace.at(stepNumber)};
    if (stepNumber == 0 or !interval.contains(timeStamp - timeStepsTrace.at(stepNumber - 1)))
        return {timeStamp, false};
    return {timeStamp, subMonitor->step(monitorId, stepNumber - 1).value};
}

void PreviousMonitor::setTrace(const std::shared_ptr<Trace> &tra,
                               const std::vector<std::shared_ptr<DomainStorage>> &domains) {
    subMonitor->setTrace(tra, domains);
    timeStepsTrace = {};
    for (const auto &event : tra->getTrace())
        timeStepsTrace.push_back(event.getTimeStep());
    trace = tra;
}

void PreviousMonitor::reset() {
    Hydra2Monitor::reset();
    timeStepsTrace = {};
    subMonitor->reset();
}

std::shared_ptr<Hydra2Monitor> PreviousMonitor::clone(const std::vector<std::string> &domValNames) {
    if (auto tmpCopy{Hydra2Monitor::clone(domValNames)}; tmpCopy != nullptr)
        return tmpCopy;
    const auto clonedMonitor = std::make_shared<PreviousMonitor>(*this);
    clonedMonitor->subMonitor = subMonitor->clone(domValNames);
    return resetClonedMonitor(clonedMonitor, domValNames);
}

std::string PreviousMonitor::to_string() {
    std::string itv{};
    std::string end{};
    if (!interval.isEmpty()) {
        if (!interval.isFinite())
            end = "inf";
        else
            end = std::to_string(interval.getEnd());
        itv = "[" + std::to_string(interval.getStart()) + ", " + end + "]";
    }
    return std::string{"P"} + itv + "(" + subMonitor->to_string() + ")";
}

void PreviousMonitor::addDomainValues(const std::vector<DomainValue> &vals) { subMonitor->addDomainValues(vals); }

void PreviousMonitor::to_tree(std::map<size_t, std::tuple<std::string, std::vector<size_t>>> &tree) {
    if (tree.find(monitorId) != tree.end())
        return;
    tree.insert({monitorId, {formula.getExpr()->getSymbol(), {subMonitor->getMonitorID()}}});
    subMonitor->to_tree(tree);
}
