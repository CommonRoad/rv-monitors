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
                    Hydra2IntervalTemporalOperator interval, size_t monitorId, bool connectedEval)
        : Hydra2Monitor(formula, monitorId, connectedEval, subMonitor->getEvalCost(),
                        subMonitor->getSatisfactionProbability()),
          subMonitor(std::move(subMonitor)), interval(std::move(interval)){};

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
     * Resets monitor variables to default values.
     */
    void reset() override;

    /**
     * Creates string representation of monitor.
     *
     * @return String representing monitor.
     */
    std::string to_string() override;

    void updateDynamicInterval(size_t stepNumber) override;

  private:
    std::shared_ptr<Hydra2Monitor> subMonitor; //**< Child monitor. */
    std::optional<Verdict> currentVerdict;     //**< Stores previous verdict. */
    Hydra2IntervalTemporalOperator interval;   //**< Interval of operator. */
};
