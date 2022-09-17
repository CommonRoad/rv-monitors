#pragma once

#include "monitors/formula/formula.h"
#include "monitors/rv_algorithms/monitor_interface.h"
#include "monitors/trace/trace.h"
#include "state_mt_fsm.h"
#include <memory>
#include <utility>
#include <vector>

/**
 * Class to manage an MT-FSM*.
 */
class MtFsm : public MonitorInterface {
  private:
    std::vector<StateMT_FSM> states; //**< States of MT-FSM. */
    bool pastEvaluation;             //**< Boolean indicating whether past-based evaluation should be applied. */

  public:
    /**
     * Constructor for a MT-FSM.
     * @param formula Formula that we want the BTT-FSM for.
     * @param pastEvaluation Boolean indicating whether past-based evaluation should be applied.
     */
    MtFsm(const std::vector<Formula> &formulas);

    /**
     * Getter for states.
     * @return states.
     */
    [[nodiscard]] const std::vector<StateMT_FSM> &getStates() const;

    /**
     * String representation of the MT-FSM*.
     * @return String representation of the MT-FSM*.
     */
    [[nodiscard]] std::string to_string() const override;

    /**
     * Checks if a trace satisfies a specification by traversing an MT-FSM*.
     * @param mtFsm
     * @param trace
     * @param past True if formula is in PLTLf.
     * @return True if satisfied, false otherwise.
     */
    [[nodiscard]] std::map<std::string, std::vector<Verdict>> evaluate(Trace &trace) override;

    /**
     * Helper for formulaToMT_FSM().
     * @param formula
     * @param states
     * @param past True if formula is in PLTLf.
     */
    static void formulaToMT_FSM(Formula &formula, std::vector<StateMT_FSM> &states, bool past);

    /**
     * Constructs an MT-FSM* from a given formula.
     * @param formula
     * @param past True if formula is in PLTLf.
     * @return MT-FSM*.
     */
    static std::vector<StateMT_FSM> formulaToMT_FSM(const Formula &ltlFormula, bool past);

    /**
     * Checks for a given formula and a set of states if formula returns true for every form it can evolve into.
     * @param formula
     * @return
     */
    [[nodiscard]] static bool valid(Formula &formula);

    /**
     * Helper for recursion with additional argument.
     * @param formula
     * @param states
     * @return True if valid, false otherwise.
     */
    [[nodiscard]] static bool valid(Formula &formula, std::set<Formula> &states);

    /**
     * Merges two multi-transistions if possible.
     * @param mt_1 First multi-transistion.
     * @param mt_2 Second multi_transition.
     * @return New merged MT.
     */
    [[nodiscard]] static MultiTransition merge(MultiTransition &mt_1, MultiTransition mt_2);
};