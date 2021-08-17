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
#include "monitors/rv_algorithms/event_consuming/event_consuming.h"
#include "monitors/rv_algorithms/mt_fsm/mt_fsm.h"
#include "monitors/trace/trace.h"
#include "monitors/utils/execution_statistic.h"
#include <iostream>
#include <vector>

enum AlgorithmType { dynamic_programming_ltl, event_consuming_ltl, mt_fsm, btt_fsm, event_consuming_mtl };

class RVMonitor {
  public:
    RVMonitor(AlgorithmType algorithm, const Formula &formula, bool past, bool measurement);

    /**
     * Executes algorithm on a given trace and with given parameters.
     * @param formula Formula.
     * @param algorithm Monitoring algorithm.
     * @param trace Trace.
     * @param past True if formula is in PLTLf.
     * @param consoleOutput True if console output is desired.
     */
    bool verify(Trace trace);

    const ExecutionStatistic &getExecutionStatistic() const;

  private:
    bool past{false};
    AlgorithmType algorithm{event_consuming_ltl};
    std::unique_ptr<MtFsm> mtFSM;
    std::unique_ptr<BttFsm> bttFSM;
    std::unique_ptr<DynamicProgrammingLTL> dynamic_programming;
    std::unique_ptr<EventConsumingLTL> ltl_event_consuming;
    Formula formula;
    ExecutionStatistic executionStatistic;
    bool performanceMeasurement;
};

/**
 * Easier output for algorithms.
 * @param os
 * @param algorithm
 * @return osstream for algorithm.
 */
std::ostream &operator<<(std::ostream &os, const AlgorithmType &algorithm);

/**
 * Converts a string into the corresponding algorithm.
 * @param algorithmString
 * @return
 */
AlgorithmType stringToAlgorithm(const std::string &algorithmString);
