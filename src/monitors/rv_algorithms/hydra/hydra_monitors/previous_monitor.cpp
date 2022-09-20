//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "previous_monitor.h"

Verdict PreviousMonitorHydra::concreteStep() {
    Verdict subVerdict = subMonitor->concreteStep();

    if (currentVerdict and subVerdict.timeStamp) {
        Verdict lastSubVerdict = *currentVerdict;
        currentVerdict = subVerdict;
        if (Utils::isInInterval(subVerdict.timeStamp.value() - lastSubVerdict.timeStamp.value(), from, to))
            return {subVerdict.timeStamp.value(), lastSubVerdict.value};
        else
            return {subVerdict.timeStamp.value(), false};
    } else if (subVerdict.timeStamp) {
        currentVerdict = subVerdict;
        return {subVerdict.timeStamp.value(), false};
    } else
        return {false};
}

void PreviousMonitorHydra::setTrace(const Trace &tra) { subMonitor->setTrace(tra); }

void PreviousMonitorHydra::reset() {
    subMonitor->reset();
    currentVerdict = {};
}

std::string PreviousMonitorHydra::to_string() {
    std::string interval;
    if (from != 0 or to != 10000)
        interval = "[" + std::to_string(from) + ", " + std::to_string(to) + "]";
    return std::string{"P"} + interval + "(" + subMonitor->to_string() + ")";
}