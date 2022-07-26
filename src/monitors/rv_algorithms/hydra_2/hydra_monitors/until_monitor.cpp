//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "until_monitor.h"

size_t UntilMonitor::stepLeftRightMonitors(size_t stepNumber) {
    subVerdictLeft = subMonitorLeft->step(monitorId, stepNumber, true);
    if (subVerdictLeft->ts)
        headPositionMonitorLeft.update(subVerdictLeft->ts.value());
    else {
        endReached = true;
        return stepNumber++;
    }
    subVerdictRight = subMonitorRight->step(monitorId, stepNumber, true);
    if (subVerdictRight->ts)
        headPositionMonitorRight.update(subVerdictRight->ts.value());
    else
        endReached = true;
    return stepNumber++;
}

Verdict UntilMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    subVerdictBaseTimeStep = subMonitorBaseTimeStep->step(monitorId, stepNumber, true);
    if (subVerdictBaseTimeStep->ts)
        headPositionMonitorBaseTimeStep.update(subVerdictBaseTimeStep->ts.value());

    if (lastSatisfaction &&
        lastSatisfaction->tp <
            headPositionMonitorBaseTimeStep.tp) // if last satisfaction (-> right side of until satisfied) is before
                                                // current time step, reset last satisfaction
        lastSatisfaction = {};
    if (lastViolation &&
        lastViolation->tp < headPositionMonitorBaseTimeStep.tp) // if last violation (-> left side of until violated) is
                                                                // before current time step, reset last satisfaction
        lastViolation = {};

    if (stepNumber == 0) { // only executed at first call of function
        stepNumber = stepLeftRightMonitors(stepNumber);
    }

    while (!endReached && !lastViolation &&
           headPositionMonitorLeft.ts - headPositionMonitorBaseTimeStep.ts <=
               to) { // loop until end is reached, until left side of monitor is false, or until upper bound of time
                     // interval is violated
        if (subVerdictRight->b) {
            lastSatisfaction = HeadPos(headPositionMonitorRight.ts, headPositionMonitorRight.tp);
        }
        if (!subVerdictLeft->b) {
            lastViolation = HeadPos(headPositionMonitorLeft.ts, headPositionMonitorLeft.tp);
        }
        stepNumber = stepLeftRightMonitors(stepNumber);
    }

    if (lastSatisfaction && lastSatisfaction->ts - headPositionMonitorBaseTimeStep.ts >= from)
        return {headPositionMonitorBaseTimeStep.ts, true};
    else
        return {headPositionMonitorBaseTimeStep.ts, false};
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
