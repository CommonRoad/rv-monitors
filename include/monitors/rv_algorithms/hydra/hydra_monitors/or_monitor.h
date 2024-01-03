//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include "monitors/rv_algorithms/hydra/hydra_monitors/hydra_monitor.h"

/**
 * Hydra monitor representing or-operator.
 */
class OrMonitorHydra : public HydraMonitor {
  public:
    /**
     * Constructor of or-monitor.
     *
     * @param formula Formula to be monitored.
     * @param subMonitors Child monitors.
     */
    OrMonitorHydra(Formula formula, std::vector<std::shared_ptr<HydraMonitor>> subMonitors)
        : HydraMonitor(std::move(formula)), subMonitors(std::move(subMonitors)){};

    /**
     * Performs concrete step of or-monitor.
     *
     * @return Verdict for monitoring step result.
     */
    Verdict concreteStep() override;

    /**
     * Setter for trace.
     *
     * @param trace Trace.
     */
    void setTrace(const Trace &trace) override;

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
    std::vector<std::shared_ptr<HydraMonitor>> subMonitors; //**< Child monitors. */
};
