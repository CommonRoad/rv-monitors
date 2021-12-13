//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
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

enum AlgorithmType { dynamic_programming_ltl, event_consuming_ltl, mt_fsm, btt_fsm, hydra_mtl };

/**
 * List of configuration parameters for the different monitoring algorithms.
 */
struct MonitorConfiguration {
    MonitorConfiguration(AlgorithmType algorithm, bool past, bool measurement, bool hydraConnectedEvaluation,
                         bool hydraReordering);
    MonitorConfiguration(AlgorithmType algorithm, bool past, bool measurement);
    MonitorConfiguration() = default;
    AlgorithmType algorithm{AlgorithmType::hydra_mtl}; /**< Algorithm to use, e.g., Hydra, LTL event consuming, .... */
    bool past{false}; /**< Set only past evaluation (only relevant for certain algorithms, i.e. LTL algorithms) */
    bool performanceMeasurement{false};  /**< Indicates whether time measurement should be evaluated */
    bool hydraConnectedEvaluation{true}; /**< Indicates whether Hydra monitor should perform connected evaluation */
    bool hydraReordering{true};          /**< Indicates whether Hydra monitor should perform reordering */
};

class RVMonitor {
  public:
    /**
     * Constructor
     *
     * @param algorithm Algorithm type, e.g., event-consuming LTL, BTT-FSM, etc.
     * @param formula Temporal logic formula.
     * @param past Boolean indicating whether formula specifies only future or past properties (true if formula is in
     * PLTLf).
     * @param measurement Boolean indicating whether performance evaluation should be performed.
     */
    RVMonitor(const Formula &formula, MonitorConfiguration configuration = {},
              const std::vector<std::tuple<std::string, double, double>> &predicateCosts = {});

    RVMonitor(const RVMonitor &obj);

    RVMonitor &operator=(const RVMonitor &other);

    /**
     * Constructor
     *
     * @param algorithm Algorithm type, e.g., event-consuming LTL, BTT-FSM, etc.
     * @param formula List of temporal logic formulas.
     * @param past Boolean indicating whether formula specifies only future or past properties (true if formula is in
     * PLTLf).
     * @param measurement Boolean indicating whether performance evaluation should be performed.
     */
    RVMonitor(const std::vector<Formula> &formula, MonitorConfiguration configuration,
              const std::vector<std::tuple<std::string, double, double>> &predicateCosts = {});

    /**
     * Evaluates trace based on temporal logic formula.
     *
     * @param trace Trace of predicates/atomic prepositions.
     * @return Verdict for each time step of trace.
     */
    std::map<std::string, std::vector<Verdict>> verify(Trace trace);

    /**
     * Getter for execution statistics.
     *
     * @return Execution statistic object.
     */
    const ExecutionStatistic &getExecutionStatistic() const;

    std::string to_string();

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
