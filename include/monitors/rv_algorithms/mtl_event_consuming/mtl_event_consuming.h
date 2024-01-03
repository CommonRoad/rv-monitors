//
// Created by Sebastian Maierhofer and Evald Nexhipi.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include "monitors/formula/formula.h"
#include "monitors/rv_algorithms/monitor_interface.h"
#include <monitors/trace/trace.h>

/**
 * Functions for monitoring a trace using the MTL event consuming algorithm.
 */
class MTLEventConsuming : public MonitorInterface {
  public:
    /** Constructor.
     *
     * @param formulas List of formulas to monitor.
     */
    MTLEventConsuming(const std::vector<Formula> &formulas);

    /**
     * Evaluation function for monitor algorithms.
     *
     * @param trace Trace which should be evaluated.
     * @return List of verdicts for each time step of the trace. If the length of the vector is 1,
     * the runtime verification algorithm can only provide a global verdict for the complete trace.
     */
    std::map<std::string, std::vector<Verdict>> evaluate(Trace &trace) override;

    /**
     * to_string() function.
     * @return String representation of the BTT-FSM.
     */
    [[nodiscard]] std::string to_string() const override;
};