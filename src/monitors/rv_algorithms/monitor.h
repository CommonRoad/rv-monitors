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
class Monitor {
  public:
    /**
     * Constructor for monitor.
     *
     * @param formula Temporal logic formula.
     * @param timeProperty  Specification whether formula is/can be specified with future/past operators.
     */
    Monitor(Formula formula);

    /**
     * Evaluation function for monitor algorithms.
     *
     * @param trace Trace which should be evaluated.
     * @return List of verdicts for each time step of the trace. If the length of the vector is 1,
     * the runtime verification algorithm can only provide a global verdict for the complete trace.
     */
    virtual std::vector<Verdict> evaluate(Trace &trace) = 0;

    /**
     * Getter for formula which the runtime verification algorithms monitors.
     *
     * @return Temporal logic formula.
     */
    [[nodiscard]] const Formula &getFormula() const;

    /**
     * Prints a representation of the monitor.
     *
     * @return String representation of the monitor.
     */
    virtual std::string to_string() const = 0;

  protected:
    Formula formula; //**< Formula which is evaluated by monitor */
};
