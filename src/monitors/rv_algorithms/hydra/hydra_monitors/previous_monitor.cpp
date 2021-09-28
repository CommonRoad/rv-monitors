//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "previous_monitor.h"
#include "monitors/utils/utils.h"

Verdict PreviousMonitor::step() {
    Verdict subVerdict = subMonitor->get()->step();

    if (currentVerdict) {
        Verdict lastSubVerdict = *currentVerdict;
        currentVerdict = subVerdict;
        if (Utils::isInInterval(subVerdict.ts - lastSubVerdict.ts, from.value(), to.value()))
            return Verdict(subVerdict.ts, lastSubVerdict.b);
        else
            return Verdict(subVerdict.ts, false);
    } else {
        currentVerdict = subVerdict;
        return Verdict(subVerdict.ts, false);
    }
}
void PreviousMonitor::setTrace(const Trace &tr) {
    trace = tr;
    subMonitor.value()->setTrace(tr);
}
