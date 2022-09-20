//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "negation_monitor.h"

Verdict NegationMonitorHydra::concreteStep() { return !subMonitor->concreteStep(); }

void NegationMonitorHydra::setTrace(const Trace &tra) { subMonitor->setTrace(tra); }

void NegationMonitorHydra::reset() { subMonitor->reset(); }

std::string NegationMonitorHydra::to_string() { return "!(" + subMonitor->to_string() + ")"; }
