//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "next_monitor.h"
#include <iostream>

Verdict NextMonitor::concreteStep(size_t parentId, size_t stepNumber) {

    if (!currentTime) // first event in trace (next time step specifies verdict
        currentTime = subMonitor->step(monitorId, stepNumber).timeStamp;

    auto subVerdict{subMonitor->step(monitorId, stepNumber + 1)};
    if (subVerdict.timeStamp.has_value() and currentTime) {
        size_t lastTime = *currentTime;
        currentTime = subVerdict.timeStamp;
        if (Utils::isInInterval(subVerdict.timeStamp.value() - lastTime, from, to))
            return {lastTime, subVerdict.value};
        else
            return {lastTime, false};
    } else // last event in trace
        return {*currentTime, false};
}

void NextMonitor::setTrace(const Trace &tra) { subMonitor->setTrace(tra); }

void NextMonitor::reset() {
    Hydra2Monitor::reset();
    currentTime = {};
    subMonitor->reset();
}
std::string NextMonitor::to_string() {
    std::string interval;
    if (from != 0 or to != 10000)
        interval = "[" + std::to_string(from) + ", " + std::to_string(to) + "]";
    return std::string{"X"} + interval + "(" + subMonitor->to_string() + ")";
}
