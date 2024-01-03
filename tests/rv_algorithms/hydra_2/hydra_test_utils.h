//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2022 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//

#pragma once

#include "monitors/formula/formula.h"
#include "monitors/rv_algorithms/hydra_2/hydra.h"
#include "monitors/utils/utils.h"

namespace hydraTestUtils {
Trace createTraceWithVariable(size_t length, std::shared_ptr<IFunction<bool>> function);

size_t executeHydraEval(Trace trace, const std::vector<Formula> &formulas,
                        const std::vector<std::vector<Verdict>> &expSolution,
                        const std::vector<std::shared_ptr<DomainStorage>> &domains = {});

size_t evalHydra(const std::vector<Formula> &formulas, const std::vector<std::vector<Verdict>> &expSolution,
                 Hydra2 &monitor, Trace &trac, const std::string &evaluation);

bool higher_than_time_step(size_t timeStep, size_t val);

std::tuple<size_t, size_t> twoSizeTArgs(std::vector<DomainValue> values);

bool higher_than_zero(size_t timeStep, size_t val);

bool equal_to_time_step(size_t timeStep, size_t value);

bool divisible_by_time_step(size_t timeStep, size_t val);

bool lower_than_time_step(size_t timeStep, size_t val);

} // namespace hydraTestUtils
