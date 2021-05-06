#pragma once

#include "btt_fsm.h"
#include "monitors/formula/formula.h"
#include "monitors/trace/trace.h"

/**
 * Functionality for monitoring a formula on a trace.
 */
namespace BTT_FSMEvaluation {
/**
 * Builds the BttFsm of a formula and then monitors the trace.
 * @param formula Formula.
 * @param trace Trace.
 * @param past Is true if formula is in PLTL.
 * @return True if formula is satisfied on trace, false otherwise.
 */
[[nodiscard]] bool evaluateBTT_FSM(const BttFsm &bttFsm, Trace &trace, bool past);
}; // namespace BTT_FSMEvaluation
