#include "btt_fsm.h"

#include <utility>

BttFsm::BttFsm(Formula formula, std::vector<StateBttFsm> states)
    : formula(std::move(formula)), states(std::move(states)) {}

const Formula &BttFsm::getFormula() const { return formula; }

void BttFsm::setFormula(const Formula &fm) { BttFsm::formula = fm; }

const std::vector<StateBttFsm> &BttFsm::getStates() const { return states; }

void BttFsm::setStates(const std::vector<StateBttFsm> &st) { BttFsm::states = st; }

std::string BttFsm::to_string() const {
    std::stringstream res;
    res << "BTT-FSM for " << formula.to_string() << std::endl;
    for (const auto &state : states) {
        res << state.to_string();
    }
    return res.str();
}
