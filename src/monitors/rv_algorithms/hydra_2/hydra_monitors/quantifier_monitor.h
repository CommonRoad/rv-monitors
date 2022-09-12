#pragma once

#include "hydra_monitor.h"

/**
 * Hydra monitor representing quantifier monitor.
 */
class QuantifierMonitor : public HydraMonitor {
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
     * @param cost Evaluation cost assigned to monitor.
     * @param satisfactionProbability Satisfaction probability of monitor.
     */
    QuantifierMonitor(Formula formula, std::shared_ptr<HydraMonitor> subMonitor, size_t monitorId,
                      std::map<std::string, std::string> variables, DomainCollection &domains,
                      bool connectedEval, bool shortCircuiting, double cost, double satisfactionProbability);


    /**
     * Performs concrete step of quantifier monitor. If the current step is not covered by the trace or
     * one of the domains of the variables is empty the step results in a false verdict. In general,
     * for each combination determined by the domains the child monitor is started. After computing the subverdict
     * the child monitor will be reset to the current step number and restarted for the next combination and so on.
     * Finally, the step number is incremented for next execution of the quantifier monitor.
     *
     * The computation of the verdict depends on the type of quantifier monitor.
     *
     * @param parentId ID of parent monitor.
     * @param stepNumber Current step of monitor.
     * @return Verdict for monitoring step result.
     */
    Verdict concreteStep(size_t parentId, size_t stepNumber) override = 0;

    /**
     * Shifts the iterators for each variables domain so that a new combination is formed. If all combinations
     * are already requested the iterators will be reset.
     *
     * @return Boolean indicating whether a new combination can be formed.
     */
    bool nextCombination();

    /**
     * Resets the iterators forming the current combination.
     */
    void resetIterators();

    /**
     * Computes the number of combinations depending on the domains of the used variables.
     *
     * @return Number of combinations.
     */
    size_t numCombinations() const;

    /**
     * Checks whether one of the domains of the variables is empty.
     *
     * @return Boolean indicating whether one of the domains is empty.
     */
    bool checkNonEmptiness() const;

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
     * Resets the variables to default values and desired time step. If specific time step is not defined,
     * the step number 0 will be considered.
     *
     * @param stepNumber Step number.
     */
    void reset(std::optional<size_t> stepNumber = std::nullopt) override;

    /**
     * Computes normalized cost over provided monitor.
     *
     * @param monitor Monitor which should be used for universal evaluation cost computation.
     * @param numEval Number of evaluations.
     * @return Normalized evaluation cost of universal monitor in interval [0, 100].
     */
    static double computeCost(const std::shared_ptr<HydraMonitor> &monitor, size_t numEval);

    /**
     * Computes satisfaction probability over provided monitor.
     *
     * @param monitor Monitor which should be used for universal satisfaction probability computation.
     * @return Satisfaction probability of universal monitor in interval [0, 1].
     */
    static double computeSatisfactionProbability(const std::shared_ptr<HydraMonitor> &monitor, size_t numEval);

  protected:
    std::shared_ptr<HydraMonitor> subMonitor; //**< Child monitor. */
    bool shortCircuiting; //**< Child monitors. */
    std::map<std::string, Value> variableValues; //**< Variables values from higher quantifier scopes. */
    Trace trace; //**< Trace. */
    std::vector<std::pair<std::string, Domain>> varDomains; //**< Domains of variables. */
    std::vector<std::vector<Value>::iterator> iters; //**< Iterators forming the current combination. */

};
