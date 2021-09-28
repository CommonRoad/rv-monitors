//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "next_monitor.h"
#include <iostream>

std::optional<Verdict> NextMonitor::step() {
    do {
        std::optional<Verdict> subVerdict = subMonitor->get()->step();
        if (subVerdict and currentTime) {
            size_t lastTime = *currentTime;
            currentTime = subVerdict.value().ts;
            if (Utils::isInInterval(subVerdict.value().ts - lastTime, from.value(), to.value()))
                return Verdict(lastTime, subVerdict.value().b);
            else
                return Verdict(lastTime, false);
        } else if (!subVerdict) {
            return Verdict(*currentTime, false);
        } else {
            currentTime = subVerdict.value().ts;
        }
    } while (true);
}

void NextMonitor::setTrace(const Trace &tr) { subMonitor.value()->setTrace(tr); }
