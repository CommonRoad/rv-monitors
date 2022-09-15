//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "atomic_monitor.h"

Verdict AtomicMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    if (traceIdx[parentId] < trace.getLength()) {
        const Atomic &atomic = *std::dynamic_pointer_cast<Atomic>(formula.getExpr());
        auto event{trace.getEvent(traceIdx[parentId])};

        auto iter{variableValues.find(atomic.to_string())};
        if (iter != variableValues.end())
            return {event.getTimeStep(), event.getValue(iter->second.getVal())};
        else
            return {event.getTimeStep(), event.getValue(atomic)};
    } else
        return {false};
}

void AtomicMonitor::setTrace(const Trace &tra) { trace = tra; }

void AtomicMonitor::setVariableValues(std::map<std::string, Value> values) { variableValues = values; }

void AtomicMonitor::reset(std::optional<size_t> stepNumber) {
    HydraMonitor::reset(stepNumber);
    variableValues.clear();
}

std::shared_ptr<HydraMonitor> AtomicMonitor::clone() { return std::make_shared<AtomicMonitor>(*this); }

std::string AtomicMonitor::to_string() { return formula.to_string(); }
