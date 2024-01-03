//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include <utility>

#include "../../../expression_tree/atomic/constant.h"
#include "../../../utils/interval.h"
#include "constant_monitor.h"
#include "hydra_monitor.h"

/**
 * Hydra2 monitor for representing until operator.
 */
class UntilMonitor : public Hydra2Monitor {
  public:
    /**
     *
     * @param formula Formula to be monitored.
     * @param subMonitorLeft Monitor from left side of until operator.
     * @param subMonitorRight Monitor from right side of until operator.
     * @param interval  Interval of operator.
     * @param monitorId ID of monitor.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     */
    UntilMonitor(const Formula &formula, std::shared_ptr<Hydra2Monitor> subMonitorLeft,
                 std::shared_ptr<Hydra2Monitor> subMonitorRight, IntervalTemporalOperator interval, size_t monitorId,
                 bool connectedEval)
        : Hydra2Monitor(
              formula, monitorId, connectedEval, (subMonitorRight->getEvalCost() + subMonitorLeft->getEvalCost()) / 2,
              (subMonitorRight->getSatisfactionProbability() + subMonitorRight->getSatisfactionProbability()) / 2),
          subMonitorLeft(std::move(subMonitorLeft)), subMonitorRight(std::move(subMonitorRight)),
          subMonitorBaseTimeStep(
              std::make_shared<ConstantMonitor>(Formula(std::make_shared<Constant>(true)), 0, connectedEval)),
          interval(interval){};

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
     * Calls sub-monitors to add domain value.
     *
     * @param vals Domain values which should be added.
     */
    void addDomainValues(const std::vector<DomainValue> &vals) override;

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
    /**
     * Executes left and right monitors of until.
     */
    void stepLeftRightMonitors();

    std::shared_ptr<Hydra2Monitor> subMonitorLeft;  //**< Monitor for specification on left side of until. */
    std::shared_ptr<Hydra2Monitor> subMonitorRight; //**< Monitor for specification on right side of until. */
    std::shared_ptr<Hydra2Monitor>
        subMonitorBaseTimeStep; //**< Monitor to extract time step and index from trace at position of until monitor
                                // head. Represented by Constant-True-Monitor. */
    std::optional<Verdict> subVerdictLeft;         //**< Verdict for monitor on left side of until. */
    std::optional<Verdict> subVerdictRight;        //**< Verdict for monitor on right side of until. */
    std::optional<Verdict> subVerdictBaseTimeStep; //**< Monitor to extract time step and index from trace at position
                                                   // of until monitor head. Represented by Constant-True-Monitor. */
    IntervalTemporalOperator interval;             //**< Interval of operator. */
    HeadPosHydra2 headPositionMonitorLeft;         //**< Head position of left monitor. */
    HeadPosHydra2 headPositionMonitorRight;        //**< Head position of right monitor. *
    HeadPosHydra2 headPositionMonitorBaseTimeStep; //**< Head position of base time monitor. *
    std::optional<HeadPosHydra2> lastSatisfaction; //**< Last time right side of until was satisfied. */
    std::optional<HeadPosHydra2> lastViolation;    //**< Last time left side of until was violated. */
    bool endReached{false};                        //**< Boolean indicating whether end of trace was reached. */
    size_t stepNumberUntil{0}; //**< Stores real step number because until operator can be before parent step number. */
};
