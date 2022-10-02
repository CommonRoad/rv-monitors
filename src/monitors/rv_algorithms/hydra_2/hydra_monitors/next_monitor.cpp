//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "next_monitor.h"
#include <iostream>

#include "monitors/rv_algorithms/hydra_2/interval/interval_function.h"
#include "predicate_monitor.h"

Verdict NextMonitor::concreteStep(size_t parentId, size_t stepNumber) {

    if (!currentTime) // first event in trace (next time step specifies verdict
        currentTime = subMonitor->step(monitorId, stepNumber).timeStamp;

    auto subVerdict{subMonitor->step(monitorId, stepNumber + 1)};
    if (subVerdict.timeStamp.has_value() and currentTime) {
        size_t lastTime = *currentTime;
        currentTime = subVerdict.timeStamp;
        if (!interval.isFinite() and interval.getStart() <= lastTime)
            return {lastTime, subVerdict.value};
        else if (interval.isFinite() and
                 Utils::isInInterval(subVerdict.timeStamp.value() - lastTime, interval.getStart(), interval.getEnd()))
            return {lastTime, subVerdict.value};
        else
            return {lastTime, false};
    } else // last event in trace
        return {*currentTime, false};
}

void NextMonitor::setTrace(const Trace &tra) {
    subMonitor->setTrace(tra);
    interval.setTrace(tra);
}

void NextMonitor::reset() {
    Hydra2Monitor::reset();
    currentTime = {};
    subMonitor->reset();
}
std::string NextMonitor::to_string() {
    return std::string{"X"} + interval.to_string() + "(" + subMonitor->to_string() + ")";
}
void NextMonitor::updateDynamicInterval(size_t stepNumber) {
    subMonitor->updateDynamicInterval(stepNumber);
    interval.updateDynamicInterval(stepNumber);
}
