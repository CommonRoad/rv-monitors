#pragma once

#include "mt_fsm.h"
#include "formula/formula.h"
#include "trace/trace.h"

namespace MT_FSMEvaluation {
/**
 * Checks if a trace satisfies a specification by traversing an MT-FSM*.
 * @param mtFsm
 * @param trace
 * @param past True if formula is in PLTLf.
 * @return True if satisfied, false otherwise.
 */
[[nodiscard]] bool evaluateMT_FSM(const MtFsm &mtFsm, Trace &trace, bool past);
} // namespace MT_FSMEvaluation
