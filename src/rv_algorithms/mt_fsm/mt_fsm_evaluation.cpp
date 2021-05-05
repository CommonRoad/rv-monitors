#include "mt_fsm_evaluation.h"
#include "mt_fsm.h"
#include "expression_tree/atomic/constant.h"
#include <iostream>

bool MT_FSMEvaluation::evaluateMT_FSM(const MtFsm &mtFsm, Trace &trace, bool past) {
    // Nothing to do here
    //    if (auto constant = std::dynamic_pointer_cast<Constant>(formula.getExpr())) {
    //        return constant->getValue();
    //    }
    //    auto formulaCopy = formula.deepCopy();
    //    MtFsm mtFsm = MT_FSMCreation::formulaToMT_FSM(formulaCopy, past);
    const auto &states = mtFsm.getStates();

    // First two states are true and false
    auto currentState = states[2];

    if (past) {
        trace.setTerminalEvent(false);
    }

    for (size_t i = 0; i < trace.getLength(); ++i) {
        // If past then access in inverted order
        size_t eventIndex = past ? trace.getLength() - 1 - i : i;
        size_t nextState = currentState.nextState(trace.getEvent(eventIndex));
        if (!nextState) {
            // std::cout << "Formula violated in event " << eventIndex << std::endl;
            return false;
        }
        if (nextState == 1) {
            // std::cout << "Formula satisfied in event " << eventIndex << std::endl;
            return true;
        }
        currentState = states[nextState];
    }
    return false;
}
