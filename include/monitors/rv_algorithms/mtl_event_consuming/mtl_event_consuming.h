#pragma once

#include "monitors/formula/formula.h"
#include "monitors/rv_algorithms/monitor_interface.h"
#include <monitors/trace/trace.h>

/**
 * Functions for monitoring a trace using the MTL event consuming algorithm.
 */
class MTLEventConsuming : public MonitorInterface {
  public:
    /** Constructor.
     *
     * @param formulas List of formulas to monitor.
     */
    MTLEventConsuming(const std::vector<Formula> &formulas);

    /**
     * Evaluation function for monitor algorithms.
     *
     * @param trace Trace which should be evaluated.
     * @param domains List of domains. Not used by MTLEventConsuming.
     * @param formulaIDs List of formula IDs which should be evaluated (not used for this monitor)
     * @return List of verdicts for each time step of the trace. If the length of the vector is 1,
     * the runtime verification algorithm can only provide a global verdict for the complete trace.
     */
    std::map<std::string, std::vector<Verdict>>
    evaluate(const std::shared_ptr<Trace> &trace, const std::vector<std::shared_ptr<DomainStorage>> &domains = {},
             const std::vector<std::string> &formulaIDs = {}) override;

    /**
     * to_string() function.
     * @return String representation of the BTT-FSM.
     */
    [[nodiscard]] std::string to_string() const override;
};
