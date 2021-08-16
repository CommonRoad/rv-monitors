//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#pragma once

#include "../../utils/utils.h"
#include "monitors/formula/formula.h"
#include <monitors/trace/trace.h>

/**
 * Functions for monitoring a trace using the Hydra MTL monitor.
 */
namespace Hydra {
/**
 * Monitors a formula in MTL or LTL on a trace. The formula can have future and past operators.
 * @param formula Formula.
 * @param trace Trace.
 * @return Boolean value for each time step.
 */
[[nodiscard]] std::vector<BooleanVerdict> evaluate(const Formula &formula, Trace &trace);

}; // namespace Hydra
