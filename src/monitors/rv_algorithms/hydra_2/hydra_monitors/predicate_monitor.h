//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include "hydra_monitor.h"

/**
 * Hydra2 monitor representing predicate.
 */
class PredicateMonitor : public Hydra2Monitor {
  public:
    /**
     * Constructor.
     *
     * @param formula Formula to be monitored.
     * @param monitorId ID of monitor.
     * @param functions Collection of functions.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     * @param evalCost Evaluation cost assigned to monitor.
     * @param satisfactionProbability Satisfaction probability of monitor.
     */
    PredicateMonitor(Formula formula, size_t monitorId, FunctionCollection &functions, bool connectedEval, double evalCost = 0.0,
                     double satisfactionProbability = 1.0)
        : Hydra2Monitor(std::move(formula), monitorId, connectedEval, evalCost, satisfactionProbability), functions(functions) {};

    /**
     * Performs concrete step of predicate monitor. It evaluates the trace element of the concrete step. Returns false
     * if stepNumber >= traceLength.
     *
     * @param parentId ID of parent monitor.
     * @param stepNumber Current step/position in trace.
     * @return Verdict for monitoring step result.
     */
    Verdict concreteStep(size_t parentId, size_t stepNumber) override;

    /**
     * Setter for trace.
     *
     * @param trace Trace.
     */
    void setTrace(const Trace &trace) override;

    /**
     * Setter for the current values of the quantified variables.
     *
     * @param values Current values.
     */
    void setVariableValues(std::map<std::string, Value> values) override;

    /**
     * Clones this monitor.
     * @return New shared_ptr of this monitor.
     */
    std::shared_ptr<Hydra2Monitor> clone() override;

    /**
     * Resets monitor variables to default values at a specified step number.
     *
     * @param stepNumber Optional step number.
     */
    void reset(std::optional<size_t> stepNumber = std::nullopt) override;

    /**
     * Creates string representation of monitor.
     *
     * @return String representing monitor.
     */
    std::string to_string() override;

  private:
    Trace trace; //**< Trace. */
    std::map<std::string, Value> variableValues; //**< Current values of quantified variables. */
    FunctionCollection functions; //**< Functions used for evaluation of predicates. */

};
