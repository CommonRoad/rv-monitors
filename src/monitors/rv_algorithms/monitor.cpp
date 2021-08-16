//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "monitor.h"

#include <utility>

const Formula &Monitor::getFormula() const { return formula; }

Monitor::Monitor(Formula formula) : formula(std::move(formula)) {}
