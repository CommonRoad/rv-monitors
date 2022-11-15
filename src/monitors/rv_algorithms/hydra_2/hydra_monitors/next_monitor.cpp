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

void NextMonitor::setTrace(const Trace &tra) { subMonitor->setTrace(tra); }

void NextMonitor::setVariableValues(std::map<std::string, Value> values) { subMonitor->setVariableValues(values); }

void NextMonitor::reset() {
    Hydra2Monitor::reset();
    currentTime = {};
    subMonitor->reset();
}

std::shared_ptr<Hydra2Monitor> NextMonitor::clone() {
    auto clonedMonitor = std::make_shared<NextMonitor>(*this);
    clonedMonitor->subMonitor = subMonitor->clone();
    return clonedMonitor;
}

std::string NextMonitor::to_string() {
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
