#include "monitors/rv_algorithms/hydra_2/hydra_monitors/next_monitor.h"
#include <iostream>

Verdict NextMonitor::concreteStep(size_t parentId, size_t stepNumber) {

    if (!currentTime) // first event in trace (next time step specifies verdict
        currentTime = subMonitor->step(monitorId, stepNumber).timeStamp;

    if (!std::all_of(
            domainValueArguments.begin(), domainValueArguments.end(),
            [step = stepNumber + 1](const DomainValue &dv) { return dv.isConst() or dv.checkValidity(step); })) {
        return {*currentTime, false};
    }

    auto subVerdict{subMonitor->step(monitorId, stepNumber + 1)};
    if (subVerdict.timeStamp.has_value() and currentTime) {
        size_t lastTime = *currentTime;
        currentTime = subVerdict.timeStamp;
        if (!interval.isFinite() and interval.getStart() <= lastTime)
            return {lastTime, subVerdict.value};
        else if (interval.isFinite() and
                 Utils::isInInterval(subVerdict.timeStamp.value() - lastTime, interval.getStart(), interval.getEnd()))
            return {lastTime, subVerdict.value};
        else
            return {lastTime, false};
    } else // last event in trace
        return {*currentTime, false};
}

void NextMonitor::setTrace(const std::shared_ptr<Trace> &tra,
                           const std::vector<std::shared_ptr<DomainStorage>> &domains) {
    subMonitor->setTrace(tra, domains);
}

void NextMonitor::reset() {
    Hydra2Monitor::reset();
    currentTime = {};
    subMonitor->reset();
}

std::shared_ptr<Hydra2Monitor> NextMonitor::clone(const std::vector<std::string> &domValNames) {
    auto tmpCopy{Hydra2Monitor::clone(domValNames)};
    if (tmpCopy != nullptr)
        return tmpCopy;
    auto clonedMonitor = std::make_shared<NextMonitor>(*this);
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

void NextMonitor::setStepNumber(size_t stepNumber, size_t parentId) {
    Hydra2Monitor::setStepNumber(stepNumber, parentId);
    if (!Hydra2Monitor::stepNumberValid(stepNumber, parentId))
        return;
    subMonitor->setStepNumber(stepNumber, monitorId);
}

void NextMonitor::to_tree(std::map<size_t, std::tuple<std::string, std::vector<size_t>>> &tree) {
    if (tree.find(monitorId) != tree.end())
        return;
    tree.insert({monitorId, {formula.getExpr()->getSymbol(), {subMonitor->getMonitorID()}}});
    subMonitor->to_tree(tree);
}
