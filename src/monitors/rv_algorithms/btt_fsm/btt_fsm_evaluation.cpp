#include "btt_fsm_evaluation.h"
#include "monitors/expression_tree/atomic/constant.h"
#include <iostream>

bool BTT_FSMEvaluation::evaluateBTT_FSM(const BttFsm &bttFsm, Trace &trace, bool past) {
    // Nothing to do here
    //    if (auto constant = std::dynamic_pointer_cast<Constant>(formula.getExpr())) {
    //        return constant->getValue();
    //    }
    //    auto formulaCopy = formula.deepCopy();
    //    BttFsm bttFsm = BTT_FSMCreation::formulaToBTT_FSM(formulaCopy, past);
    // std::cout << bttFsm.to_string();
    const auto &states = bttFsm.getStates();

    // First two states are true and false
    auto currentState = states[2];

    if (past) {
        trace.setTerminalEvent(false);
    }

    for (size_t i = 0; i < trace.getLength(); ++i) {
        // If past then access in inverted order
        size_t eventIndex = past ? trace.getLength() - 1 - i : i;
        size_t nextState = currentState.nextState(trace.getEvent(eventIndex));
        if (nextState == 0u) {
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
