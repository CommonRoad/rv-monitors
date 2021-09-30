#pragma once

#include "monitors/formula/formula.h"
#include "monitors/rv_algorithms/monitor_interface.h"
#include "monitors/trace/trace.h"

/**
 * Functions for monitoring a trace using the event consuming algorithm.
 */
class EventConsumingLTL : public MonitorInterface {
  private:
    bool pastEvaluation;

  public:
    EventConsumingLTL(const std::vector<Formula> &formulas, bool pastEvaluation = false);
    /**
     * Monitors a formula in LTLf on a trace.
     * @param formula Formula.
     * @param trace Trace.
     * @return Formula changed by the event consuming algorithm, should be true or false at the end.
     */
    [[nodiscard]] Formula eventConsumingFuture(Trace &trace);

    /**
     * Monitors a formula in pLTL on a trace.
     * @param formula Formula.
     * @param trace Trace.
     * @return Formula changed by the event consuming algorithm, should be true or false at the end.
     */
    [[nodiscard]] Formula eventConsumingPast(Trace &trace);

    /**
     * Evaluation function for monitor algorithms.
     *
     * @param trace Trace which should be evaluated.
     * @return List of verdicts for each time step of the trace. If the length of the vector is 1,
     * the runtime verification algorithm can only provide a global verdict for the complete trace.
     */
    std::map<std::string, std::vector<Verdict>> evaluate(Trace &trace) override;

    /**
     * to_string() function.
     * @return String representation of the BTT-FSM.
     */
    [[nodiscard]] std::string to_string() const override;
};
