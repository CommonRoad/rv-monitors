#pragma once

#include "monitors/formula/formula.h"
#include "state_btt_fsm.h"
#include <set>
#include <vector>

/**
 * Class representing a Binary Transition Tree Finite State Machine (BTT-FSM).
 */
class BttFsm {
  private:
    Formula formula;
    std::vector<StateBttFsm> states;

  public:
    /**
     * Constructor for a BTT-FSM.
     * @param formula Forumla that we want the BTT-FSM for.
     * @param states Vector of StateBttFsm.
     */
    BttFsm(Formula formula, std::vector<StateBttFsm> states);

    /**
     * Returns the formula the BTT-FSM was constrcuted for.
     * @return Formula.
     */
    [[nodiscard]] const Formula &getFormula() const;

    /**
     * Returns a vector of all states in the BTT-FSM.
     * @return Vector of StateBttFsm.
     */
    [[nodiscard]] const std::vector<StateBttFsm> &getStates() const;

    /**
     * Setter for formula.
     * @param formula
     */
    void setFormula(const Formula &formula);

    /**
     * Setter for state vector.
     * @param states
     */
    void setStates(const std::vector<StateBttFsm> &states);

    /**
     * to_string() function.
     * @return String representation of the BTT-FSM.
     */
    [[nodiscard]] std::string to_string() const;
};
