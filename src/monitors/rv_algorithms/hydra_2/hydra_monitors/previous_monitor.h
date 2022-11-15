//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include "../../../utils/interval.h"
#include "hydra_monitor.h"
#include <utility>

/**
 * Hydra2 monitor representing (strong) previous operator.
 */
class PreviousMonitor : public Hydra2Monitor {
  public:
    /**
     * Constructor.
     *
     * @param formula Formula to be monitored.
     * @param subMonitor Child monitor.
     * @param interval  Interval of operator.
     * @param monitorId ID of monitor.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     */
    PreviousMonitor(const Formula &formula, std::shared_ptr<Hydra2Monitor> subMonitor,
                    IntervalTemporalOperator interval, size_t monitorId, bool connectedEval)
        : Hydra2Monitor(formula, monitorId, connectedEval, subMonitor->getEvalCost(),
                        subMonitor->getSatisfactionProbability()),
          subMonitor(std::move(subMonitor)), interval(interval){};

    /**
     * Performs concrete step of previous operator monitor.
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
    void setTrace(const Trace &tra) override;

    /**
     * Setter for the current values of the quantified variables.
     *
     * @param values Current values.
     */
    void setVariableValues(std::map<std::string, Value> values) override;

    /**
     * Resets monitor variables to default values.
     *
     */
    void reset() override;

    /**
     * Clones this monitor.
     * @return New shared_ptr of this monitor.
     */
    std::shared_ptr<Hydra2Monitor> clone() override;

    /**
     * Creates string representation of monitor.
     *
     * @return String representing monitor.
     */
    std::string to_string() override;

  private:
    std::shared_ptr<Hydra2Monitor> subMonitor; //**< Child monitor. */
    std::optional<Verdict> currentVerdict;     //**< Stores previous verdict. */
    IntervalTemporalOperator interval;         //**< Interval of operator. */
};
