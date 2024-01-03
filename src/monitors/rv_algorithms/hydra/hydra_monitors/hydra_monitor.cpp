//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "monitors/rv_algorithms/hydra/hydra_monitors/hydra_monitor.h"

#include <utility>

void HydraMonitor::setFormulaId(const std::string &formula_id) { formulaId = formula_id; }

HydraMonitor::HydraMonitor(Formula formula) : formula(std::move(formula)) {}

const std::string &HydraMonitor::getFormulaId() const { return formulaId; }