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

enum AlgorithmType { dynamic_programming_ltl, event_consuming_ltl, mt_fsm, btt_fsm, event_consuming_mtl };

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
    RVMonitor(AlgorithmType algorithm, const Formula &formula, bool past, bool measurement);

    /**
     * Evaluates trace based on temporal logic formula.
     *
     * @param trace Trace of predicates/atomic prepositions.
     * @return Verdict for each time step of trace.
     */
    std::vector<Verdict> verify(Trace trace);

    /**
     * Getter for execution statistics.
     *
     * @return Execution statistic object.
     */
    const ExecutionStatistic &getExecutionStatistic() const;

  private:
    std::unique_ptr<MonitorInterface> monitor; //**< Pointer to monitoring algorithm */
    ExecutionStatistic executionStatistic;     //**< Execution statistic object */
    bool performanceMeasurement; //**< Boolean indicating whether performance evaluation should be performed. */
};

/**
 * Overridden << operator for console output of algorithm type.
 *
 * @param os Reference to OS stream
 * @param algorithm Algorithm type which should be outputted to terminal.
 * @return osstream for algorithm type.
 */
std::ostream &operator<<(std::ostream &os, const AlgorithmType &algorithm);

/**
 * Converts a string into the corresponding algorithm type enum element.
 *
 * @param algorithmString String which should be converted
 * @return AlgorithmType enum element.
 */
AlgorithmType stringToAlgorithm(const std::string &algorithmString);
