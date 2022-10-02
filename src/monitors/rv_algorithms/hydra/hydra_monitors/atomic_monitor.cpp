//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "atomic_monitor.h"
#include "monitors/expression_tree/atomic/atomic.h"

Verdict AtomicMonitorHydra::concreteStep() {
    if (traceIdx < trace.value().getLength()) {
        auto event{trace.value().getBooleanEvent(traceIdx)};
        auto verdict{
            Verdict(event.getTimeStep(), event.getValue(*std::dynamic_pointer_cast<Atomic>(formula.getExpr())))};
        ++traceIdx;
        return verdict;
    } else
        return {false};
}

void AtomicMonitorHydra::setTrace(const Trace &tra) { trace = tra; }

void AtomicMonitorHydra::reset() { traceIdx = 0; }

std::string AtomicMonitorHydra::to_string() { return formula.to_string(); }
