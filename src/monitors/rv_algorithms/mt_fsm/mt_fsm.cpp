#include "mt_fsm.h"
#include "monitors/expression_tree/atomic/atomic.h"
#include "monitors/expression_tree/atomic/constant.h"
#include "monitors/expression_tree/binary/bool/equivalence.h"
#include "monitors/expression_tree/nary/bool/or.h"
#include "monitors/qmc/qmc.h"
#include "monitors/rewriting/basic_rewrite_visitor.h"
#include "monitors/rv_algorithms/ltl_event_consuming/event_consuming.h"
#include "monitors/trace/event_generator.h"

MtFsm::MtFsm(const std::vector<Formula> &formulas, bool pastEvaluation)
    : MonitorInterface((formulas)), states(formulaToMT_FSM(formulas.at(0), pastEvaluation)),
      pastEvaluation(pastEvaluation) {}

// TODO check if empty
std::string MtFsm::to_string() const {
    auto atomics{Utils::getAtomicsAndPredicates(formulas.at(0).getExpr())};
    std::stringstream res;
    res << "Formula: " << std::endl << formulas.at(0).getId() << std::endl;
    res << "Atomics:" << std::endl << "[" << *atomics.begin();
    for (auto it = std::next(atomics.begin()); it != atomics.end(); ++it) {

        res << ", " << *it;
    }
    res << "]" << std::endl;
    for (const auto &state : states) {
        res << "Id: " << std::endl << state.getId() << std::endl;
        res << "Non-terminal transitions: " << std::endl << state.getNonTerminal().to_string() << std::endl;
        res << "Terminal transitions: " << std::endl << state.getTerminal().to_string() << std::endl;
    }

    return res.str();
}

const std::vector<StateMT_FSM> &MtFsm::getStates() const { return states; }

