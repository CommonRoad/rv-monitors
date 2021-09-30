//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "atomic_monitor.h"

Verdict AtomicMonitor::step(size_t parentId) {
    if (traceIdx < trace.value().getLength()) {
        auto event{trace.value().getEvent(traceIdx)};
        auto verdict{
            Verdict(event.getTimeStep(), event.getValue(*std::dynamic_pointer_cast<Atomic>(formula.getExpr())))};
        ++traceIdx;
        return verdict;
    } else
        return Verdict(false);
}

void AtomicMonitor::setTrace(const Trace &tr) { trace = tr; }
void AtomicMonitor::reset() { traceIdx = 0; }
