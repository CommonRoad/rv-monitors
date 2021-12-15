//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "since_monitor.h"

Verdict SinceMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    subVerdictF = subMonitorF->step(monitorId, stepNumber);
    if (subVerdictF->ts.has_value())
        positionMonitorF.update(subVerdictF->ts.value());
    else if (!subVerdictF->b)
        lastViolation = std::make_pair(positionMonitorF.ts, positionMonitorF.tp);

    if (positionMonitorG.tp == -1) {
        subVerdictG = subMonitorG->step(monitorId, stepNumber);
        stepNumber += 1;
        if (subVerdictG->ts.has_value())
            positionMonitorG.update(subVerdictG->ts.value());
    }

    while (!positionMonitorG.eof && positionMonitorG.tp <= positionMonitorF.tp &&
           positionMonitorF.ts - positionMonitorG.ts >= from) {
        if (subVerdictG->b)
            lastSatisfaction = std::make_pair(positionMonitorG.ts, positionMonitorG.tp);
        subVerdictG = subMonitorG->step(monitorId, stepNumber);
        stepNumber += 1;
        if (subVerdictG->ts.has_value())
            positionMonitorG.update(subVerdictG->ts.value());
        else {
            positionMonitorG.eof = true;
            positionMonitorG.tp++;
        }
    }

    int v = static_cast<int>(lastSatisfaction && positionMonitorF.ts - lastSatisfaction->first <= to &&
                             (!lastViolation || lastSatisfaction->second >= lastViolation->second));
    return {positionMonitorF.ts, v != 0};
}
void SinceMonitor::setTrace(const Trace &tra) {
    subMonitorF->setTrace(tra);
    subMonitorG->setTrace(tra);
}
void SinceMonitor::reset() {
    HydraMonitor::reset();
    subMonitorG->reset();
    subMonitorF->reset();
    subVerdictF = {};
    subVerdictG = {};
    lastSatisfaction = {};
    lastViolation = {};
    positionMonitorG.reset();
    positionMonitorF.reset();
}
std::string SinceMonitor::to_string() {
    return "(" + subMonitorF->to_string() + " S " + subMonitorG->to_string() + ")";
}
