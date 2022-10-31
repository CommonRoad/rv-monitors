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
 * Hydra monitor representing (strong) previous operator.
 */
class PreviousMonitorHydra : public HydraMonitor {
  public:
    /**
     * Constructor.
     *
     * @param formula Formula to be monitored.
     * @param subMonitor Child monitor.
     * @param interval  Interval of operator.
     */
    PreviousMonitorHydra(const Formula &formula, std::shared_ptr<HydraMonitor> subMonitor,
                         IntervalTemporalOperator interval)
        : HydraMonitor(formula), subMonitor(std::move(subMonitor)), interval(interval){};

    /**
     * Performs concrete step of previous operator monitor.
     *
     * @return Verdict for monitoring step result.
     */
    Verdict concreteStep() override;

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
    std::optional<Verdict> currentVerdict;    //**< Stores previous verdict. */
    IntervalTemporalOperator interval;        //**< Interval of operator. */
};
