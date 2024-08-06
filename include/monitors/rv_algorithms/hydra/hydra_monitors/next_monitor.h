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
 * Monitor representing (strong) next Hydra operator.
 */
class NextMonitorHydra : public HydraMonitor {
  public:
    /**
     * Constructor
     *
     * @param formula Formula to be monitored.
     * @param subMonitor Child monitor.
     * @param interval  Interval of operator.
     */
    NextMonitorHydra(const Formula &formula, std::shared_ptr<HydraMonitor> subMonitor,
                     IntervalTemporalOperator interval)
        : HydraMonitor(formula), subMonitor(std::move(subMonitor)), interval(interval){};

    /**
     * Performs concrete step of next operator monitor.
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
    std::shared_ptr<HydraMonitor> subMonitor; //**< Child monitor. */
    IntervalTemporalOperator interval;        //**< Interval of operator. */
    std::optional<size_t> currentTime;        //**< Previous time step to check whether next verdict lies in interval.
    // Initialized after first event has been seen. */
};
