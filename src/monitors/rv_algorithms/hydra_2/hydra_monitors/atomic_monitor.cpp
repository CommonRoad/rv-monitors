//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/atomic_monitor.h"

Verdict AtomicMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    if (stepNumber < trace.getLength()) {
        if (!domainValue.empty())
            return trace.verdictAtTimeStep(stepNumber, domainValue);
        else
            return trace.verdictAtTimeStep(stepNumber, name);
    } else
        return {false};
}

void AtomicMonitor::setTrace(const Trace &tra) { trace = tra; }

std::string AtomicMonitor::to_string() { return formula.to_string(); }

void AtomicMonitor::addDomainValues(const std::vector<DomainValue> &vals) {
    for (const auto &dom : vals)
        if (dom.getVariable() == name)
            domainValue = std::any_cast<const char *>(dom.getValue());
}

std::shared_ptr<Hydra2Monitor> AtomicMonitor::clone() { return std::make_shared<AtomicMonitor>(*this); }