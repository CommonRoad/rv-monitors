//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "previous_monitor.h"

Verdict PreviousMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    Verdict subVerdict = subMonitor->step(monitorId, stepNumber);

    if (currentVerdict.has_value() and subVerdict.ts.has_value()) {
        Verdict lastSubVerdict = *currentVerdict;
        currentVerdict = subVerdict;
        if (Utils::isInInterval(subVerdict.ts.value() - lastSubVerdict.ts.value(), from, to))
            return {subVerdict.ts.value(), lastSubVerdict.b};
        else
            return {subVerdict.ts.value(), false};
    } else if (subVerdict.ts.has_value()) {
        currentVerdict = subVerdict;
        return {subVerdict.ts.value(), false};
    } else
        return {false};
}
void PreviousMonitor::setTrace(const Trace &tra) { subMonitor->setTrace(tra); }

void PreviousMonitor::reset() {
    HydraMonitor::reset();
    subMonitor->reset();
    currentVerdict = {};
}
std::string PreviousMonitor::to_string() {
    std::string interval;
    if(from != 0 or to != 10000)
        interval = "[" + std::to_string(from) + ", " + std::to_string(to) + "]";
    return std::string{"P"} + interval + "(" + subMonitor->to_string() + ")";
}
