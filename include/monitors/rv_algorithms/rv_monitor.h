#pragma once

#include "monitors/formula/formula.h"
#include "monitors/rv_algorithms/btt_fsm/btt_fsm.h"
#include "monitors/rv_algorithms/dynamic_programming/dynamic.h"
#include "monitors/rv_algorithms/ltl_event_consuming/event_consuming.h"
#include "monitors/rv_algorithms/mt_fsm/mt_fsm.h"
#include "monitors/trace/trace.h"
#include "monitors/utils/execution_statistic.h"
#include <iostream>
#include <vector>

/**
 * List of configuration parameters for the different monitoring algorithms.
 */
struct MonitorConfiguration {
  public:
    /**
     * Constructor defining all configuration variables.
     *
     * @param algorithm Algorithm type, e.g., hydra, event-consuming LTL, etc.
     * @param measurement Indicates whether evaluation with time measurement should be performed
     * @param hydraConnectedEvaluation Indicates whether HYDRA monitor should perform connected evaluation
     * @param hydraReordering Indicates whether HYDRA monitor should perform reordering
     * @param shortCircuiting Indicates whether monitor should perform short circuiting
     * @param formulaEvaluationMode Indicates how to evaluate the formula
     */
    MonitorConfiguration(AlgorithmType algorithm, bool measurement, bool hydraConnectedEvaluation, bool hydraReordering,
                         bool shortCircuiting, FormulaEvaluationMode formulaEvaluationMode);

    /**
     * Constructor defining only algorithm type, past evaluation, and performance evaluation.
     *
     * @param algorithm Algorithm type, e.g., hydra, event-consuming LTL, etc.
     * @param measurement Indicates whether evaluation with time measurement should be performed
     */
    MonitorConfiguration(AlgorithmType algorithm, bool measurement);

    /*
     * Default constructor.
     */
    MonitorConfiguration() = default;

    AlgorithmType algorithm{
        AlgorithmType::hydra_2_mtl};     /**< Algorithm to use, e.g., Hydra, LTL event consuming, .... */
    bool performanceMeasurement{false};  /**< Indicates whether evaluation with time measurement should be performed */
    bool hydraConnectedEvaluation{true}; /**< Indicates whether HYDRA monitor should perform connected evaluation */
    bool hydraReordering{true};          /**< Indicates whether HYDRA monitor should perform reordering */
    bool shortCircuiting{true};          /**< Indicates whether monitor should perform short circuiting */
    FormulaEvaluationMode formulaEvaluationMode{globally}; /**< Indicates how to evaluate the formula */
};

class RVMonitor {
  public:
    /**
     * Constructor
     *
     * @param formula Temporal logic formula.
     * @param configuration List of configuration parameters for the different monitoring algorithms.
     * @param predicateCosts Cost assigned to predicates consisting of evaluation cost in interval [0, 100],
     * satisfaction probability in interval [0, 1], and priority >= 0.
     * @param domains List of domains.
     */
    RVMonitor(const Formula &formula, MonitorConfiguration configuration = {},
              const std::map<std::string, std::array<double, 3>> &predicateCosts = {},
              const std::vector<std::shared_ptr<DomainStorage>> &domains = {});

    /*
     * Copy constructor.
     * @param obj New RVMonitor object.
     */
    RVMonitor(const RVMonitor &obj);

    /*
     * Copy assignment
     *
     * @param other New RVMonitor object.
     */
    RVMonitor &operator=(const RVMonitor &other);

    /**
     * Constructor
     *
     * @param formula Temporal logic formula.
     * @param configuration List of configuration parameters for the different monitoring algorithms.
     * @param predicateCosts Cost assigned to predicates consisting of evaluation cost in interval [0, 100],
     * satisfaction probability in interval [0, 1], and priority >= 0.
     * @param domains List of domains.
     */
    RVMonitor(const std::vector<Formula> &formula, MonitorConfiguration configuration,
              const std::map<std::string, std::array<double, 3>> &predicateCosts = {},
              const std::vector<std::shared_ptr<DomainStorage>> &domains = {});

    /**
     * Evaluates trace based on temporal logic formula.
     *
     * @param trace Trace of predicates/atomic prepositions.
     * @param domains List of domains.
     * @return Verdict for each time step of trace.
     */
    std::map<std::string, std::vector<Verdict>> verify(const std::shared_ptr<Trace> &trace,
                                                       const std::vector<std::shared_ptr<DomainStorage>> &domains = {},
                                                       const std::vector<std::string> &formulas = {});

    /**
     * Getter for execution statistics.
     *
     * @return Execution statistic object.
     */
    [[nodiscard]] const ExecutionStatistic &getExecutionStatistic() const;

    /**
     * Creates string representation of activated monitoring algorithm.
     *
     * @return String representation of activated monitoring algorithm.
     */
    std::string to_string() const;

    /**
     * Getter for configuration
     * @return MonitorConfiguration
     */
    MonitorConfiguration getConfiguration() const;

    /**
     * Extracts violating results from first-order monitors.
     * @return Map containing violating domain values per rule and time step.
     */
    [[nodiscard]] std::map<std::string, std::map<size_t, std::vector<std::vector<DomainValue>>>> getMfotlRes() const;

  protected:
    std::unique_ptr<MonitorInterface> monitor; //**< Pointer to monitoring algorithm */
    ExecutionStatistic executionStatistic;     //**< Execution statistic object */
    MonitorConfiguration configuration; //**< Boolean indicating whether performance evaluation should be performed. */
};

/**
 * Overridden << operator for console output of algorithm type.
 *
 * @param stream Reference to OS stream
 * @param algorithm Algorithm type which should be outputted to terminal.
 * @return osstream for algorithm type.
 */
std::ostream &operator<<(std::ostream &stream, const AlgorithmType &algorithm);

/**
 * Converts a string into the corresponding algorithm type enum element.
 *
 * @param algorithmString String which should be converted
 * @return AlgorithmType enum element.
 */
AlgorithmType stringToAlgorithm(const std::string &algorithmString);
