#pragma once

#include "../monitor_interface.h"
#include "monitors/formula/formula.h"
#include "state_btt_fsm.h"
#include <set>
#include <vector>

/**
 * Class representing a Binary Transition Tree Finite State Machine (BTT-FSM).
 */
class BttFsm : public MonitorInterface {
  private:
    std::vector<StateBttFsm> states;
    bool pastEvaluation;

  public:
    /**
     * Constructor for a BTT-FSM.
     * @param formula Formula that we want the BTT-FSM for.
     * @param pastEvaluation Boolean indicating whether past-based evaluation should be applied.
     */
    BttFsm(const Formula &formula, bool pastEvaluation = false);

    /**
     * Returns a vector of all states in the BTT-FSM.
     * @return Vector of StateBttFsm.
     */
    [[nodiscard]] const std::vector<StateBttFsm> &getStates() const;

    /**
     * to_string() function.
     * @return String representation of the BTT-FSM.
     */
    [[nodiscard]] std::string to_string() const override;

    /**
     * Creates a BTT-FSM from a given formula.
     *
     * @param formula Formula.
     * @return BTT-FSM of formula.
     */
    std::vector<StateBttFsm> formulaToBTT_FSM();

    /**
     * Evaluation function for monitor algorithms.
     *
     * @param trace Trace which should be evaluated.
     * @return List of verdicts for each time step of the trace. If the length of the vector is 1,
     * the runtime verification algorithm can only provide a global verdict for the complete trace.
     */
    std::vector<Verdict> evaluate(Trace &trace) override;
};
