#include "state_mt_fsm.h"
#include "monitors/utils/utils.h"
#include <utility>

StateMT_FSM::StateMT_FSM(size_t id, MultiTransition nonTerminal, MultiTransition terminal, Formula formula)
    : id(id), nonTerminal(std::move(nonTerminal)), terminal(std::move(terminal)), formula(std::move(formula)) {}

StateMT_FSM::StateMT_FSM(size_t id, Formula formula) : id(id), formula(std::move(formula)) {}

size_t StateMT_FSM::getId() const { return id; }

void StateMT_FSM::setId(size_t index) { StateMT_FSM::id = index; }

MultiTransition StateMT_FSM::getNonTerminal() const { return nonTerminal; }

void StateMT_FSM::setNonTerminal(const MultiTransition &nonT) { StateMT_FSM::nonTerminal = nonT; }

const MultiTransition &StateMT_FSM::getTerminal() const { return terminal; }

void StateMT_FSM::setTerminal(const MultiTransition &ter) { StateMT_FSM::terminal = ter; }

const Formula &StateMT_FSM::getFormula() const { return formula; }

void StateMT_FSM::setFormula(const Formula &form) { StateMT_FSM::formula = form; }

std::string StateMT_FSM::to_string() const {
    std::stringstream ret;
    ret << "Id: " << std::endl << id << std::endl;
    ret << "Non-terminal transitions: " << std::endl << nonTerminal.to_string() << std::endl;
    ret << "Terminal transitions: " << std::endl << terminal.to_string() << std::endl;
    return ret.str();
}

size_t StateMT_FSM::nextState(Event &event) {
    std::map<Formula, size_t> transitions;
    // Terminal
    if (event.isTerminal()) {
        transitions = terminal.getTransitions();
    }

    // Non-terminal
    else {
        transitions = nonTerminal.getTransitions();
    }

    for (const auto &transition : transitions) {
        if (Utils::evaluateBooleanExpression(transition.first.getExpr(), event.getEventMapRef())) {
            return transition.second;
        }
    }
    throw std::runtime_error("StateMT_FSM::nextState() No next state could be found.");
}
