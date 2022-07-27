//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "until_monitor.h"

size_t UntilMonitor::stepLeftRightMonitors(size_t stepNumber) {
    subVerdictLeft = subMonitorLeft->step(monitorId, stepNumber, true);
    if (subVerdictLeft->timeStamp)
        headPositionMonitorLeft.update(subVerdictLeft->timeStamp.value());
    else {
        endReached = true;
        return ++stepNumber;
    }
    subVerdictRight = subMonitorRight->step(monitorId, stepNumber, true);
    if (subVerdictRight->timeStamp)
        headPositionMonitorRight.update(subVerdictRight->timeStamp.value());
    else
        endReached = true;
    return ++stepNumber;
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

    if (stepNumber == 0) { // only executed at first call of function
        stepNumber = stepLeftRightMonitors(stepNumber);
    }

    while (!endReached && !lastViolation &&
           (headPositionMonitorLeft.timeStep - headPositionMonitorBaseTimeStep.timeStep <=
               to || headPositionMonitorLeft.timeStep < headPositionMonitorBaseTimeStep.timeStep)) { // loop until end is reached, until left side of monitor is false, or until upper bound of time
                     // interval is violated
        if (subVerdictRight->value) {
            lastSatisfaction = HeadPos(headPositionMonitorRight.timeStep, headPositionMonitorRight.traceIdx);
        }
        if (!subVerdictLeft->value) {
            lastViolation = HeadPos(headPositionMonitorLeft.timeStep, headPositionMonitorLeft.traceIdx);
        }
        stepNumber = stepLeftRightMonitors(stepNumber);
    }

    // check whether right side of until was satisfied and whether satisfaction is after start of allowed time interval
    if (lastSatisfaction && lastSatisfaction->timeStep - headPositionMonitorBaseTimeStep.timeStep >= from)
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
    HydraMonitor::reset();
    subMonitorRight->reset();
    subMonitorLeft->reset();
    subMonitorBaseTimeStep->reset();
    subVerdictLeft = {};
    subVerdictRight = {};
    subVerdictBaseTimeStep = {};
    lastSatisfaction = {};
    lastViolation = {};
    headPositionMonitorRight.reset();
    headPositionMonitorLeft.reset();
    headPositionMonitorBaseTimeStep.reset();
}
std::string UntilMonitor::to_string() {
    std::string interval{" "};
    if (from != 0 or to != 10000)
        interval = "[" + std::to_string(from) + ", " + std::to_string(to) + "] ";
    return "(" + subMonitorLeft->to_string() + " U" + interval + subMonitorRight->to_string() + ")";
}
