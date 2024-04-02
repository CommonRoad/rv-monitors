#pragma once

#include "hydra_monitor.h"

/**
 * Monitor representing negation Hydra2 operator.
 */
class NegationMonitor : public Hydra2Monitor {
  public:
    /**
     * Constructor
     *
     * @param formula Formula to be monitored.
     * @param subMonitor Child monitor.
     * @param monitorId ID of monitor.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     */
    NegationMonitor(Formula formula, std::shared_ptr<Hydra2Monitor> subMonitor, size_t monitorId, bool connectedEval)
        : Hydra2Monitor(std::move(formula), monitorId, connectedEval, subMonitor->getEvalCost(),
                        1 - subMonitor->getSatisfactionProbability()),
          subMonitor(std::move(subMonitor)){};

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
     * @param trace Trace.
     * @param domains List of domains.
     */
    void setTrace(const Trace &trace, const std::vector<std::shared_ptr<DomainStorage>> &domains = {}) override;

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
     * @return New shared_ptr of this monitor.
     */
    std::shared_ptr<Hydra2Monitor> clone() override;

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
    void setStepNumber(size_t stepNumber, size_t parentId);

  private:
    std::shared_ptr<Hydra2Monitor> subMonitor; //**< Child monitor. */
};
