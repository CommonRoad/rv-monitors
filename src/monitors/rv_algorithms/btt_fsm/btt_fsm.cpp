#include "btt_fsm.h"
#include "btt_fsm_utils_creation.h"
#include "monitors/expression_tree/atomic/constant.h"
#include "monitors/rv_algorithms/mt_fsm/mt_fsm.h"

BttFsm::BttFsm(const Formula &formula, bool pastEvaluation)
    : Monitor((formula)), states(formulaToBTT_FSM()), pastEvaluation(pastEvaluation) {}

const std::vector<StateBttFsm> &BttFsm::getStates() const { return states; }

std::string BttFsm::to_string() const {
    std::stringstream res;
    res << "BTT-FSM for " << formula.to_string() << std::endl;
    for (const auto &state : states) {
        res << state.to_string();
    }
    return res.str();
}

std::vector<Verdict> BttFsm::evaluate(Trace &trace) {
    // First two states are true and false
    auto currentState = states[2];

    if (pastEvaluation) {
        trace.setTerminalEvent(false);
    }

    for (size_t i = 0; i < trace.getLength(); ++i) {
        // If past then access in inverted order
        size_t eventIndex = pastEvaluation ? trace.getLength() - 1 - i : i;
        size_t nextState = currentState.nextState(trace.getEvent(eventIndex));
        if (nextState == 0u)
            return {Verdict{0, false}};
        if (nextState == 1) {
            return {Verdict{0, true}};
        }
        currentState = states[nextState];
    }

    return {Verdict{0, false}};
}

std::vector<StateBttFsm> BttFsm::formulaToBTT_FSM() {
    // Only a constant
    if (auto constant = std::dynamic_pointer_cast<Constant>(formula.getExpr())) {
        StateBttFsm singleState(static_cast<size_t>(constant->getValue()), {}, {});
        return {singleState};
    }
    // Get corresponding MT-FSM states
    auto mtFsmStates{MtFsm::formulaToMT_FSM(formula, pastEvaluation)};

    // States of BTT-FSM
    std::vector<StateBttFsm> bttStates;

    // Add states true and false
    for (const auto &mtFsmState : mtFsmStates) {
        size_t id = mtFsmState.getId();
        // States false (0) and true (1)
        if ((id == 0u) or id == 1) {
            bttStates.emplace_back(id);
            continue;
        }
        // Other state
        // Get precedences
        auto precedencesNonTerminal = mtFsmState.getNonTerminal().getAtomicAndPredicateWeight();
        auto precedencesTerminal = mtFsmState.getTerminal().getAtomicAndPredicateWeight();

        // Create BTT for terminal and non-terminal multitransition
        auto bttNonTerminal = BTT_FSMUtilsCreation::findNextNode(mtFsmState.getNonTerminal(), precedencesNonTerminal);
        auto bttTerminal = BTT_FSMUtilsCreation::findNextNode(mtFsmState.getTerminal(), precedencesTerminal);

        // Create new state and add it
        auto newState = StateBttFsm(id, bttNonTerminal, bttTerminal);
        bttStates.push_back(newState);
    }

    return bttStates;
}