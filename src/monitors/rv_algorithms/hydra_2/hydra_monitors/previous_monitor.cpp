//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "previous_monitor.h"

Verdict PreviousMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    Verdict subVerdict = subMonitor->step(monitorId, stepNumber);

    if (currentVerdict.has_value() and subVerdict.timeStamp.has_value()) {
        Verdict lastSubVerdict = *currentVerdict;
        currentVerdict = subVerdict;
        if (Utils::isInInterval(subVerdict.timeStamp.value() - lastSubVerdict.timeStamp.value(), from, to))
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

void PreviousMonitor::setVariableValues(std::map<std::string, Value> values) {
    subMonitor->setVariableValues(values);
}

void PreviousMonitor::reset(std::optional<size_t> stepNumber) {
    HydraMonitor::reset(stepNumber);
    subMonitor->reset(stepNumber);
    currentVerdict = {};
}

std::shared_ptr<HydraMonitor> PreviousMonitor::clone() {
    auto clonedMonitor = std::make_shared<PreviousMonitor>(*this);
    clonedMonitor->subMonitor = subMonitor->clone();
    return clonedMonitor;
}

std::string PreviousMonitor::to_string() {
    std::string interval;
    if (from != 0 or to != 10000)
        interval = "[" + std::to_string(from) + ", " + std::to_string(to) + "]";
    return std::string{"P"} + interval + "(" + subMonitor->to_string() + ")";
}
