//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "predicate_monitor.h"

std::optional<Verdict> PredicateMonitor::step() {
    if(traceIdx < trace.value().getLength()) {
        auto event{trace.value().getEvent(traceIdx)};
        auto verdict{Verdict(event.getTimeStep(),
                          event.getValue(*std::dynamic_pointer_cast<Predicate>(formula.getExpr())))};
        ++traceIdx;
        return verdict;
    } else
        return {};
}

void PredicateMonitor::setTrace(const Trace &tr) { trace = tr; }
