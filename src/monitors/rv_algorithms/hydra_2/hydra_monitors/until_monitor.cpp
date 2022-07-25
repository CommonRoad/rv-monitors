//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "until_monitor.h"

void UntilMonitor::stepLeftRightMonitors(size_t stepNumber) {
    subVerdictLeft = subMonitorLeft->step(monitorId, stepNumber, true);
    if (subVerdictLeft->ts)
        headPositionMonitorLeft.update(subVerdictLeft->ts.value());
    else {
        endReached = true;
        return;
    }
    subVerdictRight = subMonitorRight->step(monitorId, stepNumber, true);
    if (subVerdictRight->ts)
        headPositionMonitorRight.update(subVerdictRight->ts.value());
    else
        endReached = true;
}

Verdict UntilMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    subVerdictTmp = subMonitorTmp->step(monitorId, stepNumber, true);
    if (subVerdictTmp->ts)
        headPositionMonitorTmp.update(subVerdictTmp->ts.value());

    if (lastSatisfaction && lastSatisfaction->tp < headPositionMonitorTmp.tp)
        lastSatisfaction = {};
    if (lastViolation && lastViolation->tp < headPositionMonitorTmp.tp)
        lastViolation = {};

    if (stepNumber == 0) { // only executed at first call of function
        stepLeftRightMonitors(stepNumber);
        stepNumber += 1;
    }

    while (!endReached && !lastViolation &&
           headPositionMonitorLeft.ts - headPositionMonitorTmp.ts <= to) {
        if (subVerdictRight->b) {
            lastSatisfaction = HeadPos(headPositionMonitorRight.ts, headPositionMonitorRight.tp);
        }
        if (!subVerdictLeft->b) {
            lastViolation = HeadPos(headPositionMonitorLeft.ts, headPositionMonitorLeft.tp);
        }
        stepLeftRightMonitors(stepNumber);
        stepNumber += 1;
    }

    int verd = static_cast<int>(lastSatisfaction && lastSatisfaction->ts - headPositionMonitorTmp.ts >= from);
    return {headPositionMonitorTmp.ts, verd != 0};
}
void UntilMonitor::setTrace(const Trace &tra) {
    subMonitorLeft->setTrace(tra);
    subMonitorRight->setTrace(tra);
    subMonitorTmp->setTrace(tra);
}
void UntilMonitor::reset() {
    HydraMonitor::reset();
    subMonitorRight->reset();
    subMonitorLeft->reset();
    subMonitorTmp->reset();
    subVerdictLeft = {};
    subVerdictRight = {};
    subVerdictTmp = {};
    lastSatisfaction = {};
    lastViolation = {};
    headPositionMonitorRight.reset();
    headPositionMonitorLeft.reset();
    headPositionMonitorTmp.reset();
}
std::string UntilMonitor::to_string() {
    std::string interval{" "};
    if (from != 0 or to != 10000)
        interval = "[" + std::to_string(from) + ", " + std::to_string(to) + "] ";
    return "(" + subMonitorLeft->to_string() + " U" + interval + subMonitorRight->to_string() + ")";
}
