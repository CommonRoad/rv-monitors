//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include "monitors/rv_algorithms/hydra/hydra_monitors/hydra_monitor.h"

/**
 * Hydra monitor representing atomic proposition.
 */
class AtomicMonitorHydra : public HydraMonitor {
  public:
    /**
     * Constructor.
     *
     * @param formula Formula to be monitored.
     */
    AtomicMonitorHydra(Formula formula) : HydraMonitor(std::move(formula)){};

    /**
     * Performs concrete step of atomic monitor. It evaluates the trace element of the concrete step. Returns false if
     * traceIdx >= traceLength.
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
    size_t traceIdx{0};         //**< Index of current trace element. */
    std::optional<Trace> trace; //**< Trace which should be monitored. */
};
