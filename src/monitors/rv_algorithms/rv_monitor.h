//
// Created by Sebastian Maierhofer on 18.01.21.
//

#ifndef RV_MONITORS_RV_MONITOR_H
#define RV_MONITORS_RV_MONITOR_H

#include "monitors/formula/formula.h"
#include "monitors/rv_algorithms/btt_fsm/btt_fsm.h"
#include "monitors/rv_algorithms/mt_fsm/mt_fsm.h"
#include "monitors/trace/trace.h"
#include "monitors/utils/execution_statistic.h"
#include <iostream>
#include <vector>

enum AlgorithmType { dynamic, eventconsuming_ltl, mt_fsm, btt_fsm, eventconsuming_mtl };

class RVMonitor {
  private:
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

  private:
    bool past{false};
    AlgorithmType algorithm{eventconsuming_ltl};
    std::unique_ptr<MtFsm> mtFSM;
    std::unique_ptr<BttFsm> bttFSM;
    Formula formula;
    ExecutionStatistic executionStatistic;
    bool performanceMeasurement;

  public:
    const ExecutionStatistic &getExecutionStatistic() const;
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
#endif // RV_MONITORS_RV_MONITOR_H
