//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/previous_monitor.h"

Verdict PreviousMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    Verdict subVerdict = subMonitor->step(monitorId, stepNumber);

    if (currentVerdict.has_value() and subVerdict.timeStamp.has_value()) {
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
    } else if (subVerdict.timeStamp.has_value()) { // first trace element
        currentVerdict = subVerdict;
        return {subVerdict.timeStamp.value(), false};
    } else // behind trace (no more trace element exist)
        return {false};
}
void PreviousMonitor::setTrace(const Trace &tra) { subMonitor->setTrace(tra); }

void PreviousMonitor::reset() {
    Hydra2Monitor::reset();
    subMonitor->reset();
    currentVerdict = {};
}
std::string PreviousMonitor::to_string() {
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
