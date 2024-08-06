//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include "../../../utils/interval.h"
#include "monitors/rv_algorithms/hydra/hydra_monitors/hydra_monitor.h"
#include <utility>

/**
 * Hydra monitor for representing since operator.
 */
class SinceMonitorHydra : public HydraMonitor {
  public:
    /**
     *
     * @param formula Formula to be monitored.
     * @param subMonitorF Monitor from left side of until operator.
     * @param subMonitorG Monitor from right side of until operator.
     * @param interval  Interval of operator.
     */
    SinceMonitorHydra(const Formula &formula, std::shared_ptr<HydraMonitor> subMonitorF,
                      std::shared_ptr<HydraMonitor> subMonitorG, IntervalTemporalOperator interval)
        : HydraMonitor(formula), subMonitorF(std::move(subMonitorF)), subMonitorG(std::move(subMonitorG)),
          interval(interval){};

    /**
     * Performs concrete step of since-monitor.
     *
     * @return Verdict for monitoring step result.
     */
    Verdict concreteStep() override;

    /**
     * Setter for trace.
     *
     * @param trace Trace.
     */
    void setTrace(const std::shared_ptr<Trace> &tra) override;

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
    std::shared_ptr<HydraMonitor> subMonitorF;       //**< Monitor for specification on left side of since. */
    std::shared_ptr<HydraMonitor> subMonitorG;       //**< Monitor for specification on right side of since. */
    std::optional<Verdict> subVerdictF;              //**< Verdict for monitor on left side of since. */
    std::optional<Verdict> subVerdictG;              //**< Verdict for monitor on right side of since. */
    IntervalTemporalOperator interval;               //**< Interval of operator. */
    HeadPosHydra positionMonitorF;                   //**< Head position of left monitor. */
    HeadPosHydra positionMonitorG;                   //**< Head position of right monitor. *
    std::optional<std::pair<size_t, int>> last_sat;  //**< Last time right side of until was satisfied. */
    std::optional<std::pair<size_t, int>> last_viol; //**< Last time left side of until was violated. */
};
