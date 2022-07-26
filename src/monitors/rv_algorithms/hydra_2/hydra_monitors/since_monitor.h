//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include <utility>

#include "hydra_monitor.h"

class SinceMonitor : public HydraMonitor {
  public:
    /**
     *
     * @param formula Formula to be monitored.
     * @param subMonitorLeft Monitor from left side of until operator.
     * @param subMonitorRight Monitor from right side of until operator.
     * @param intervalFrom Start of time interval.
     * @param intervalTo End of time interval.
     * @param monitorId ID of monitor.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     */
    SinceMonitor(const Formula &formula, std::shared_ptr<HydraMonitor> subMonitorF,
                 std::shared_ptr<HydraMonitor> subMonitorG, size_t intervalFrom, size_t intervalTo, size_t monitorId,
                 bool connectedEval)
        : HydraMonitor(formula, monitorId, connectedEval, (subMonitorG->getEvalCost() + subMonitorF->getEvalCost()) / 2,
                       (subMonitorG->getSatisfactionProbability() + subMonitorG->getSatisfactionProbability()) / 2),
          subMonitorLeft(std::move(subMonitorF)), subMonitorRight(std::move(subMonitorG)), from(intervalFrom),
          to(intervalTo){};

    /**
     * Performs concrete step of since-monitor.
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
    std::shared_ptr<HydraMonitor> subMonitorLeft;  //**< Monitor for specification on left side of since. */
    std::shared_ptr<HydraMonitor> subMonitorRight; //**< Monitor for specification on right side of since. */
    std::optional<Verdict> subVerdictLeft;         //**< Verdict for monitor on left side of since. */
    std::optional<Verdict> subVerdictRight;        //**< Verdict for monitor on right side of since. */
    size_t from;                                   //**< Start of time interval. */
    size_t to;                                     //**< End of time interval. */
    HeadPos headPositionMonitorLeft;
    HeadPos headPositionMonitorRight;
    std::optional<HeadPos> lastSatisfaction; //**< Last time right side of until was satisfied. */
    std::optional<HeadPos> lastViolation; //**< Last time left side of until was violated. */
    size_t stepNumberRight{0}; // Custom step number for right monitor since left monitor is only called once
    bool endReached{false};
};
