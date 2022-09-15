//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include "../utils/utils.h"

enum AlgorithmType { dynamic_programming_ltl, event_consuming_ltl, mt_fsm, btt_fsm, hydra_mtl, event_consuming_mtl };

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
     */
    MonitorInterface(Formula formula, AlgorithmType algorithm, bool formulaCompatibilityCheck = true);

    /**
     * Constructor for monitor based on several formulas.
     *
     * @param formula List of formulas.
     * @param algorithm Name of algorithm.
     * @param formulaCompatibilityCheck Boolean which activates check whether formula provided to monitor is evaluated
     * for monitor compatibility.
     */
    MonitorInterface(std::vector<Formula> formulas, AlgorithmType algorithm, bool formulaCompatibilityCheck = true);

    /**
     * Evaluation function for monitor algorithms.
     *
     * @param trace Trace which should be evaluated.
     * @return Map of verdicts for each specification/formula at each time step of the trace.
     * If the length of the vector is 1, the runtime verification algorithm can only provide a
     * global verdict for the complete trace.
     */
    virtual std::map<std::string, std::vector<Verdict>> evaluate(Trace &trace) = 0;

    /**
     * Prints a representation of the monitor.
     *
     * @return String representation of the monitor.
     */
    virtual std::string to_string() const = 0;

    /**
     * Getter for formulas.
     *
     * @return List of formulas to monitor.
     */
    const std::vector<Formula> &getFormulas() const;

  protected:
    std::vector<Formula> formulas; //**< Temporal logic formulas which are evaluated by monitor */
    AlgorithmType algorithm;       //**< Name of algorithm. */
};

extern const std::map<AlgorithmType, std::vector<MonitoringProperties>>
    supportedFormulaTypes; //**< Map of supported formula types to algorithm. */
