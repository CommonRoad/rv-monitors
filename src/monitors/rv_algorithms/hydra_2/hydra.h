//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include "../../utils/utils.h"
#include "hydra_monitors/hydra_monitor.h"
#include "monitors/formula/formula.h"
#include "monitors/rv_algorithms/monitor_interface.h"
#include <monitors/trace/trace.h>

/**
 * Class for monitoring a trace using the Hydra MTL monitor.
 */
class Hydra : public MonitorInterface {
  public:
    Hydra(const std::vector<Formula> &formulas, bool connectedEval = true);

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
     * @return String representation of Hydra.
     */
    [[nodiscard]] std::string to_string() const override;

  private:
    void createMonitor(const std::vector<Formula> &formulas, bool connectedEval);
    void resetMonitors();
    void setTraceMonitors(const Trace &tr);

    std::vector<std::shared_ptr<HydraMonitor>> monitors;
};
