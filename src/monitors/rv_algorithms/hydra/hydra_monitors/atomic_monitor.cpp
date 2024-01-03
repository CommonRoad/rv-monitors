//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "monitors/rv_algorithms/hydra/hydra_monitors/atomic_monitor.h"

Verdict AtomicMonitorHydra::concreteStep() {
    if (traceIdx < trace.value().getLength()) {
        auto event{trace.value().getEvent(traceIdx)};
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
