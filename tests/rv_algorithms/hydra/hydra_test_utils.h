#pragma once

#include "monitors/formula/formula.h"
#include "monitors/rv_algorithms/hydra/hydra.h"
#include "monitors/utils/utils.h"

namespace hydraTestUtils {
size_t executeHydraEval(const std::vector<Formula> &formulas, const std::vector<std::vector<Verdict>> &expSolution,
                        Trace &trac);
bool evalHydra(const std::vector<Formula> &formulas, const std::vector<std::vector<Verdict>> &expSolution,
               Hydra &monitor, Trace &trace);
} // namespace hydraTestUtils
