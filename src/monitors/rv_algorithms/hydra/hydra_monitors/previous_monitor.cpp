//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "monitors/rv_algorithms/hydra/hydra_monitors/previous_monitor.h"

Verdict PreviousMonitorHydra::concreteStep() {
    Verdict subVerdict = subMonitor->concreteStep();

    if (currentVerdict and subVerdict.timeStamp) {
        Verdict lastSubVerdict = *currentVerdict;
        currentVerdict = subVerdict;
        if (!interval.isFinite() and
            interval.getStart() <= subVerdict.timeStamp.value() - lastSubVerdict.timeStamp.value())
            return {subVerdict.timeStamp.value(), lastSubVerdict.value};
        else if (interval.isFinite() and
                 Utils::isInInterval(subVerdict.timeStamp.value() - lastSubVerdict.timeStamp.value(),
                                     interval.getStart(), interval.getEnd()))
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
    std::string itv{};
    std::string end{};
    if (!interval.isEmpty()) {
        if (!interval.isFinite())
            end = "inf";
        else
            end = std::to_string(interval.getEnd());
        itv = "[" + std::to_string(interval.getStart()) + ", " + end + "]";
    }
    return std::string{"P"} + itv + "(" + subMonitor->to_string() + ")";
}