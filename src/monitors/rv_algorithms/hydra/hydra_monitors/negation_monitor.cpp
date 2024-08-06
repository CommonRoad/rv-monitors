//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "monitors/rv_algorithms/hydra/hydra_monitors/negation_monitor.h"

Verdict NegationMonitorHydra::concreteStep() { return !subMonitor->concreteStep(); }

void NegationMonitorHydra::setTrace(const std::shared_ptr<Trace> &tra) { subMonitor->setTrace(tra); }

void NegationMonitorHydra::reset() { subMonitor->reset(); }

std::string NegationMonitorHydra::to_string() { return "!(" + subMonitor->to_string() + ")"; }
