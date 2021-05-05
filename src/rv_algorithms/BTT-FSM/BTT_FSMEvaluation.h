#pragma once

#include "BTT_FSM.h"
#include "formula/formula.h"
#include "trace/trace.h"

/**
 * Functionality for monitoring a formula on a trace.
 */
namespace BTT_FSMEvaluation {
/**
 * Builds the BTT_FSM of a formula and then monitors the trace.
 * @param formula Formula.
 * @param trace Trace.
 * @param past Is true if formula is in PLTL.
 * @return True if formula is satisfied on trace, false otherwise.
 */
[[nodiscard]] bool evaluateBTT_FSM(const BTT_FSM &bttFsm, Trace &trace, bool past);
}; // namespace BTT_FSMEvaluation
