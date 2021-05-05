#include "mt_fsm.h"

MtFsm::MtFsm(std::set<std::string> &atomics, Formula &formula, std::vector<StateMT_FSM> &states)
    : atomics(std::move(atomics)), formula(std::move(formula)), states(std::move(states)) {}

// TODO check if empty
std::string MtFsm::to_string() const {
    std::stringstream res;
    res << "Formula: " << std::endl << formula.to_string() << std::endl;
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

const std::set<std::string> &MtFsm::getAtomics() const { return atomics; }

void MtFsm::setAtomics(const std::set<std::string> &atomics) { MtFsm::atomics = atomics; }

const Formula &MtFsm::getFormula() const { return formula; }

void MtFsm::setFormula(const Formula &formula) { MtFsm::formula = formula; }

const std::vector<StateMT_FSM> &MtFsm::getStates() const { return states; }

void MtFsm::setStates(const std::vector<StateMT_FSM> &states) { MtFsm::states = states; }
