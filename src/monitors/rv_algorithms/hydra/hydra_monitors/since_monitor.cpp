//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "since_monitor.h"

Verdict SinceMonitor::concreteStep(size_t parentId) {
    subVerdictF = subMonitorF->step(monitorId);
    if (subVerdictF->ts)
        positionMonitorF.update(subVerdictF->ts.value());
    else if (!subVerdictF->b)
        lastViolation = std::make_pair(positionMonitorF.ts, positionMonitorF.tp);

    if (positionMonitorG.tp == -1) {
        subVerdictG = subMonitorG->step(monitorId);
        if (subVerdictG->ts)
            positionMonitorG.update(subVerdictG->ts.value());
    }

    while (!positionMonitorG.eof && positionMonitorG.tp <= positionMonitorF.tp &&
           positionMonitorF.ts - positionMonitorG.ts >= from) {
        if (subVerdictG->b)
            lastSatisfaction = std::make_pair(positionMonitorG.ts, positionMonitorG.tp);

        subVerdictG = subMonitorG->step(monitorId);
        if (subVerdictG->ts)
            positionMonitorG.update(subVerdictG->ts.value());
        else {
            positionMonitorG.eof = true;
            positionMonitorG.tp++;
        }
    }

    int v = static_cast<int>(lastSatisfaction && positionMonitorF.ts - lastSatisfaction->first <= to &&
                             (!lastViolation || lastSatisfaction->second >= lastViolation->second));
    return Verdict(positionMonitorF.ts, v != 0);
}
void SinceMonitor::setTrace(const Trace &tr) {
    subMonitorF->setTrace(tr);
    subMonitorG->setTrace(tr);
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
