//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include <utility>

#include "hydra_monitor.h"

/**
 * Hydra monitor representing (strong) previous operator.
 */
class PreviousMonitor : public HydraMonitor {
  public:
    /**
     * Constructor.
     *
     * @param formula Formula to be monitored.
     * @param subMonitor Child monitor.
     * @param intervalFrom Start of time interval.
     * @param intervalTo End of time interval.
     * @param monitorId ID of monitor.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     */
    PreviousMonitor(const Formula &formula, std::shared_ptr<HydraMonitor> subMonitor, size_t intervalFrom, size_t intervalTo, size_t monitorId,
                    bool connectedEval)
        : HydraMonitor(formula, monitorId, connectedEval, subMonitor->getEvalCost(), subMonitor->getSatisfactionProbability()),
          subMonitor(std::move(subMonitor)), from(intervalFrom), to(intervalTo){};

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

  private:
    std::shared_ptr<HydraMonitor> subMonitor; //**< Child monitor. */
    std::optional<Verdict> currentVerdict; //**< Stores previous verdict. */
    size_t from; //**< Start of time interval. */
    size_t to; //**< End of time interval. */
};
