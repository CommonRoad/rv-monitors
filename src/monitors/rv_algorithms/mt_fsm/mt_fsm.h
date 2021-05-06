#pragma once

#include "monitors/formula/formula.h"
#include "monitors/trace/trace.h"
#include "state_mt_fsm.h"
#include <memory>
#include <utility>
#include <vector>

/**
 * Class to manage an MT-FSM*.
 */
class MtFsm {
  private:
    std::set<std::string> atomics;
    Formula formula;
    std::vector<StateMT_FSM> states;

  public:
    /**
     * Constructor.
     * @param atomics Predicates.
     * @param formula Formula.
     * @param states Vector of states.
     */
    MtFsm(std::set<std::string> &atomics, Formula &formula, std::vector<StateMT_FSM> &states);

    /**
     * Getter for atomics.
     * @return atomics.
     */
    [[nodiscard]] const std::set<std::string> &getAtomics() const;

    /**
     * Setter for atomics.
     * @param atomics
     */
    void setAtomics(const std::set<std::string> &atomics);

    /**
     * Getter for formula.
     * @return formula.
     */
    [[nodiscard]] const Formula &getFormula() const;

    /**
     * Setter for formula.
     * @param formula
     */
    void setFormula(const Formula &formula);

    /**
     * Getter for states.
     * @return states.
     */
    [[nodiscard]] const std::vector<StateMT_FSM> &getStates() const;

    /**
     * Setter for states.
     * @param states
     */
    void setStates(const std::vector<StateMT_FSM> &states);

    /**
     * String representation of the MT-FSM*.
     * @return String representation of the MT-FSM*.
     */
    [[nodiscard]] std::string to_string() const;
};
