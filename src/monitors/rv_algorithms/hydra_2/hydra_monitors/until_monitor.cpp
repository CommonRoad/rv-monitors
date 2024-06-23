#include "monitors/rv_algorithms/hydra_2/hydra_monitors/until_monitor.h"

void UntilMonitor::stepLeftRightMonitors() {
    subVerdictLeft = subMonitorLeft->step(monitorId, stepNumberUntil, true);
    if (subVerdictLeft->timeStamp)
        headPositionMonitorLeft.update(subVerdictLeft->timeStamp.value());
    else {
        endReached = true;
        ++stepNumberUntil;
    }
    subVerdictRight = subMonitorRight->step(monitorId, stepNumberUntil, true);
    if (subVerdictRight->timeStamp)
        headPositionMonitorRight.update(subVerdictRight->timeStamp.value());
    else
        endReached = true;
    ++stepNumberUntil;
}

Verdict UntilMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    subVerdictBaseTimeStep = subMonitorBaseTimeStep->step(monitorId, stepNumber, true);
    if (subVerdictBaseTimeStep->timeStamp)
        headPositionMonitorBaseTimeStep.update(subVerdictBaseTimeStep->timeStamp.value());
    else
        return {false};

    if (lastSatisfaction &&
        lastSatisfaction->traceIdx <
            headPositionMonitorBaseTimeStep.traceIdx) // if last satisfaction (-> right side of until satisfied) is
                                                      // before current time step, reset last satisfaction
        lastSatisfaction = {};
    if (lastViolation &&
        lastViolation->traceIdx <
            headPositionMonitorBaseTimeStep.traceIdx) // if last violation (-> left side of until violated) is
                                                      // before current time step, reset last satisfaction
        lastViolation = {};

    // if not step was performed yet or last verdicts are behind base time step, execute step for current base step
    if (!endReached and (stepNumberUntil == 0 or
                         (subVerdictLeft->timeStamp.has_value() and
                          subVerdictLeft->timeStamp.value() < headPositionMonitorBaseTimeStep.timeStep) or
                         (subVerdictRight->timeStamp.has_value() and
                          subVerdictRight->timeStamp.value() <
                              headPositionMonitorBaseTimeStep.timeStep))) { // only executed at first call of function
        stepLeftRightMonitors();
    }

    while (!endReached && !lastViolation &&
           (!interval.isFinite() or
            headPositionMonitorLeft.timeStep - headPositionMonitorBaseTimeStep.timeStep <= interval.getEnd() or
            headPositionMonitorLeft.timeStep <
                headPositionMonitorBaseTimeStep.timeStep)) { // loop until end is reached, until left side of monitor is
                                                             // false, or until upper bound of time
                                                             // interval is violated
        if (subVerdictRight->value) {
            lastSatisfaction = HeadPosHydra2(headPositionMonitorRight.timeStep, headPositionMonitorRight.traceIdx);
        }
        if (!subVerdictLeft->value) {
            lastViolation = HeadPosHydra2(headPositionMonitorLeft.timeStep, headPositionMonitorLeft.traceIdx);
        }
        if (!std::all_of(
                domainValueArguments.begin(), domainValueArguments.end(),
                [step = stepNumberUntil](const DomainValue &dv) { return dv.isConst() or dv.checkValidity(step); })) {
            endReached = true; // just for completeness, could be removed
            break;
        }
        stepLeftRightMonitors();
    }
    // check whether right side of until was satisfied, whether satisfaction is after start of allowed time interval,
    // and whether left side of from is greater/equal zero (required since unsigned, and we have an overflow
    if (lastSatisfaction &&
        lastSatisfaction->timeStep - headPositionMonitorBaseTimeStep.timeStep >= interval.getStart() &&
        lastSatisfaction->timeStep >= headPositionMonitorBaseTimeStep.timeStep)
        return {headPositionMonitorBaseTimeStep.timeStep, true};
    else
        return {headPositionMonitorBaseTimeStep.timeStep, false};
}

void UntilMonitor::setTrace(const Trace &tra, const std::vector<std::shared_ptr<DomainStorage>> &domains) {
    subMonitorLeft->setTrace(tra, domains);
    subMonitorRight->setTrace(tra, domains);
    subMonitorBaseTimeStep->setTrace(tra, domains);
}
void UntilMonitor::reset() {
    Hydra2Monitor::reset();
    subMonitorRight->reset();
    subMonitorLeft->reset();
    subMonitorBaseTimeStep->reset();
    subVerdictLeft = {};
    subVerdictRight = {};
    lastSatisfaction = {};
    lastViolation = {};
    headPositionMonitorRight.reset();
    headPositionMonitorLeft.reset();
    headPositionMonitorBaseTimeStep.reset();
    stepNumberUntil = 0;
    endReached = false;
}

std::shared_ptr<Hydra2Monitor> UntilMonitor::clone() {
    auto clonedMonitor = std::make_shared<UntilMonitor>(*this);
    clonedMonitor->subMonitorRight = subMonitorRight->clone();
    clonedMonitor->subMonitorLeft = subMonitorLeft->clone();
    clonedMonitor->subMonitorBaseTimeStep = subMonitorBaseTimeStep->clone();
    return clonedMonitor;
}

std::string UntilMonitor::to_string() {
    std::string itv{" "};
    std::string end{};
    if (!interval.isEmpty()) {
        if (!interval.isFinite())
            end = "inf";
        else
            end = std::to_string(interval.getEnd());
        itv = "[" + std::to_string(interval.getStart()) + ", " + end + "] ";
    }
    return "(" + subMonitorLeft->to_string() + " U" + itv + subMonitorRight->to_string() + ")";
}

void UntilMonitor::addDomainValues(const std::vector<DomainValue> &vals) {
    domainValueArguments = vals;
    subMonitorRight->addDomainValues(vals);
    subMonitorLeft->addDomainValues(vals);
    subMonitorBaseTimeStep->addDomainValues(vals);
}
void UntilMonitor::setStepNumber(size_t stepNumber, size_t parentId) {
    Hydra2Monitor::setStepNumber(stepNumber, parentId);
    subMonitorRight->setStepNumber(stepNumber, monitorId);
    subMonitorLeft->setStepNumber(stepNumber, monitorId);
    stepNumberUntil = stepNumber;
}

void UntilMonitor::to_tree(std::map<size_t, std::tuple<std::string, std::vector<size_t>>> &tree) {
    if (tree.find(monitorId) != tree.end())
        return;
    tree.insert({monitorId,
                 {formula.getExpr()->getSymbol(), {subMonitorLeft->getMonitorID(), subMonitorRight->getMonitorID()}}});
    subMonitorLeft->to_tree(tree);
    subMonitorRight->to_tree(tree);
}
