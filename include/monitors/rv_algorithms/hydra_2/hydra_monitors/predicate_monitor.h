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
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     * @param evalCost Evaluation cost assigned to monitor.
     * @param satisfactionProbability Satisfaction probability of monitor.
     */
    PredicateMonitor(Formula formula, size_t monitorId, bool connectedEval, double evalCost = 0.0,
                     double satisfactionProbability = 1.0)
        : Hydra2Monitor(std::move(formula), monitorId, connectedEval, evalCost, satisfactionProbability){};

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
     * Creates string representation of monitor.
     *
     * @return String representing monitor.
     */
    std::string to_string() override;

  private:
    Trace trace; //**< Trace which should be monitored. */
};
