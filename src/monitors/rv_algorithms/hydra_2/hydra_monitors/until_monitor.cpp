//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "until_monitor.h"

void UntilMonitor::stepLeftRightMonitors() {
    subVerdictLeft = subMonitorLeft->step(monitorId, stepNumberUntil, true);
    if (subVerdictLeft->timeStamp)
        headPositionMonitorLeft.update(subVerdictLeft->timeStamp.value());
    else {
        endReached = true;
        ++stepNumberUntil;
    }
    subVerdictRight = subMonitorRight->step(monitorId, stepNumberUntil, true);
    if (subVerdictRight->timeStamp)
        headPositionMonitorRight.update(subVerdictRight->timeStamp.value());
    else
        endReached = true;
    ++stepNumberUntil;
}

Verdict UntilMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    subVerdictBaseTimeStep = subMonitorBaseTimeStep->step(monitorId, stepNumber, true);
    if (subVerdictBaseTimeStep->timeStamp)
        headPositionMonitorBaseTimeStep.update(subVerdictBaseTimeStep->timeStamp.value());

    if (lastSatisfaction &&
        lastSatisfaction->traceIdx <
            headPositionMonitorBaseTimeStep.traceIdx) // if last satisfaction (-> right side of until satisfied) is
                                                      // before current time step, reset last satisfaction
        lastSatisfaction = {};
    if (lastViolation &&
        lastViolation->traceIdx <
            headPositionMonitorBaseTimeStep.traceIdx) // if last violation (-> left side of until violated) is
                                                      // before current time step, reset last satisfaction
        lastViolation = {};

    // if not step was performed yet or last verdicts are behind base time step, execute step for current base step
    if (stepNumberUntil == 0 or
        (subVerdictLeft->timeStamp.has_value() and
         subVerdictLeft->timeStamp.value() < headPositionMonitorBaseTimeStep.timeStep) or
        (subVerdictRight->timeStamp.has_value() and
         subVerdictRight->timeStamp.value() <
             headPositionMonitorBaseTimeStep.timeStep)) { // only executed at first call of function
        stepLeftRightMonitors();
    }

    while (!endReached && !lastViolation &&
           (headPositionMonitorLeft.timeStep - headPositionMonitorBaseTimeStep.timeStep <= to ||
            headPositionMonitorLeft.timeStep <
                headPositionMonitorBaseTimeStep.timeStep)) { // loop until end is reached, until left side of monitor is
                                                             // false, or until upper bound of time
                                                             // interval is violated
        if (subVerdictRight->value) {
            lastSatisfaction = HeadPos(headPositionMonitorRight.timeStep, headPositionMonitorRight.traceIdx);
        }
        if (!subVerdictLeft->value) {
            lastViolation = HeadPos(headPositionMonitorLeft.timeStep, headPositionMonitorLeft.traceIdx);
        }
        stepLeftRightMonitors();
    }
    // check whether right side of until was satisfied, whether satisfaction is after start of allowed time interval,
    // and whether left side of from is greater/equal zero (required since unsigned, and we have an overflow
    if (lastSatisfaction && lastSatisfaction->timeStep - headPositionMonitorBaseTimeStep.timeStep >= from &&
        lastSatisfaction->timeStep >= headPositionMonitorBaseTimeStep.timeStep)
        return {headPositionMonitorBaseTimeStep.timeStep, true};
    else
        return {headPositionMonitorBaseTimeStep.timeStep, false};
}

void UntilMonitor::setTrace(const Trace &tra) {
    subMonitorLeft->setTrace(tra);
    subMonitorRight->setTrace(tra);
    subMonitorBaseTimeStep->setTrace(tra);
}

void UntilMonitor::setVariableValues(std::map<std::string, Value> values) {
    subMonitorRight->setVariableValues(values);
    subMonitorLeft->setVariableValues(values);
    subMonitorBaseTimeStep->setVariableValues(values);
}

void UntilMonitor::reset(std::optional<size_t> stepNumber) {
    HydraMonitor::reset(stepNumber);
    subMonitorRight->reset(stepNumber);
    subMonitorLeft->reset(stepNumber);
    subMonitorBaseTimeStep->reset(stepNumber);
    subVerdictLeft = {};
    subVerdictRight = {};
    subVerdictBaseTimeStep = {};
    lastSatisfaction = {};
    lastViolation = {};
    headPositionMonitorRight.reset();
    headPositionMonitorLeft.reset();
    headPositionMonitorBaseTimeStep.reset();
}

std::shared_ptr<HydraMonitor> UntilMonitor::clone() {
    auto clonedMonitor = std::make_shared<UntilMonitor>(*this);
    clonedMonitor->subMonitorRight = subMonitorRight->clone();
    clonedMonitor->subMonitorLeft = subMonitorLeft->clone();
    clonedMonitor->subMonitorBaseTimeStep = subMonitorBaseTimeStep->clone();
    return clonedMonitor;
}

std::string UntilMonitor::to_string() {
    std::string interval{" "};
    if (from != 0 or to != 10000)
        interval = "[" + std::to_string(from) + ", " + std::to_string(to) + "] ";
    return "(" + subMonitorLeft->to_string() + " U" + interval + subMonitorRight->to_string() + ")";
}
