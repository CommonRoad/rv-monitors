#pragma once

#include "hydra_monitor.h"

/**
 * Hydra2 monitor representing the equality operator monitor.
 */
class EqualityOperatorMonitor : public Hydra2Monitor {
  public:
    /**
     * Constructor.
     *
     * @param formula Formula to be monitored.
     * @param monitorId ID of monitor.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     * @param evalCost Evaluation costs.
     * @param satisfactionProbability Satisfaction probability.
     */
    EqualityOperatorMonitor(Formula formula, size_t monitorId, bool connectedEval, double evalCost = 0, double satisfactionProbability = 100.0)
        : Hydra2Monitor(std::move(formula), monitorId, connectedEval, evalCost, satisfactionProbability) {}

    /**
     * Performs concrete step of same function monitor. Constants and variables are compared for equality.
     *
     * @param parentId ID of parent monitor.
     * @param stepNumber Current step of monitor.
     * @return Verdict for monitoring step result.
     */
    Verdict concreteStep(size_t parentId, size_t stepNumber) override;

    /**
     * Setter for trace.
     *
     * @param tra Trace.
     */
    void setTrace(const Trace &tra) override;

    /**
     * Setter for current variable values from higher quantifier scopes.
     *
     * @param values Variable values.
     */
    void setVariableValues(std::map<std::string, Value> values) override;

    /**
     * Resets the variables to default values.
     *
     */
    void reset() override;

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

  private:
    Trace trace; //**< Trace. */
    std::map<std::string, Value> variableValues;  //**< Variable values from higher quantifier scopes. */

};
