#pragma once

#include "hydra_monitor.h"

/**
 * Monitor representing negation Hydra2 operator.
 */
class NegationMonitor final : public Hydra2Monitor {
  public:
    /**
     * Constructor
     *
     * @param formula Formula to be monitored.
     * @param subMonitor Child monitor.
     * @param monitorId ID of monitor.
     */
    NegationMonitor(Formula formula, std::shared_ptr<Hydra2Monitor> subMonitor, const size_t monitorId)
        : Hydra2Monitor(std::move(formula), monitorId, subMonitor->getEvalCost(),
                        1 - subMonitor->getSatisfactionProbability()),
          subMonitor(std::move(subMonitor)) {}

    /**
     * Performs concrete step of negation operator monitor.
     *
     * @param parentId ID of parent monitor.
     * @param stepNumber Current step/position in trace.
     * @return Verdict for monitoring step result.
     */
    Verdict concreteStep(size_t parentId, size_t stepNumber) override;

    /**
     * Setter for trace.
     *
     * @param tra Trace.
     * @param domains List of domains.
     */
    void setTrace(const std::shared_ptr<Trace> &tra,
                  const std::vector<std::shared_ptr<DomainStorage>> &domains = {}) override;

    /**
     * Resets monitor variables to default values.
     */
    void reset() override;

    /**
     * Calls sub-monitor to add domain value.
     *
     * @param vals Domain values which should be added.
     */
    void addDomainValues(const std::vector<DomainValue> &vals) override;

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

    /**
     * Sets step number of monitor and sub-monitors.
     *
     * @param stepNumber Desired step number.
     * @param parentId ID of parent monitor.
     */
    void setStepNumber(size_t stepNumber, size_t parentId) override;

    /**
     * Extracts tree structure for visualization.
     *
     * @param tree Mapping of monitor ID to its logical representation and child nodes.
     */
    void to_tree(std::map<size_t, std::tuple<std::string, std::vector<size_t>>> &tree) override;

  protected:
    [[nodiscard]] std::vector<std::shared_ptr<Hydra2Monitor>> getChildren() const override { return {subMonitor}; }

  private:
    std::shared_ptr<Hydra2Monitor> subMonitor; //**< Child monitor. */
};
