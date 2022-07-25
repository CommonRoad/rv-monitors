//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include <utility>

#include "hydra_monitor.h"

class UntilMonitor : public HydraMonitor {
  public:
    /**
     *
     * @param formula Formula to be monitored.
     * @param subMonitorLeft Monitor from left side of until operator.
     * @param subMonitorRight Monitor from right side of until operator.
     * @param subMonitorTmp
     * @param intervalFrom Start of time interval.
     * @param intervalTo End of time interval.
     * @param monitorId ID of monitor.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     */
    UntilMonitor(const Formula &formula, std::shared_ptr<HydraMonitor> subMonitorLeft,
                 std::shared_ptr<HydraMonitor> subMonitorRight, std::shared_ptr<HydraMonitor> subMonitorTmp,
                 size_t intervalFrom, size_t intervalTo, size_t monitorId, bool connectedEval)
        : HydraMonitor(formula, monitorId, connectedEval, (subMonitorRight->getEvalCost() + subMonitorLeft->getEvalCost()) / 2,
                       (subMonitorRight->getSatisfactionProbability() + subMonitorRight->getSatisfactionProbability()) / 2),
          subMonitorLeft(std::move(subMonitorLeft)), subMonitorRight(std::move(subMonitorRight)),
          subMonitorTmp(std::move(subMonitorTmp)),
          from(intervalFrom), to(intervalTo){};

    /**
     * Performs concrete step of until-monitor.
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
    /**
     * Executes left and right monitors of until.
     *
     * @param stepNumber
     */
    void stepLeftRightMonitors(size_t stepNumber);

    std::shared_ptr<HydraMonitor> subMonitorLeft; //**< Monitor for specification on left side of until. */
    std::shared_ptr<HydraMonitor> subMonitorRight; //**< Monitor for specification on right side of until. */
    std::shared_ptr<HydraMonitor> subMonitorTmp;
    std::optional<Verdict> subVerdictLeft; //**< Verdict for monitor on left side of until. */
    std::optional<Verdict> subVerdictRight; //**< Verdict for monitor on right side of until. */
    std::optional<Verdict> subVerdictTmp;
    size_t from;  //**< Start of time interval. */
    size_t to; //**< End of time interval. */
    HeadPos headPositionMonitorLeft;
    HeadPos headPositionMonitorRight;
    HeadPos headPositionMonitorTmp;
    std::optional<HeadPos> lastSatisfaction; //**< Last time right side of until was satisfied (time step, trace idx). */
    std::optional<HeadPos> lastViolation; //**< Last time left side of until was violated (time step, trace idx). */
    bool endReached{false};
};
