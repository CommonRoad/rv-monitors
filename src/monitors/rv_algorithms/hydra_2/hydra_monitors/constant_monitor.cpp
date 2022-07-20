//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "constant_monitor.h"
#include <monitors/expression_tree/atomic/constant.h>

Verdict ConstantMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    if (traceIdx[parentId] < trace.getLength()) {
        auto event{trace.getEvent(traceIdx[parentId])};
        auto verdict{
            Verdict(event.getTimeStep(), (*std::dynamic_pointer_cast<Constant>(formula.getExpr())).getValue())};
        return verdict;
    } else
        return {false};
}

void ConstantMonitor::setTrace(const Trace &tra) { trace = tra; }

std::string ConstantMonitor::to_string() { return formula.getExpr()->getSymbol(); }
