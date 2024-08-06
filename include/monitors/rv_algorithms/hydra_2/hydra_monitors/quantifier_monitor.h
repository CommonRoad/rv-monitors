#pragma once

#include "hydra_monitor.h"
#include "monitors/collections/domain_collection.h"

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
     * @param domains Collection of domains.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     * @param shortCircuiting Boolean indicating whether short circuiting should be used.
     * @param cost Evaluation cost assigned to monitor.
     * @param satisfactionProbability Satisfaction probability of monitor.
     */
    QuantifierMonitor(Formula formula, std::shared_ptr<Hydra2Monitor> subMonitor, size_t monitorId,
                      std::map<std::string, std::shared_ptr<DomainStorage>> &variableValues, bool connectedEval,
                      bool shortCircuiting, double cost, double satisfactionProbability);

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
    Verdict concreteStep(size_t parentId, size_t stepNumber) override;

    /**
     * Initializes monitor variables and evaluates domains and trace for immediately return. Uses true for base verdict.
     *
     * @param parentId ID of parent monitor.
     * @return Verdict.
     */
    Verdict initializeStepTrue(size_t parentId);

    /**
     * Initializes monitor variables and evaluates domains and trace for immediately return. Uses false for base
     * verdict.
     *
     * @param parentId ID of parent monitor.
     * @return Verdict.
     */
    Verdict initializeStepFalse(size_t parentId);

    /**
     *
     * @param parentId ID of parent monitor.
     * @param stepNumber Current step of monitor.
     * @param numberCombinations Number of combinations of variables.
     * @return Verdict.
     */
    virtual Verdict quantifierStep(size_t parentId, size_t stepNumber, size_t numberCombinations) = 0;

    /**
     * Returns next monitor for evaluation.
     *
     * @param parentId ID of parent monitor.
     * @param count ID of current combination.
     * @return Monitor.
     */
    std::shared_ptr<Hydra2Monitor> nextEvalMonitor(size_t stepNumber);

    /**
     * Extracts combination of domain values.
     *
     * @return List of domain values for sub-monitor.
     */
    std::vector<DomainValue> nextCombination();

    /**
     * Shifts the iterators for each variables domain so that a new combination is formed. If all combinations
     * are already requested the iterators will be reset.
     */
    void shiftCombination();

    /**
     * Resets the iterators forming the current combination.
     */
    void resetIterators();

    /**
     * Computes the number of combinations depending on the domains of the used variables.
     *
     * @return Number of combinations.
     */
    [[nodiscard]] size_t numCombinations() const;

    /**
     * Checks whether one of the domains of the variables is empty.
     *
     * @return Boolean indicating whether one of the domains is empty.
     */
    [[nodiscard]] bool checkNonEmptiness() const;

    /**
     * Setter for trace.
     *
     * @param tra Trace.
     * @param domains List of domains.
     */
    void setTrace(const std::shared_ptr<Trace> &tra,
                  const std::vector<std::shared_ptr<DomainStorage>> &domains = {}) override;

    /**
     * Setter for current variable values from higher quantifier scopes.
     *
     * @param values Variable values.
     */
    void addDomainValues(const std::vector<DomainValue> &vals) override;

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

    /**
     * Function to extract combination of violating domain values.
     *
     * @return Map containing violating domain values per rule and time step.
     */
    [[nodiscard]] std::map<size_t, std::vector<std::vector<DomainValue>>> getViolatingDomainValues() const override;

    /**
     * Extracts unique ID from all currently relevant domain values.
     *
     * @return String representing unique ID.
     */
    std::string getUniqueID();

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
    std::shared_ptr<Hydra2Monitor> subMonitor;                          //**< Child monitor. */
    std::map<std::string, std::shared_ptr<Hydra2Monitor>> evalMonitors; //**< Monitors for evaluation. */
    bool shortCircuiting; //**< Boolean indicating whether short-circuiting should be used. */
    std::map<std::string, std::shared_ptr<DomainStorage>>
        variableValues;                                 //**< Mapping of name of quantified variables to domains. */
    std::shared_ptr<Trace> trace;                       //**< Trace. */
    std::vector<std::vector<std::any>::iterator> iters; //**< Iterators forming the current combination. */
    std::vector<DomainValue> domainValues;              //**< Domain values. */
    std::map<size_t, std::vector<std::vector<DomainValue>>> violations; //**< Violating domain values per time step. */
    std::vector<DomainValue> currentDomainValues;                       //**< Currently evaluated domain values. */
};
