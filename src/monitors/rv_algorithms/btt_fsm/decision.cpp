#include "monitors/rv_algorithms/btt_fsm/decision.h"
#include "monitors/rv_algorithms/btt_fsm/state_btt_fsm.h"

#include <sstream>
#include <utility>

Decision::Decision(std::shared_ptr<node> left, std::shared_ptr<node> right, std::shared_ptr<Expression> predicate)
    : left(std::move(left)), right(std::move(right)), predicate(std::move(predicate)) {}

const std::shared_ptr<node> &Decision::getLeft() const { return left; }

void Decision::setLeft(const std::shared_ptr<node> &le) { Decision::left = le; }

const std::shared_ptr<node> &Decision::getRight() const { return right; }

void Decision::setRight(const std::shared_ptr<node> &ri) { Decision::right = ri; }

const std::shared_ptr<Expression> &Decision::getPredicate() const { return predicate; }

void Decision::setPredicate(const std::shared_ptr<Expression> &pred) { Decision::predicate = pred; }

std::string Decision::to_string() const {
    std::stringstream res;
    res << "(" << predicate->to_string() << " ? ";
    if (auto state = std::dynamic_pointer_cast<StateBttFsm>(left)) {
        if (state->getId() == 0u) {
            res << "false : ";
        } else if (state->getId() == 1) {
            res << "true : ";
        } else {
            res << state->getId() << " : ";
        }
    } else {
        res << left->to_string() << " : ";
    }

    if (auto state = std::dynamic_pointer_cast<StateBttFsm>(right)) {
        if (state->getId() == 0u) {
            res << "false";
        } else if (state->getId() == 1) {
            res << "true";
        } else {
            res << state->getId();
        }
    } else {
        res << right->to_string();
    }
    res << ")";
    return res.str();
}
