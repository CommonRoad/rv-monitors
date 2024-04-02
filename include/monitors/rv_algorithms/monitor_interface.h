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
     * @param earlyTermination Boolean indicating whether the complete trace has to be evaluated or whether the
     * execution can be terminated as soon as a global verdict for a monitor can be provided.
     */
    MonitorInterface(Formula formula, AlgorithmType algorithm, bool formulaCompatibilityCheck = true,
                     bool earlyTermination = false, const std::vector<std::shared_ptr<DomainStorage>> &domains = {});

    /**
     * Constructor for monitor based on several formulas.
     *
     * @param formula List of formulas.
     * @param algorithm Name of algorithm.
     * @param formulaCompatibilityCheck Boolean which activates check whether formula provided to monitor is evaluated
     * for monitor compatibility.
     * @param earlyTermination Boolean indicating whether the complete trace has to be evaluated or whether the
     * execution can be terminated as soon as a global verdict for a monitor can be provided.
     */
    MonitorInterface(std::vector<Formula> formulas, AlgorithmType algorithm, bool formulaCompatibilityCheck = true,
                     bool earlyTermination = false, const std::vector<std::shared_ptr<DomainStorage>> &domains = {});

    /**
     * Evaluation function for monitor algorithms.
     *
     * @param trace Trace which should be evaluated.
     * @param domains Domains for higher-order-based formulas.
     * @return Map of verdicts for each specification/formula at each time step of the trace.
     * If the length of the vector is 1, the runtime verification algorithm can only provide a
     * global verdict for the complete trace.
     */
    virtual std::map<std::string, std::vector<Verdict>>
    evaluate(Trace &trace, const std::vector<std::shared_ptr<DomainStorage>> &domains = {}) = 0;

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
    std::vector<Formula> formulas; //**< Temporal logic formulas which are evaluated by monitor */
    AlgorithmType algorithm;       //**< Name of algorithm. */
    bool earlyTermination; //**< Boolean indicating whether the complete trace has to be evaluated or whether the
    // execution can be terminated as soon as a global verdict for a monitor can be provided. */
    std::vector<std::shared_ptr<DomainStorage>> domains; //**< List of domains. */
};

extern const std::map<AlgorithmType, std::vector<MonitoringProperties>>
    supportedFormulaTypes; //**< Map of supported formula types to algorithm. */
