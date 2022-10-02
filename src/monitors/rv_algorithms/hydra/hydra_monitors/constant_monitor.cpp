//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "constant_monitor.h"
#include <monitors/expression_tree/atomic/constant.h>

Verdict ConstantMonitorHydra::concreteStep() {
    if (traceIdx < trace.value().getLength()) {
        auto event{trace.value().getBooleanEvent(traceIdx)};
        auto verdict{
            Verdict(event.getTimeStep(), (*std::dynamic_pointer_cast<Constant>(formula.getExpr())).getValue())};
        ++traceIdx;
        return verdict;
    } else
        return {false};
}

void ConstantMonitorHydra::setTrace(const Trace &tra) { trace = tra; }

void ConstantMonitorHydra::reset() { traceIdx = 0; }

std::string ConstantMonitorHydra::to_string() { return formula.getExpr()->getSymbol(); }