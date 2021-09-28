//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "constant_monitor.h"
#include <monitors/expression_tree/atomic/constant.h>

Verdict ConstantMonitor::step() {
    auto event{trace.value().getEvent(traceIdx)};
    auto verdict{
        Verdict(event.getTimeStep(), (*std::dynamic_pointer_cast<Constant>(formula.getExpr())).getValue())};
    ++traceIdx;
    return verdict;
}

void ConstantMonitor::setTrace(const Trace &tr) { trace = tr; }
