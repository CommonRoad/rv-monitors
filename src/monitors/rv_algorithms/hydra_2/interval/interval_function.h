//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include "monitors/formula/formula.h"
#include "monitors/trace/trace.h"

class Formula;

/**
 * Hydra2 monitor representing a function.
 */
class IntervalFunction {
  public:
    /**
     * Constructor.
     *
     * @param formula Formula to be monitored.
     */
    IntervalFunction(Formula formula) : formula(std::move(formula)){};

    /**
     * Performs concrete step of predicate monitor. It evaluates the Trace<T> element of the concrete step. Returns
     * false if stepNumber >= Trace<T>Length.
     *
     * @param parentId ID of parent monitor.
     * @param stepNumber Current step/position in Trace<T>.
     * @return Verdict for monitoring step result.
     */
    std::array<size_t, 2> concreteStep(size_t stepNumber);

    /**
     * Setter for Trace<T>.
     *
     * @param Trace<T> Trace<T>.
     */
    void setTrace(const Trace &trace);

    /**
     * Creates string representation of monitor.
     *
     * @return String representing monitor.
     */
    std::string to_string();

  private:
    Trace trace; //**< Trace<T> which should be monitored. */
    Formula formula;
};
