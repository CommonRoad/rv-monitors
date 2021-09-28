//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "since_monitor.h"

Verdict SinceMonitor::step() {
    subVerdictF = subMonitorF->step();
    if (subVerdictF->ts)
        positionMonitorF.update(subVerdictF->ts.value());
    else if (!subVerdictF->b)
        last_viol = std::make_pair(positionMonitorF.ts, positionMonitorF.tp);

    if (positionMonitorG.tp == -1) {
        subVerdictG = subMonitorG->step();
        if (subVerdictG->ts)
            positionMonitorG.update(subVerdictG->ts.value());
    }

    while (!positionMonitorG.eof && positionMonitorG.tp <= positionMonitorF.tp &&
           positionMonitorF.ts - positionMonitorG.ts >= from) {
        if (subVerdictG->b)
            last_sat = std::make_pair(positionMonitorG.ts, positionMonitorG.tp);

        subVerdictG = subMonitorG->step();
        if (subVerdictG->ts)
            positionMonitorG.update(subVerdictG->ts.value());
        else {
            positionMonitorG.eof = true;
            positionMonitorG.tp++;
        }
    }

    int v = static_cast<int>(last_sat && positionMonitorF.ts - last_sat->first <= to &&
                             (!last_viol || last_sat->second >= last_viol->second));
    return Verdict(positionMonitorF.ts, v != 0);
}
void SinceMonitor::setTrace(const Trace &tr) {
    subMonitorF->setTrace(tr);
    subMonitorG->setTrace(tr);
}
