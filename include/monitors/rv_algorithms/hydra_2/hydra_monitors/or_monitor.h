#pragma once

#include "hydra_monitor.h"

/**
 * Hydra2 monitor representing or-operator.
 */
class OrMonitor : public Hydra2Monitor {
  public:
    /**
     * Constructor of or-monitor.
     *
     * @param formula Formula to be monitored.
     * @param subMonitors Child monitors.
     * @param monitorId ID of monitor.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     * @param shortCircuiting Boolean indicating whether short circuiting should be used.
     */
    OrMonitor(Formula formula, const std::vector<std::shared_ptr<Hydra2Monitor>> &subMonitors, size_t monitorId,
              bool connectedEval, bool shortCircuiting)
        : Hydra2Monitor(std::move(formula), monitorId, connectedEval, computeEvaluationCost(subMonitors),
                        computeSatisfactionProbability(subMonitors)),
          subMonitors(subMonitors), shortCircuiting(shortCircuiting){};

    /**
     * Performs concrete step of or-monitor.
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
     * Calls sub-monitors to add domain value.
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
    void setStepNumber(size_t stepNumber, size_t parentId) override;

  private:
    std::vector<std::shared_ptr<Hydra2Monitor>> subMonitors; //**< Child monitors. */
    bool shortCircuiting; //**< Boolean indicating whether short-circuiting should be used. */

    /**
     * Computes normalized cost over all provided monitors.
     *
     * @param monitors List of monitors which should be used for predicate evaluation cost computation.
     * @return Normalized evaluation cost of or-monitor in interval [0, 100]
     */
    static double computeEvaluationCost(const std::vector<std::shared_ptr<Hydra2Monitor>> &monitors);

    /**
     * Computest satisfaction probability over all provided monitors.
     *
     * @param monitors List of monitors which should be used for predicate satisfaction probability computation.
     * @return Satisfaction probability of or-monitor in interval [0, 1]
     */
    static double computeSatisfactionProbability(const std::vector<std::shared_ptr<Hydra2Monitor>> &monitors);
};
