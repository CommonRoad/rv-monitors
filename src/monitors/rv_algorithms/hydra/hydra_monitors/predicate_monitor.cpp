//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "predicate_monitor.h"
#include "monitors/utils/utils.h"

Verdict PredicateMonitor::step() {
    auto event{trace->getEvent(traceIdx)};
    auto verd{Verdict(event.getTimeStep(), event.getValue(*std::dynamic_pointer_cast<Atomic>(formula.getExpr())))};
    ++traceIdx;
    return verd;
}

void PredicateMonitor::setTrace(const Trace &tr) { trace = tr; }
