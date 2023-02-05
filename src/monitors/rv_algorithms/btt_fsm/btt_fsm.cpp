#include "monitors/rv_algorithms/btt_fsm/btt_fsm.h"
#include "monitors/expression_tree/atomic/constant.h"
#include "monitors/formula/supported_formula_type.h"
#include "monitors/rv_algorithms/btt_fsm/btt_fsm_utils_creation.h"
#include "monitors/rv_algorithms/mt_fsm/mt_fsm.h"

BttFsm::BttFsm(const std::vector<Formula> &formulas, bool pastEvaluation)
    : MonitorInterface(formulas, AlgorithmType::btt_fsm), states(formulaToBTT_FSM()), pastEvaluation(pastEvaluation) {}

const std::vector<StateBttFsm> &BttFsm::getStates() const { return states; }

std::string BttFsm::to_string() const {
    std::stringstream res;
    res << "BTT-FSM for " << formulas.at(0).getId() << std::endl;
    for (const auto &state : states) {
        res << state.to_string();
    }
    return res.str();
}

std::map<std::string, std::vector<Verdict>> BttFsm::evaluate(Trace &trace) {
    // First two states are true and false
    auto currentState = states[2];
    std::map<std::string, std::vector<Verdict>> result;
    if (pastEvaluation) {
        trace.setTerminalEvent(false);
    }

    for (size_t i = 0; i < trace.getLength(); ++i) {
        // If past then access in inverted order
        size_t eventIndex = pastEvaluation ? trace.getLength() - 1 - i : i;
        size_t nextState = currentState.nextState(trace.getEvent(eventIndex));
        if (nextState == 0u) {
            result[formulas.at(0).getId()] = {Verdict{false}};
            return result;
        }
        if (nextState == 1) {
            result[formulas.at(0).getId()] = {Verdict{true}};
            return result;
        }
        currentState = states[nextState];
    }
    result[formulas.at(0).getId()] = {Verdict{false}};
    return result;
}

std::vector<StateBttFsm> BttFsm::formulaToBTT_FSM() {
    // Only a constant
    if (auto constant = std::dynamic_pointer_cast<Constant>(formulas.at(0).getExpr())) {
        StateBttFsm singleState(static_cast<size_t>(constant->getValue()), {}, {});
        return {singleState};
    }
    // Get corresponding MT-FSM states
    auto mtFsmStates{MtFsm::formulaToMT_FSM(formulas.at(0), pastEvaluation)};

    // States of BTT-FSM
    std::vector<StateBttFsm> bttStates;

    // Add states true and false
    for (const auto &mtFsmState : mtFsmStates) {
        size_t stateId = mtFsmState.getId();
        // States false (0) and true (1)
        if ((stateId == 0u) or stateId == 1) {
            bttStates.emplace_back(stateId);
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
        auto newState = StateBttFsm(stateId, bttNonTerminal, bttTerminal);
        bttStates.push_back(newState);
    }

    return bttStates;
}