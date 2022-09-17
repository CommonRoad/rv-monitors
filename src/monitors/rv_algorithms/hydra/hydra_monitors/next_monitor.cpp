//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "next_monitor.h"
#include <iostream>

Verdict NextMonitorHydra::concreteStep() {
    do {
        auto subVerdict = subMonitor->concreteStep();
        if (subVerdict.timeStamp and currentTime) {
            size_t lastTime = *currentTime;
            currentTime = subVerdict.timeStamp;
            if (Utils::isInInterval(subVerdict.timeStamp.value() - lastTime, from, to))
                return {lastTime, subVerdict.value};
            else
                return {lastTime, false};
        } else if (!subVerdict.timeStamp) {
            return {*currentTime, false};
        } else {
            currentTime = subVerdict.timeStamp;
        }
    } while (true);
}

void NextMonitorHydra::setTrace(const Trace &tra) { subMonitor->setTrace(tra); }

void NextMonitorHydra::reset() {
    currentTime = {};
    subMonitor->reset();
}

std::string NextMonitorHydra::to_string() {
    std::string interval;
    if (from != 0 or to != 10000)
        interval = "[" + std::to_string(from) + ", " + std::to_string(to) + "]";
    return std::string{"X"} + interval + "(" + subMonitor->to_string() + ")";
}