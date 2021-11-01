//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "next_monitor.h"
#include <iostream>

Verdict NextMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    do {
        auto subVerdict = subMonitor->step(monitorId, stepNumber);
        if (subVerdict.ts.has_value() and currentTime) {
            size_t lastTime = *currentTime;
            currentTime = subVerdict.ts;
            if (Utils::isInInterval(subVerdict.ts.value() - lastTime, from, to))
                return Verdict(lastTime, subVerdict.b);
            else
                return Verdict(lastTime, false);
        } else if (!subVerdict.ts.has_value()) {
            return Verdict(*currentTime, false);
        } else {
            currentTime = subVerdict.ts;
        }
    } while (true);
}

void NextMonitor::setTrace(const Trace &tr) { subMonitor->setTrace(tr); }

void NextMonitor::reset() {
    HydraMonitor::reset();
    currentTime = {};
    subMonitor->reset();
}
