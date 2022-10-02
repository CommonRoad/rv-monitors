//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "since_monitor.h"

Verdict SinceMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    subVerdictLeft = subMonitorLeft->step(monitorId, stepNumber);
    if (subVerdictLeft->timeStamp.has_value())
        headPositionMonitorLeft.update(subVerdictLeft->timeStamp.value());
    if (!subVerdictLeft->value)
        lastViolation = HeadPosHydra2(headPositionMonitorLeft.timeStep, headPositionMonitorLeft.traceIdx);

    if (stepNumber == 0) {
        subVerdictRight = subMonitorRight->step(monitorId, stepNumberRight);
        stepNumberRight++;
        if (subVerdictRight->timeStamp.has_value())
            headPositionMonitorRight.update(subVerdictRight->timeStamp.value());
    }

    // loop until end is reached, right monitor time points equals (is greater) than left monitor's time point, or lower
    // time bound is violated
    while (!endReached && headPositionMonitorRight.traceIdx <= headPositionMonitorLeft.traceIdx &&
           headPositionMonitorLeft.timeStep - headPositionMonitorRight.timeStep >= interval.getStart()) {
        if (subVerdictRight->value)
            lastSatisfaction = HeadPosHydra2(headPositionMonitorRight.timeStep, headPositionMonitorRight.traceIdx);
        subVerdictRight = subMonitorRight->step(monitorId, stepNumberRight);
        stepNumberRight++;
        if (subVerdictRight->timeStamp.has_value())
            headPositionMonitorRight.update(subVerdictRight->timeStamp.value());
        else {
            endReached = true;
            headPositionMonitorRight.traceIdx++;
        }
    }

    // check whether right side of until was satisfied and whether satisfaction is before end of allowed time interval
    if (lastSatisfaction &&
        (!interval.isFinite() or headPositionMonitorLeft.timeStep - lastSatisfaction->timeStep <= interval.getEnd()) &&
        (!lastViolation || lastSatisfaction->traceIdx >= lastViolation->traceIdx))
        return {headPositionMonitorLeft.timeStep, true};
    else
        return {headPositionMonitorLeft.timeStep, false};
}

void SinceMonitor::setTrace(const Trace &tra) {
    subMonitorLeft->setTrace(tra);
    subMonitorRight->setTrace(tra);
    interval.setTrace(tra);
}
void SinceMonitor::reset() {
    Hydra2Monitor::reset();
    subMonitorRight->reset();
    subMonitorLeft->reset();
    subVerdictLeft = {};
    subVerdictRight = {};
    lastSatisfaction = {};
    lastViolation = {};
    headPositionMonitorRight.reset();
    headPositionMonitorLeft.reset();
    stepNumberRight = 0;
    endReached = false;
}

std::string SinceMonitor::to_string() {
    return "(" + subMonitorLeft->to_string() + " S" + interval.to_string() + " " + subMonitorRight->to_string() + ")";
}

void SinceMonitor::updateDynamicInterval(size_t stepNumber) {
    subMonitorRight->updateDynamicInterval(stepNumber);
    subMonitorLeft->updateDynamicInterval(stepNumber);
    interval.updateDynamicInterval(stepNumber);
}