std::map<std::string, std::vector<Verdict>> MtFsm::evaluate(Trace &trace) {
    // First two states are true and false
    auto currentState = states[2];
    std::map<std::string, std::vector<Verdict>> result;

    if (pastEvaluation)
        trace.setTerminalEvent(false);

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

// Fig 5
void MtFsm::formulaToMT_FSM(Formula &currentFormula, std::vector<StateMT_FSM> &currentStates, bool past) {
    MultiTransition mtTerminal({});
    MultiTransition mtNonTerminal({});
    auto eventConsuming = EventConsumingLTL(std::vector<Formula>{currentFormula});
    auto atomics = Utils::getAtomicsAndPredicates(currentFormula.getExpr(), {});
    auto numberOfAtomics = Utils::getNumberOfAtomicsAndPredicates(currentFormula.getExpr());
    // All possible events
    EventGenerator gen(numberOfAtomics);
    for (auto event = gen.getEventVector(); gen.hasNext(); event = gen.getNext()) {
        auto currentEvent = Utils::boolVectorToEvent(atomics, event, false);
        auto conjunction = Utils::eventToFormula(currentEvent); // 7

        auto traceTerminal = Utils::boolVectorToTrace(atomics, event, true);
        auto formulaEventConsumingTerminal = eventConsuming.eventConsumingFuture(traceTerminal);

        // Sanity check
        if (not std::dynamic_pointer_cast<Constant>(formulaEventConsumingTerminal.getExpr())) {
            throw std::runtime_error(
                "MT_FSMCreation::formulaToMT_FSM() EventConsuming did not return a Constant on terminal event");
        }
        // Is true or false
        size_t evaluation = static_cast<size_t>(formulaEventConsumingTerminal.getExpr()->isConstTrue());

        MultiTransition currentMT = MultiTransition({{conjunction, evaluation}});
        mtTerminal = merge(currentMT, mtTerminal);

        Formula formulaEventConsuming(nullptr);
        if (!past) {
            auto traceNonTerminal = Utils::boolVectorToTrace(atomics, event, false);
            formulaEventConsuming = eventConsuming.eventConsumingFuture(traceNonTerminal);
        } else {
            auto trace = Utils::boolVectorToTrace(atomics, event, false);
            formulaEventConsuming = eventConsuming.eventConsumingPast(trace);
        }
        // 10 Search for equivalent formula
        bool eqFormulaExists = false;

        for (size_t stateId = 0; stateId < currentStates.size(); ++stateId) {
            auto equivalence = Formula(std::make_shared<Equivalence>(formulaEventConsuming.getExpr(),
                                                                     currentStates[stateId].getFormula().getExpr()));
            // TODO: add shortcut: if string representation of formulaEventConsuming and currentStates[i] is equal then
            // the equivalence is valid (inverse obviously not)
            if (valid(equivalence)) {
                currentMT = MultiTransition({{conjunction, stateId}});
                mtNonTerminal = merge(currentMT, mtNonTerminal);
                eqFormulaExists = true;
                break;
            }
        }
        if (not eqFormulaExists) {

            // Check if this formula already exists in states since they should be unique
            bool found = false;
            for (const auto &state : currentStates) {
                if (state.getFormula() == formulaEventConsuming) {
                    currentMT = MultiTransition({{conjunction, state.getId()}});
                    found = true;
                    break;
                }
            }
            // Not found so add formula to states
            if (not found) {
                // Add next state
                StateMT_FSM newState(currentStates.size(), formulaEventConsuming);
                currentStates.push_back(newState);

                currentMT = MultiTransition({{conjunction, currentStates.size() - 1}});

                mtNonTerminal = merge(currentMT, mtNonTerminal);

                formulaToMT_FSM(formulaEventConsuming, currentStates, past);
            }
        }
    }
    auto &transitionsNT = mtNonTerminal.getTransitions();
    auto &transitionsT = mtTerminal.getTransitions();
    if (transitionsNT.size() == 1 and transitionsT.size() == 1) {

        auto formulaTrue = Formula(std::make_shared<Constant>(true));
        auto propositionNT = transitionsNT.find(formulaTrue);
        auto propositionT = transitionsT.find(formulaTrue);
        if (propositionNT != transitionsNT.end() and propositionT != transitionsT.end()) {

            // Replace formula in MT
            auto newFormula = propositionT->first;
            auto stateTmp = propositionT->second;
            transitionsNT.erase(formulaTrue);
            transitionsNT.emplace(newFormula, stateTmp);
        }
    }

    auto currentStateIter = std::find_if(currentStates.begin(), currentStates.end(),
                                         [&](const StateMT_FSM &s) { return s.getFormula() == currentFormula; });
    auto &currentState = currentStates[static_cast<size_t>(std::distance(currentStates.begin(), currentStateIter))];
    currentState.setTerminal(mtTerminal);
    currentState.setNonTerminal(mtNonTerminal);
}

// Wrapper function
std::vector<StateMT_FSM> MtFsm::formulaToMT_FSM(const Formula &ltlFormula, bool past) {
    auto formulaCopy = ltlFormula.deepCopy();
    // Only a constant
    if (auto constant = std::dynamic_pointer_cast<Constant>(ltlFormula.getExpr())) {
        StateMT_FSM singleState(static_cast<size_t>(constant->getValue()), ltlFormula);
        return {singleState};
    }
    auto formulaFalse = Formula(std::make_shared<Constant>(false));
    auto formulaTrue = Formula(std::make_shared<Constant>(true));
    StateMT_FSM stateFalse(0, formulaFalse);
    StateMT_FSM stateTrue(1, formulaTrue);
    StateMT_FSM stateFirst(2, formulaCopy);
    std::vector<StateMT_FSM> newStates{stateFalse, stateTrue, stateFirst};

    formulaToMT_FSM(formulaCopy, newStates, past);
    return newStates;
}

bool MtFsm::valid(Formula &formula) {
    std::set<Formula> statesToCheck{formula};
    return valid(formula, statesToCheck);
}

// Fig 6
bool MtFsm::valid(Formula &formulaToCheck, std::set<Formula> &statesToCheck) {
    auto atomics = Utils::getAtomicsAndPredicates(formulaToCheck.getExpr(), {});
    auto numberOfAtomics = Utils::getNumberOfAtomicsAndPredicates(formulaToCheck.getExpr());
    auto eventConsuming = EventConsumingLTL(std::vector<Formula>{formulaToCheck});

    // Only constants
    if (numberOfAtomics == 0) {
        if (auto constant = std::dynamic_pointer_cast<Constant>(formulaToCheck.getExpr())) {
            return constant->getValue();
        } else if (auto eq = std::dynamic_pointer_cast<Equivalence>(formulaToCheck.getExpr())) {
            if ((*(eq->getLeft()) == *(eq->getRight()))) {
                return true;
            }
            // TODO: better way: resolve the formula end return the result
            else if (formulaToCheck.to_string() == "(false <-> true)" or
                     formulaToCheck.to_string() == "(true <-> false)") {
                return false;
            }
        }
        // Should never be reached
        throw std::runtime_error("MT_FSMCreation.cpp::valid() : formula has no predicates and is not handled!");
    }

    EventGenerator gen(numberOfAtomics);

    for (auto event = gen.getEventVector(); gen.hasNext(); event = gen.getNext()) {
        // Terminal eventMap
        auto traceNonTerminal = Utils::boolVectorToTrace(atomics, event, true);
        if (eventConsuming.eventConsumingFuture(traceNonTerminal).getExpr()->isConstFalse()) {
            return false;
        }
        // Non-terminal eventMap
        auto traceTerminal = Utils::boolVectorToTrace(atomics, event, false);
        Formula f = eventConsuming.eventConsumingFuture(traceTerminal);

        auto [_, inserted] = statesToCheck.emplace(f);
        if (inserted and not valid(f, statesToCheck)) {
            return false;
        }
    }
    return true;
}

MultiTransition MtFsm::merge(MultiTransition &mt_1, MultiTransition mt_2) {
    BasicRewriteVisitor br;
    std::map<Formula, size_t> mergedTransitions;

    // Keeps track of every transition in mt_2 if it was already merged
    std::vector<bool> mt_2TransitionMerged(mt_2.getTransitions().size());

    // First check mt_1 -> mt_2
    for (auto &[formula1, state1] : mt_1.getTransitions()) {
        // Check if state1 is in mt_2
        bool stateFound = false;
        size_t mt_2Id = 0;
        for (auto &[formula2, state2] : mt_2.getTransitions()) {
            if (state1 == state2) {
                // Formula1 OR formula2
                Formula mergedFormula(std::make_shared<Or>(Or::SetType_T{formula1.getExpr(), formula2.getExpr()}));
                br.rewrite(mergedFormula);
                mergedTransitions.emplace(QMC::reduceBoolFormula(mergedFormula), state1);
                stateFound = true;
                mt_2TransitionMerged[mt_2Id] = true;
                break;
            }
            ++mt_2Id;
        }
        if (not stateFound) {
            auto formulaNonConst = Formula(formula1);
            br.rewrite(formulaNonConst);
            mergedTransitions.emplace(QMC::reduceBoolFormula(formulaNonConst), state1);
        }
    }

    // Then check mt_2 -> mt_1. There will be no new merged formulae so we only need to insert new ones.
    size_t mt_2Id = 0;
    for (auto &[formula2, state2] : mt_2.getTransitions()) {
        // check if state2 is in mt_1
        if (not mt_2TransitionMerged[mt_2Id]) {
            auto formulaNonConst = Formula(formula2);
            br.rewrite(formulaNonConst);
            mergedTransitions.emplace(QMC::reduceBoolFormula(formulaNonConst), state2);
        }
        ++mt_2Id;
    }

    return MultiTransition(mergedTransitions);
}
