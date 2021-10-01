//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "predicate_monitor.h"

Verdict PredicateMonitor::concreteStep(size_t parentId) {
    if (traceIdx[parentId] < trace.getLength()) {
        auto event{trace.getEvent(traceIdx[parentId])};
        auto verdict{
            Verdict(event.getTimeStep(), event.getValue(*std::dynamic_pointer_cast<Predicate>(formula.getExpr())))};
        return verdict;
    } else
        return Verdict(false);
}

void PredicateMonitor::setTrace(const Trace &tr) { trace = tr; }
