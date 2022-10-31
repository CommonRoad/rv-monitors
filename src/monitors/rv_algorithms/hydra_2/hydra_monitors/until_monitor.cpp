//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "until_monitor.h"

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
    if (stepNumberUntil == 0 or
        (subVerdictLeft->timeStamp.has_value() and
         subVerdictLeft->timeStamp.value() < headPositionMonitorBaseTimeStep.timeStep) or
        (subVerdictRight->timeStamp.has_value() and
         subVerdictRight->timeStamp.value() <
             headPositionMonitorBaseTimeStep.timeStep)) { // only executed at first call of function
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

void UntilMonitor::setTrace(const Trace &tra) {
    subMonitorLeft->setTrace(tra);
    subMonitorRight->setTrace(tra);
    subMonitorBaseTimeStep->setTrace(tra);
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
