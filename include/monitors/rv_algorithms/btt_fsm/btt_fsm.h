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
    std::vector<StateBttFsm> states; //**< States of BTT-FSM. */
    bool pastEvaluation;             //**< Boolean indicating whether past-based evaluation should be applied. */

  public:
    /**
     * Constructor for a BTT-FSM.
     *
     * @param formula Formula that we want the BTT-FSM for.
     * @param pastEvaluation Boolean indicating whether past evaluation has to be applied.
     */
    BttFsm(const std::vector<Formula> &formulas, bool pastEvaluation = false);

    /**
     * Getter for BTT-FSM states.
     *
     * Returns a vector of all states in the BTT-FSM.
     * @return Vector of StateBttFsm.
     */
    [[nodiscard]] const std::vector<StateBttFsm> &getStates() const;

    /**
     * to_string() function.
     *
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
     * @param domains List of domains. Not used by BTT-FSM.
     * @param formulaIDs List of formula IDs which should be evaluated (not used for this monitor)
     * @return List of verdicts for each time step of the trace. If the length of the vector is 1,
     * the runtime verification algorithm can only provide a global verdict for the complete trace.
     */
    std::map<std::string, std::vector<Verdict>>
    evaluate(const std::shared_ptr<Trace> &trace, const std::vector<std::shared_ptr<DomainStorage>> &domains = {},
             const std::vector<std::string> &formulaIDs = {}) override;
};
