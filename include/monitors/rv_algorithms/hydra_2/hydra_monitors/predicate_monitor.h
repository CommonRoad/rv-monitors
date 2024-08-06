#pragma once

#include "hydra_monitor.h"

/**
 * Hydra2 monitor representing predicate.
 */
class PredicateMonitor : public Hydra2Monitor {
  public:
    /**
     * Constructor.
     *
     * @param formula Formula to be monitored.
     * @param monitorId ID of monitor.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     * @param evalCost Evaluation cost assigned to monitor.
     * @param satisfactionProbability Satisfaction probability of monitor.
     */
    PredicateMonitor(Formula formula, size_t monitorId, bool connectedEval, double evalCost = 0.0,
                     double satisfactionProbability = 1.0);

    /**
     * Performs concrete step of predicate monitor. It evaluates the trace element of the concrete step. Returns false
     * if stepNumber >= traceLength.
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
    void setTrace(const std::shared_ptr<Trace> &trace,
                  const std::vector<std::shared_ptr<DomainStorage>> &domains = {}) override;

    /**
     * Calls sub-monitors to add domain value.
     *
     * @param vals Domain values which should be added.
     */
    void addDomainValues(const std::vector<DomainValue> &vals) override;

    /**
     * Resets predicate monitor class members.
     */
    void reset() override;

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
    std::shared_ptr<Trace> trace;                  //**< Trace which should be monitored. */
    std::string name;                              //**< Name of predicate. */
    std::vector<std::string> relevantArguments;    //**< All arguments of predicate. */
    std::vector<ConstantValue> constArguments;     //**< Constant arguments of predicate. */
    std::vector<DomainValue> domainValueArguments; //**< Domain values. */
    std::vector<DomainValue> orderedValues;        //**< List of ordered predicate arguments as domain values. */
    std::string domainName; //**< Extended predicate name considering instantiated domain values. */
};
