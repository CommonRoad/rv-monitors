//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "atomic_monitor.h"
#include "monitors/expression_tree/atomic/atomic.h"

Verdict AtomicMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    if (stepNumber < trace.getLength()) {
        auto event{trace.getBooleanEvent(stepNumber)};
        auto verdict{
            Verdict(event.getTimeStep(), event.getValue(*std::dynamic_pointer_cast<Atomic>(formula.getExpr())))};
        return verdict;
    } else
        return {false};
}

void AtomicMonitor::setTrace(const Trace &tra) { trace = tra; }

std::string AtomicMonitor::to_string() { return formula.to_string(); }

