//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include "../utils/utils.h"

/**
 * Interface for runtime verification algorithms/monitors.
 */
class MonitorInterface {
  public:
    /**
     * Constructor for monitor.
     *
     * @param formula Temporal logic formula.
     * @param supportedFormulaTypes List of formula types which are supported by monitor.
     * @param formulaCompatibilityCheck Boolean which activates check whether formula provided to monitor is evaluated
     * for monitor compatibility.
     */
    MonitorInterface(Formula formula, std::vector<SupportedFormulaType> supportedFormulaTypes,
                     bool formulaCompatibilityCheck = true);

    /**
     * Constructor for monitor based on several formulas.
     *
     * @param formula List of formulas.
     * @param supportedFormulaTypes List of formula types which are supported by monitor.
     * @param formulaCompatibilityCheck Boolean which activates check whether formula provided to monitor is evaluated
     * for monitor compatibility.
     */
    MonitorInterface(std::vector<Formula> formulas, std::vector<SupportedFormulaType> supportedFormulaTypes,
                     bool formulaCompatibilityCheck = true);

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
    std::vector<SupportedFormulaType>
        supportedFormulaTypes; //**< List of formula types which are supported by monitor */
};
