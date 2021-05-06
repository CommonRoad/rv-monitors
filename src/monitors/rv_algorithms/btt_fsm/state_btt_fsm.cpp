#include "state_btt_fsm.h"
#include "decision.h"

#include <monitors/utils/utils.h>
#include <sstream>
#include <utility>

StateBttFsm::StateBttFsm(size_t id, std::shared_ptr<node> nonTerminal, std::shared_ptr<node> terminal)
    : id(id), nonTerminal(std::move(nonTerminal)), terminal(std::move(terminal)) {}

StateBttFsm::StateBttFsm(size_t id) : id(id) {}

size_t StateBttFsm::getId() const { return id; }

void StateBttFsm::setId(size_t id) { StateBttFsm::id = id; }

const std::shared_ptr<node> &StateBttFsm::getNonTerminal() const { return nonTerminal; }

void StateBttFsm::setNonTerminal(const std::shared_ptr<node> &nonTerminal) { StateBttFsm::nonTerminal = nonTerminal; }

const std::shared_ptr<node> &StateBttFsm::getTerminal() const { return terminal; }

void StateBttFsm::setTerminal(const std::shared_ptr<node> &terminal) { StateBttFsm::terminal = terminal; }

size_t StateBttFsm::nextState(Event &event) {
    std::shared_ptr<Decision> decision;
    // Terminal
    if (event.isTerminal()) {
        // It's always a Decision
        decision = std::dynamic_pointer_cast<Decision>(terminal);
    }
    // Non-terminal
    else {
        decision = std::dynamic_pointer_cast<Decision>(nonTerminal);
    }
    return nextState(event, decision);
}

size_t StateBttFsm::nextState(Event &event, const std::shared_ptr<Decision> &decision) {
    std::shared_ptr<node> next;
    // Predicate is true
    if (Utils::evaluateBooleanExpression(decision->getPredicate(), event.getEventMapRef())) {
        next = decision->getLeft();
    } else {
        next = decision->getRight();
    }
    // If it's not a state we have to go further down the BTT
    if (auto nextDecision = std::dynamic_pointer_cast<Decision>(next)) {
        return nextState(event, nextDecision);
    }
    // It's a state
    else {
        auto nextState = std::dynamic_pointer_cast<StateBttFsm>(next);
        return nextState->id;
    }
}

std::string StateBttFsm::to_string() const {
    std::stringstream res;
    if (!id) {
        res << "false" << std::endl;
    } else if (id == 1) {
        res << "true" << std::endl;
    } else {
        res << "Id: " << id << std::endl;
        // True and false don't have transitions
        res << "Non-terminal: " << nonTerminal->to_string() << std::endl;
        res << "Terminal: " << terminal->to_string() << std::endl;
    }

    return res.str();
}
