//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "monitors/utils/utils.h"
#include "predicate_monitor.h"

Verdict PredicateMonitor::step() {
    auto verd{Verdict(0, trace->getEvent(traceIdx).getValue(*std::dynamic_pointer_cast<Atomic>(formula.getExpr())))};
    ++traceIdx;
    return verd;
}

void PredicateMonitor::setTrace(const Trace &tr) {
    trace = tr;
}
