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
size_t executeHydraEval(Trace trace, const std::vector<Formula> &formulas,
                        const std::vector<std::vector<Verdict>> &expSolution);

size_t evalHydra(const std::vector<Formula> &formulas, const std::vector<std::vector<Verdict>> &expSolution,
                 Hydra &monitor, Trace &trac, const std::string &evaluation);
} // namespace hydraTestUtils
