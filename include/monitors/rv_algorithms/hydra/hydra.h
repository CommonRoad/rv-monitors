#pragma once

#include "../../utils/utils.h"
#include "hydra_monitors/hydra_monitor.h"
#include "monitors/formula/formula.h"
#include "monitors/rv_algorithms/monitor_interface.h"
#include <monitors/trace/trace.h>

/**
 * Class for monitoring a trace using the Hydra MTL monitor.
 */
class Hydra : public MonitorInterface {
  public:
    /**
     * Constructor
     * @param formulas List of formulas which should be monitored.
     */
    Hydra(const std::vector<Formula> &formulas);

    /**
     * to_string() function.
     * @return String representation of Hydra.
     */
    [[nodiscard]] std::string to_string() const override;

    /**
     * Evaluation function for monitor algorithms.
     *
     * @param trace Trace which should be evaluated.
     * @param domains List of domains. Not used by Hydra.
     * @param formulaIDs List of formula IDs which should be evaluated (not used for this monitor)
     * @return List of verdicts for each time step of the trace. If the length of the vector is 1,
     * the runtime verification algorithm can only provide a global verdict for the complete trace.
     */
    std::map<std::string, std::vector<Verdict>>
    evaluate(const std::shared_ptr<Trace> &trace, const std::vector<std::shared_ptr<DomainStorage>> &domains = {},
             const std::vector<std::string> &formulaIDs = {}) override;

  private:
    std::shared_ptr<HydraMonitor> monitor; //**< Monitor corresponding to the formulas which should be evaluated. */

    /**
     * Creates monitor for specified formula.
     *
     * @param formula Formula which should be monitored.
     */
    void createMonitor(const Formula &formula);
};
