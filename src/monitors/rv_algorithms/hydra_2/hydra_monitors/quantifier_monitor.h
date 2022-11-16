#pragma once

#include "hydra_monitor.h"

/**
 * Hydra2 monitor representing quantifier monitor.
 */
class QuantifierMonitor : public Hydra2Monitor {
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
    QuantifierMonitor(Formula formula, std::shared_ptr<Hydra2Monitor> subMonitor, size_t monitorId,
                      const std::map<std::string, std::string> &variables, DomainCollection &domains,
                      bool connectedEval, bool shortCircuiting, double cost, double satisfactionProbability);

    /**
     * Performs concrete step of quantifier monitor. If the current step is not covered by the trace or
     * one of the domains of the variables is empty the step evaluation stops immediately. In general,
     * a monitor is cloned from the child monitor for each combination and the corresponding variable values
     * are inserted. For the step evaluation not all cloned monitors have to be used. If a cloned monitor is not
     * on the current step number, then it will be implicitly made up at its next step evaluation.
     *
     * The computation of the verdict depends on the type of quantifier monitor.
     *
     * @param parentId ID of parent monitor.
     * @param stepNumber Current step of monitor.
     * @return Verdict for monitoring step result.
     */
    Verdict concreteStep(size_t parentId, size_t stepNumber) override = 0;

    /**
     * Returns next monitor for evaluation.
     *
     * @param parentId ID of parent monitor.
     * @param count ID of current combination.
     * @return Monitor.
     */
    std::shared_ptr<Hydra2Monitor> nextEvalMonitor(size_t parentId, size_t count);

    /**
     * Shifts the iterators for each variables domain so that a new combination is formed. If all combinations
     * are already requested the iterators will be reset.
     */
    void nextCombination();

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
     * Resets the variables to default values.
     *
     */
    void reset() override;

    /**
     * Computes normalized cost over provided monitor.
     *
     * @param monitor Monitor which should be used for universal evaluation cost computation.
     * @param numEval Number of evaluations.
     * @return Normalized evaluation cost of universal monitor in interval [0, 100].
     */
    static double computeCost(const std::shared_ptr<Hydra2Monitor> &monitor, size_t numEval);

    /**
     * Computes satisfaction probability over provided monitor.
     *
     * @param monitor Monitor which should be used for universal satisfaction probability computation.
     * @return Satisfaction probability of universal monitor in interval [0, 1].
     */
    static double computeSatisfactionProbability(const std::shared_ptr<Hydra2Monitor> &monitor, size_t numEval);

  protected:
    std::shared_ptr<Hydra2Monitor> subMonitor;                //**< Child monitor. */
    std::vector<std::shared_ptr<Hydra2Monitor>> evalMonitors; //**< Monitors for evaluation. */
    bool shortCircuiting;                        //**< Boolean indicating whether short circuiting should be used. */
    std::map<std::string, Value> variableValues; //**< Variables values from higher quantifier scopes. */
    Trace trace;                                 //**< Trace. */
    std::vector<std::pair<std::string, Domain>> varDomains; //**< Domains of variables. */
    std::vector<std::vector<Value>::iterator> iters;        //**< Iterators forming the current combination. */
    size_t numberCombinations;                              //**< Number of combinations. */
};
