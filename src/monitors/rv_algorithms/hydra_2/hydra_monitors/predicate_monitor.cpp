//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "predicate_monitor.h"
#include <iostream>

Verdict PredicateMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    if (stepNumber < trace.getLength()) {
        auto event{trace.getEvent(stepNumber)};
        auto verdict{
            Verdict(event.getTimeStep(), event.getValue(*std::dynamic_pointer_cast<Predicate>(formula.getExpr())))};
        return verdict;
    } else
        return {false};
}

void PredicateMonitor::setTrace(const Trace &tra) { trace = tra; }

std::string PredicateMonitor::to_string() { return formula.to_string(); }
