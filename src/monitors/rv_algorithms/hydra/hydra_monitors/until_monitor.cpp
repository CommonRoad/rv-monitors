//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "until_monitor.h"

void UntilMonitorHydra::handleEof() {
    if (positionMonitorF.eof || positionMonitorG.eof)
        throw std::runtime_error("Hydra UntilMonitor readFG error in end of line");
    positionMonitorF.tp++;
    positionMonitorF.eof = true;
    positionMonitorG.tp++;
    positionMonitorG.eof = true;
}

void UntilMonitorHydra::readFG() {
    if (positionMonitorF.tp != -1 && positionMonitorF.ts != positionMonitorG.ts &&
        positionMonitorF.tp != positionMonitorG.tp)
        throw std::runtime_error("Hydra UntilMonitor readFG error");

    subVerdictF = subMonitorF->concreteStep();
    if (subVerdictF->timeStamp)
        positionMonitorF.update(subVerdictF->timeStamp.value());
    else {
        handleEof();
        return;
    }
    subVerdictG = subMonitorG->concreteStep();
    if (subVerdictG->timeStamp)
        positionMonitorG.update(subVerdictG->timeStamp.value());
    else
        handleEof();
}

Verdict UntilMonitorHydra::concreteStep() {
    subVerdictH = subMonitorH->concreteStep();
    if (subVerdictH->timeStamp)
        positionMonitorH.update(subVerdictH->timeStamp.value());

    if (last_sat && last_sat->second < positionMonitorH.tp)
        last_sat = {};
    if (last_viol && last_viol->second < positionMonitorH.tp)
        last_viol = {};

    if (positionMonitorF.tp == -1)
        readFG();

    while (!positionMonitorF.eof && !last_viol &&
           (!interval.isFinite() or positionMonitorF.ts - positionMonitorH.ts <= interval.getEnd())) {
        if (subVerdictG->value) {
            last_sat = std::make_pair(positionMonitorF.ts, positionMonitorF.tp);
        }
        if (!subVerdictF->value) {
            last_viol = std::make_pair(positionMonitorF.ts, positionMonitorF.tp);
        }
        readFG();
    }

    int verd = static_cast<int>(last_sat && last_sat->first - positionMonitorH.ts >= interval.getStart());
    return {positionMonitorH.ts, verd != 0};
}
void UntilMonitorHydra::setTrace(const Trace &tra) {
    subMonitorF->setTrace(tra);
    subMonitorG->setTrace(tra);
    subMonitorH->setTrace(tra);
}
void UntilMonitorHydra::reset() {
    subMonitorG->reset();
    subMonitorF->reset();
    subMonitorH->reset();
    subVerdictF = {};
    subVerdictG = {};
    last_sat = {};
    last_viol = {};
    positionMonitorG.reset();
    positionMonitorF.reset();
    positionMonitorH.reset();
}

std::string UntilMonitorHydra::to_string() {
    std::string itv{" "};
    std::string end{};
    if (!interval.isEmpty()) {
        if (!interval.isFinite())
            end = "inf";
        else
            end = std::to_string(interval.getEnd());
        itv = "[" + std::to_string(interval.getStart()) + ", " + end + "] ";
    }
    return "(" + subMonitorF->to_string() + " U" + itv + subMonitorG->to_string() + ")";
}
