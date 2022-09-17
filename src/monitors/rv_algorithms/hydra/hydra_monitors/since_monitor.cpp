//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "since_monitor.h"

Verdict SinceMonitorHydra::concreteStep() {
    subVerdictF = subMonitorF->concreteStep();
    if (subVerdictF->timeStamp)
        positionMonitorF.update(subVerdictF->timeStamp.value());
    if (!subVerdictF->value)
        last_viol = std::make_pair(positionMonitorF.ts, positionMonitorF.tp);

    if (positionMonitorG.tp == -1) {
        subVerdictG = subMonitorG->concreteStep();
        if (subVerdictG->timeStamp)
            positionMonitorG.update(subVerdictG->timeStamp.value());
    }

    while (!positionMonitorG.eof && positionMonitorG.tp <= positionMonitorF.tp &&
           positionMonitorF.ts - positionMonitorG.ts >= from) {
        if (subVerdictG->value)
            last_sat = std::make_pair(positionMonitorG.ts, positionMonitorG.tp);

        subVerdictG = subMonitorG->concreteStep();
        if (subVerdictG->timeStamp)
            positionMonitorG.update(subVerdictG->timeStamp.value());
        else {
            positionMonitorG.eof = true;
            positionMonitorG.tp++;
        }
    }

    int verd = static_cast<int>(last_sat && positionMonitorF.ts - last_sat->first <= to &&
                                (!last_viol || last_sat->second >= last_viol->second));
    return {positionMonitorF.ts, verd != 0};
}
void SinceMonitorHydra::setTrace(const Trace &tra) {
    subMonitorF->setTrace(tra);
    subMonitorG->setTrace(tra);
}
void SinceMonitorHydra::reset() {
    subMonitorG->reset();
    subMonitorF->reset();
    subVerdictF = {};
    subVerdictG = {};
    last_sat = {};
    last_viol = {};
    positionMonitorG.reset();
    positionMonitorF.reset();
}

std::string SinceMonitorHydra::to_string() {
    std::string interval{" "};
    if (from != 0 or to != 10000)
        interval = "[" + std::to_string(from) + ", " + std::to_string(to) + "] ";
    return "(" + subMonitorF->to_string() + " S" + interval + subMonitorG->to_string() + ")";
}