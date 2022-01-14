//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "until_monitor.h"

void UntilMonitor::handleEof() {
    if (!(!positionMonitorF.eof && !positionMonitorG.eof))
        throw std::runtime_error("Hydra UntilMonitor readFG error in end of line");
    positionMonitorF.tp++;
    positionMonitorF.eof = true;
    positionMonitorG.tp++;
    positionMonitorG.eof = true;
}

void UntilMonitor::readFG(size_t stepNumber) {
    if (!(positionMonitorF.tp == -1 || positionMonitorF.ts == positionMonitorG.ts ||
          positionMonitorF.tp == positionMonitorG.tp))
        throw std::runtime_error("Hydra UntilMonitor readFG error");

    subVerdictF = subMonitorF->step(monitorId, stepNumber, true);
    if (subVerdictF->ts)
        positionMonitorF.update(subVerdictF->ts.value());
    else {
        handleEof();
        return;
    }
    subVerdictG = subMonitorG->step(monitorId, stepNumber, true);
    if (subVerdictG->ts)
        positionMonitorG.update(subVerdictG->ts.value());
    else
        handleEof();
}

Verdict UntilMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    subVerdictH = subMonitorH->step(monitorId, stepNumber, true);
    if (subVerdictH->ts)
        positionMonitorH.update(subVerdictH->ts.value());

    if (lastSatisfaction && lastSatisfaction->second < positionMonitorH.tp)
        lastSatisfaction = {};
    if (lastViolation && lastViolation->second < positionMonitorH.tp)
        lastViolation = {};

    if (positionMonitorF.tp == -1) {
        readFG(stepNumber);
        stepNumber += 1;
    }

    while (!positionMonitorF.eof && !lastViolation && positionMonitorF.ts - positionMonitorH.ts <= to) {
        if (subVerdictG->b) {
            lastSatisfaction = std::make_pair(positionMonitorF.ts, positionMonitorF.tp);
        }
        if (!subVerdictF->b) {
            lastViolation = std::make_pair(positionMonitorF.ts, positionMonitorF.tp);
        }
        readFG(stepNumber);
        stepNumber += 1;
    }

    int v = static_cast<int>(lastSatisfaction && lastSatisfaction->first - positionMonitorH.ts >= from);
    return {positionMonitorH.ts, v != 0};
}
void UntilMonitor::setTrace(const Trace &tra) {
    subMonitorF->setTrace(tra);
    subMonitorG->setTrace(tra);
    subMonitorH->setTrace(tra);
}
void UntilMonitor::reset() {
    HydraMonitor::reset();
    subMonitorG->reset();
    subMonitorF->reset();
    subMonitorH->reset();
    subVerdictF = {};
    subVerdictG = {};
    subVerdictH = {};
    lastSatisfaction = {};
    lastViolation = {};
    positionMonitorG.reset();
    positionMonitorF.reset();
    positionMonitorH.reset();
}
std::string UntilMonitor::to_string() {
    return "(" + subMonitorF->to_string() + " U " + subMonitorG->to_string() + ")";
}
