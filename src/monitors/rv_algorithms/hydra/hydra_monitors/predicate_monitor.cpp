//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "monitors/rv_algorithms/hydra/hydra_monitors/predicate_monitor.h"

Verdict PredicateMonitorHydra::concreteStep() {
    if (traceIdx < trace.value().getLength()) {
        auto event{trace.value().getEvent(traceIdx)};
        auto verdict{
            Verdict(event.getTimeStep(), event.getValue(*std::dynamic_pointer_cast<Predicate>(formula.getExpr())))};
        ++traceIdx;
        return verdict;
    } else
        return {false};
}

void PredicateMonitorHydra::setTrace(const Trace &tra) { trace = tra; }

void PredicateMonitorHydra::reset() { traceIdx = 0; }

std::string PredicateMonitorHydra::to_string() { return formula.to_string(); }
