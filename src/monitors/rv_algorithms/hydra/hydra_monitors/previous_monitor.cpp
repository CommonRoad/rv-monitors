//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "previous_monitor.h"

Verdict PreviousMonitor::concreteStep(size_t parentId) {
    Verdict subVerdict = subMonitor->step(id);

    if (currentVerdict and subVerdict.ts) {
        Verdict lastSubVerdict = *currentVerdict;
        currentVerdict = subVerdict;
        if (Utils::isInInterval(subVerdict.ts.value() - lastSubVerdict.ts.value(), from, to))
            return Verdict(subVerdict.ts.value(), lastSubVerdict.b);
        else
            return Verdict(subVerdict.ts.value(), false);
    } else if (subVerdict.ts) {
        currentVerdict = subVerdict;
        return Verdict(subVerdict.ts.value(), false);
    } else
        return Verdict(false);
}
void PreviousMonitor::setTrace(const Trace &tr) { subMonitor->setTrace(tr); }

void PreviousMonitor::reset() {
    HydraMonitor::reset();
    subMonitor->reset();
    currentVerdict = {};
}
