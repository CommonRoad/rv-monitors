//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "negation_monitor.h"

Verdict NegationMonitor::step() { return !subMonitor->step(); }

void NegationMonitor::setTrace(const Trace &tr) { subMonitor->setTrace(tr); }
