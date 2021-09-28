//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "previous_monitor.h"

std::optional<Verdict> PreviousMonitor::step() {
    std::optional<Verdict> subVerdict = subMonitor->get()->step();

    if (currentVerdict) {
        Verdict lastSubVerdict = *currentVerdict;
        currentVerdict = subVerdict;
        if (Utils::isInInterval(subVerdict.value().ts - lastSubVerdict.ts, from.value(), to.value()))
            return Verdict(subVerdict.value().ts, lastSubVerdict.b);
        else
            return Verdict(subVerdict.value().ts, false);
    } else {
        currentVerdict = subVerdict;
        return Verdict(subVerdict.value().ts, false);
    }
}
void PreviousMonitor::setTrace(const Trace &tr) { subMonitor.value()->setTrace(tr); }
