#pragma once

#include "monitors/utils/utils.h"
#include "quantifier_monitor.h"

/**
 * Hydra2 monitor representing universal monitor.
 */
class UniversalMonitor : public QuantifierMonitor {
  public:
    /**
     * Constructor.
     *
     * @param formula Formula to be monitored.
     * @param subMonitor Child monitor.
     * @param monitorId ID of monitor.
     * @param variables Variables used by quantifier.
     * @param domains Collection of domains.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     * @param shortCircuiting Boolean indicating whether short circuiting should be used.
     */
    UniversalMonitor(Formula formula, const std::shared_ptr<Hydra2Monitor> &subMonitor, size_t monitorId,
                     std::map<std::string, std::shared_ptr<DomainStorage>> variableValues, bool connectedEval,
                     bool shortCircuiting);

    /**
     * Performs concrete step of universal monitor. The sub-verdict for each combination has to be true
     * so that the universal monitor returns also a true verdict for the current step number.
     *
     * @param parentId ID of parent monitor.
     * @param stepNumber Current step of monitor.
     * @param numberCombinations Number of combinations of variables.
     * @return Verdict for monitoring step result.
     */
    Verdict quantifierStep(size_t parentId, size_t stepNumber, size_t numberCombinations) override;

    /**
     * Clones this monitor.
     *
     * @param domValNames List of domain value names.
     * @return New shared_ptr of this monitor.
     */
    std::shared_ptr<Hydra2Monitor> clone(const std::vector<std::string> &domValNames) override;

    /**
     * Creates string representation of monitor.
     *
     * @return String representing monitor.
     */
    std::string to_string() override;

  private:
    bool storeVariableResults{true}; //**< Boolean indicating whether results should be stored. */
};
