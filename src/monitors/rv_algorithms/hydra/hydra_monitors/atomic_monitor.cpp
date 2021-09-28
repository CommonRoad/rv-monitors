//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "atomic_monitor.h"

std::optional<Verdict> AtomicMonitor::step() {
    if(traceIdx < trace.value().getLength()) {
        auto event{trace.value().getEvent(traceIdx)};
        auto verdict{Verdict(event.getTimeStep(),
                             event.getValue(*std::dynamic_pointer_cast<Atomic>(formula.getExpr())))};
        ++traceIdx;
        return verdict;
    } else
        return {};
}

void AtomicMonitor::setTrace(const Trace &tr) { trace = tr; }
