//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "since_monitor.h"

Verdict SinceMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    subVerdictLeft = subMonitorLeft->step(monitorId, stepNumber);
    if (subVerdictLeft->ts.has_value())
        headPositionMonitorLeft.update(subVerdictLeft->ts.value());
    if (!subVerdictLeft->b)
        lastViolation = HeadPos(headPositionMonitorLeft.ts, headPositionMonitorLeft.tp);

    if (stepNumber == 0) {
        subVerdictRight = subMonitorRight->step(monitorId, stepNumberRight);
        stepNumberRight += 1;
        if (subVerdictRight->ts.has_value())
            headPositionMonitorRight.update(subVerdictRight->ts.value());
    }

    while (!endReached && headPositionMonitorRight.tp <= headPositionMonitorLeft.tp &&
           headPositionMonitorLeft.ts - headPositionMonitorRight.ts >= from) {
        if (subVerdictRight->b)
            lastSatisfaction = HeadPos(headPositionMonitorRight.ts, headPositionMonitorRight.tp);
        subVerdictRight = subMonitorRight->step(monitorId, stepNumberRight);
        stepNumberRight += 1;
        if (subVerdictRight->ts.has_value())
            headPositionMonitorRight.update(subVerdictRight->ts.value());
        else {
            endReached = true;
            headPositionMonitorRight.tp++;
        }
    }

    if (lastSatisfaction && headPositionMonitorLeft.ts - lastSatisfaction->ts <= to &&
        (!lastViolation || lastSatisfaction->tp >= lastViolation->tp))
        return {headPositionMonitorLeft.ts, true};
    else
        return {headPositionMonitorLeft.ts, false};
}

void SinceMonitor::setTrace(const Trace &tra) {
    subMonitorLeft->setTrace(tra);
    subMonitorRight->setTrace(tra);
}
void SinceMonitor::reset() {
    HydraMonitor::reset();
    subMonitorRight->reset();
    subMonitorLeft->reset();
    subVerdictLeft = {};
    subVerdictRight = {};
    lastSatisfaction = {};
    lastViolation = {};
    headPositionMonitorRight.reset();
    headPositionMonitorLeft.reset();
    stepNumberRight = 0;
}
std::string SinceMonitor::to_string() {
    std::string interval{" "};
    if (from != 0 or to != 10000)
        interval = "[" + std::to_string(from) + ", " + std::to_string(to) + "] ";
    return "(" + subMonitorLeft->to_string() + " S" + interval + subMonitorRight->to_string() + ")";
}
