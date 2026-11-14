#include "monitors/rv_algorithms/hydra_2/hydra_monitors/next_monitor.h"

Verdict NextMonitor::concreteStep(size_t /*parentId*/, const size_t stepNumber) {
    if (stepNumber >= timeStepsTrace.size())
        return {false};
    const size_t timeStamp{timeStepsTrace.at(stepNumber)};
    if (stepNumber + 1 == timeStepsTrace.size() or !validAt(domainValueArguments, timeStepsTrace.at(stepNumber + 1)) or
        !interval.contains(timeStepsTrace.at(stepNumber + 1) - timeStamp))
        return {timeStamp, false};
    return {timeStamp, subMonitor->step(monitorId, stepNumber + 1).value};
}

void NextMonitor::setTrace(const std::shared_ptr<Trace> &tra,
                           const std::vector<std::shared_ptr<DomainStorage>> &domains) {
    subMonitor->setTrace(tra, domains);
    timeStepsTrace = {};
    for (const auto &event : tra->getTrace())
        timeStepsTrace.push_back(event.getTimeStep());
    trace = tra;
}

void NextMonitor::reset() {
    Hydra2Monitor::reset();
    timeStepsTrace = {};
    subMonitor->reset();
}

std::shared_ptr<Hydra2Monitor> NextMonitor::clone(const std::vector<std::string> &domValNames) {
    if (auto tmpCopy{Hydra2Monitor::clone(domValNames)}; tmpCopy != nullptr)
        return tmpCopy;
    const auto clonedMonitor = std::make_shared<NextMonitor>(*this);
    clonedMonitor->subMonitor = subMonitor->clone(domValNames);
    return resetClonedMonitor(clonedMonitor, domValNames);
}

std::string NextMonitor::to_string() {
    std::string itv{};
    std::string end{};
    if (!interval.isEmpty()) {
        if (!interval.isFinite())
            end = "inf";
        else
            end = std::to_string(interval.getEnd());
        itv = "[" + std::to_string(interval.getStart()) + ", " + end + "]";
    }
    return std::string{"X"} + itv + "(" + subMonitor->to_string() + ")";
}

void NextMonitor::addDomainValues(const std::vector<DomainValue> &vals) {
    domainValueArguments = vals;
    subMonitor->addDomainValues(vals);
}

void NextMonitor::to_tree(std::map<size_t, std::tuple<std::string, std::vector<size_t>>> &tree) {
    if (tree.find(monitorId) != tree.end())
        return;
    tree.insert({monitorId, {formula.getExpr()->getSymbol(), {subMonitor->getMonitorID()}}});
    subMonitor->to_tree(tree);
}
