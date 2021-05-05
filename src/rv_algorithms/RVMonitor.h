//
// Created by Sebastian Maierhofer on 18.01.21.
//

#ifndef RV_MONITORS_RVMONITOR_H
#define RV_MONITORS_RVMONITOR_H

#include "BTT-FSM/BTT_FSM.h"
#include "MT-FSM/MT_FSM.h"
#include "formula/formula.h"
#include "trace/trace.h"
#include <iostream>
#include <vector>

enum AlgorithmType { dynamic, eventconsuming_ltl, mt_fsm, btt_fsm, eventconsuming_mtl };

class RVMonitor {
  private:
  public:
    RVMonitor(bool past, AlgorithmType algorithm, bool consoleOutput, const Formula &formula);

  private:
    bool past{false};
    AlgorithmType algorithm{eventconsuming_ltl};
    bool consoleOutput{true};
    std::unique_ptr<MT_FSM> mtFSM;
    std::unique_ptr<BTT_FSM> bttFSM;
    Formula formula;

  public:
    /**
     * Executes algorithm on a given trace and with given parameters.
     * @param formula Formula.
     * @param algorithm Monitoring algorithm.
     * @param trace Trace.
     * @param past True if formula is in PLTLf.
     * @param consoleOutput True if console output is desired.
     */
    bool verify(Trace trace);
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
#endif // RV_MONITORS_RVMONITOR_H
