//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "monitor_interface.h"

#include <utility>

const Formula &MonitorInterface::getFormula() const { return formula; }

MonitorInterface::MonitorInterface(Formula formula) : formula(std::move(formula)) {}
