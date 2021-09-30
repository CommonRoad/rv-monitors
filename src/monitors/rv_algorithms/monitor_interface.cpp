//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "monitor_interface.h"

#include <utility>

MonitorInterface::MonitorInterface(Formula formula) : formulas({std::move(formula)}) {}

MonitorInterface::MonitorInterface(std::vector<Formula> formulas) : formulas(std::move(formulas)) {}
