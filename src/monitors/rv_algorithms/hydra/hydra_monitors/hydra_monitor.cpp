//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "hydra_monitor.h"

#include <utility>
const Formula &HydraMonitor::getFormula() const { return formula; }

HydraMonitor::HydraMonitor(Formula formula) : formula(std::move(formula)) {}
