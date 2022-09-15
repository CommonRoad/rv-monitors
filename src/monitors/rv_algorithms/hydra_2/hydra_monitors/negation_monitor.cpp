//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "negation_monitor.h"

Verdict NegationMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    return !subMonitor->step(monitorId, stepNumber);
}

void NegationMonitor::setTrace(const Trace &tra) { subMonitor->setTrace(tra); }

void NegationMonitor::setVariableValues(std::map<std::string, Value> values) {
    subMonitor->setVariableValues(values);
}

void NegationMonitor::reset(std::optional<size_t> stepNumber) {
    HydraMonitor::reset(stepNumber);
    subMonitor->reset(stepNumber);
}

std::shared_ptr<HydraMonitor> NegationMonitor::clone() {
    auto clonedMonitor = std::make_shared<NegationMonitor>(*this);
    clonedMonitor->subMonitor = subMonitor->clone();
    return clonedMonitor;
}

std::string NegationMonitor::to_string() { return "!(" + subMonitor->to_string() + ")"; }
