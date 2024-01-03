//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "monitors/rv_algorithms/hydra/hydra_monitors/next_monitor.h"
#include <iostream>

Verdict NextMonitorHydra::concreteStep() {
    do {
        auto subVerdict = subMonitor->concreteStep();
        if (subVerdict.timeStamp and currentTime) {
            size_t lastTime = *currentTime;
            currentTime = subVerdict.timeStamp;
            if (!interval.isFinite() and interval.getStart() <= lastTime)
                return {lastTime, subVerdict.value};
            if (Utils::isInInterval(subVerdict.timeStamp.value() - lastTime, interval.getStart(), interval.getEnd()))
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
    std::string itv{};
    std::string end{};
    if (!interval.isEmpty()) {
        if (!interval.isFinite())
            end = "inf";
        else
            end = std::to_string(interval.getEnd());
        itv = "[" + std::to_string(interval.getStart()) + ", " + end + "]";
    }
    return std::string{"X"} + itv + "(" + subMonitor->to_string() + ")";
}