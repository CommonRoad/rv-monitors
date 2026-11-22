#pragma once

#include "../utils/utils.h"

enum AlgorithmType {
    dynamic_programming_ltl,
    event_consuming_ltl,
    mt_fsm,
    btt_fsm,
    hydra_mtl,
    hydra_2_mtl,
    event_consuming_mtl
};

enum FormulaEvaluationMode {
    globally,                   /**< Assume an implicit top-level globally and evaluate each time step individually.
                                 * Provides a verdict for all time steps. */
    globally_early_termination, /**< Assume an implicit top-level globally and short circuit the evaluation if possible.
                                 * Only provides a verdict for the initial time step. */
    initial_step,               /**< Evaluate the formula as-is and provide a verdict for the initial time step. */
};

/**
 * Interface for runtime verification algorithms/monitors.
 */
class MonitorInterface {
  public:
    /**
     * Constructor for monitor.
     *
     * @param formula Temporal logic formula.
     * @param algorithm Name of algorithm.
     * @param formulaCompatibilityCheck Boolean which activates check whether formula provided to monitor is evaluated
     * for monitor compatibility.
     * @param formulaEvaluationMode Indicates how to evaluate the formula.
     * @param domains Domains for higher-order-based formulas.
     */
    MonitorInterface(Formula formula, AlgorithmType algorithm, bool formulaCompatibilityCheck = true,
                     FormulaEvaluationMode formulaEvaluationMode = globally,
                     const std::vector<std::shared_ptr<DomainStorage>> &domains = {});

    /**
     * Constructor for monitor based on several formulas.
     *
     * @param formulas List of formulas.
     * @param algorithm Name of algorithm.
     * @param formulaCompatibilityCheck Boolean which activates check whether formula provided to monitor is evaluated
     * for monitor compatibility.
     * @param formulaEvaluationMode Indicates how to evaluate the formula.
     * @param domains Domains for higher-order-based formulas.
     */
    MonitorInterface(std::vector<Formula> formulas, AlgorithmType algorithm, bool formulaCompatibilityCheck = true,
                     FormulaEvaluationMode formulaEvaluationMode = globally,
                     const std::vector<std::shared_ptr<DomainStorage>> &domains = {});

    /**
     * Evaluation function for monitor algorithms.
     *
     * @param trace Trace which should be evaluated.
     * @param domains Domains for higher-order-based formulas.
     * @param formulaIDs List of formula IDs which should be evaluated.
     * @return Map of verdicts for each specification/formula at each time step of the trace.
     * If the length of the vector is 1, the runtime verification algorithm can only provide a
     * global verdict for the complete trace.
     */
    virtual std::map<std::string, std::vector<Verdict>>
    evaluate(const std::shared_ptr<Trace> &trace, const std::vector<std::shared_ptr<DomainStorage>> &domains = {},
             const std::vector<std::string> &formulaIDs = {}) = 0;

    /**
     * Sets the time step from which on the trace is evaluated. Events before this time step stay in the trace as
     * context for past operators, but they are neither evaluated nor reported, and they do not cause early
     * termination. Evaluation starts at the first event whose time step is >= startTimeStep. The default is 0, i.e.,
     * the complete trace is evaluated. Currently only considered by Hydra2.
     *
     * @param startTimeStep First time step which is evaluated.
     */
    void setEvaluationStartTimeStep(size_t startTimeStep);

    /**
     * Getter for the first evaluated time step.
     *
     * @return First time step which is evaluated.
     */
    [[nodiscard]] size_t getEvaluationStartTimeStep() const;

    /**
     * Prints a representation of the monitor.
     *
     * @return String representation of the monitor.
     */
    [[nodiscard]] virtual std::string to_string() const = 0;

    /**
     * Getter for formulas.
     *
     * @return List of formulas to monitor.
     */
    [[nodiscard]] const std::vector<Formula> &getFormulas() const;

    /**
     * Getter for domains.
     *
     * @return List of registered domains.
     */
    [[nodiscard]] std::vector<std::shared_ptr<DomainStorage>> &getDomains();

    /**
     * Function to extract combination of violating domain values.
     *
     * @return Map containing violating domain values per rule and time step.
     */
    [[nodiscard]] virtual std::map<std::string, std::map<size_t, std::vector<std::vector<DomainValue>>>>
    getViolatingDomainValues() const;

  protected:
    std::vector<Formula> formulas;                       /**< Temporal logic formulas which are evaluated by monitor */
    AlgorithmType algorithm;                             /**< Name of algorithm. */
    FormulaEvaluationMode formulaEvaluationMode;         /**< Indicates how to evaluate the formula. */
    std::vector<std::shared_ptr<DomainStorage>> domains; /**< List of domains. */
    size_t evaluationStartTimeStep{0};                   /**< First time step which is evaluated. */
};

extern const std::map<AlgorithmType, std::vector<MonitoringProperties>>
    supportedFormulaTypes; /**< Map of supported formula types to algorithm. */
