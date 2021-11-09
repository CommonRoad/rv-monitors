//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "negation_monitor.h"

Verdict NegationMonitor::concreteStep(size_t parentId) { return !subMonitor->step(monitorId); }

void NegationMonitor::setTrace(const Trace &tr) { subMonitor->setTrace(tr); }

void NegationMonitor::reset() {
    HydraMonitor::reset();
    subMonitor->reset();
}
std::string NegationMonitor::to_string() { return std::string(); }
