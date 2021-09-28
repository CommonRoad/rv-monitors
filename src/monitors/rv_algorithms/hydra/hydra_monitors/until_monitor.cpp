//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "until_monitor.h"

void UntilMonitor::handleEof() {
    if (!positionMonitorF.eof && !positionMonitorG.eof)
        std::runtime_error("Hydra UntilMonitor readFG error in end of line");
    positionMonitorF.tp++;
    positionMonitorF.eof = true;
    positionMonitorG.tp++;
    positionMonitorG.eof = true;
}

void UntilMonitor::readFG() {
    if (positionMonitorF.tp == -1 || positionMonitorF.ts == positionMonitorG.ts ||
        positionMonitorF.tp == positionMonitorG.tp)
        std::runtime_error("Hydra UntilMonitor readFG error");

    subVerdictF = subMonitorF->step();
    if (subVerdictF->ts)
        positionMonitorF.update(subVerdictF->ts.value());
    else {
        handleEof();
        return;
    }
    subVerdictG = subMonitorG->step();
    if (subVerdictG->ts)
        positionMonitorG.update(subVerdictG->ts.value());
    else
        handleEof();
}

Verdict UntilMonitor::step() {
    subVerdictH = subMonitorH->step();
    if (subVerdictH->ts)
        positionMonitorH.update(subVerdictH->ts.value());

    if (last_sat && last_sat->second < positionMonitorH.tp)
        last_sat = {};
    if (last_viol && last_viol->second < positionMonitorH.tp)
        last_viol = {};

    if (positionMonitorF.tp == -1)
        readFG();

    while (!positionMonitorF.eof && !last_viol && positionMonitorF.ts - positionMonitorH.ts <= to) {
        if (subVerdictG->b) {
            last_sat = std::make_pair(positionMonitorF.ts, positionMonitorF.tp);
        }
        if (!subVerdictF->b) {
            last_viol = std::make_pair(positionMonitorF.ts, positionMonitorF.tp);
        }
        readFG();
    }

    int v = static_cast<int>(last_sat && last_sat->first - positionMonitorH.ts >= from);
    return Verdict(positionMonitorH.ts, v != 0);
}
void UntilMonitor::setTrace(const Trace &tr) {
    subMonitorF->setTrace(tr);
    subMonitorG->setTrace(tr);
    subMonitorH->setTrace(tr);
}
