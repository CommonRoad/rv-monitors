//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "atomic_monitor.h"

Verdict AtomicMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    if (traceIdx[parentId] < trace.getLength()) {
        auto event{trace.getEvent(traceIdx[parentId])};
        auto verdict{
            Verdict(event.getTimeStep(), event.getValue(*std::dynamic_pointer_cast<Atomic>(formula.getExpr())))};
        return verdict;
    } else
        return Verdict(false);
}

void AtomicMonitor::setTrace(const Trace &tr) { trace = tr; }

std::string AtomicMonitor::to_string() { return std::string(); }
