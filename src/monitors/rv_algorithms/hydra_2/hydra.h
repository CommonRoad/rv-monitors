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
 * Class for monitoring a trace using the Hydra2 MTL monitor.
 */
class Hydra2 : public MonitorInterface {
  public:
    Hydra2(const std::vector<Formula> &formulas, DomainCollection domains = {}, FunctionCollection functions = {}, bool connectedEval = true, bool reordering = true,
          bool shortCircuiting = true, const std::map<std::string, std::array<double, 2>> &predicateCosts = {});

    /**
     * Evaluation function for monitor algorithms.
     *
     * @param trace Trace which should be evaluated.
     * @return List of verdicts for each time step of the trace. If the length of the vector is 1,
     * the runtime verification algorithm can only provide a global verdict for the complete trace.
     */
    std::map<std::string, std::vector<Verdict>> evaluate(Trace &trace) override;

    /**
     * Creates string representation for corresponding monitors.
     *
     * @return String representation of Hydra2.
     */
    [[nodiscard]] std::string to_string() const override;

  private:
    void createMonitor(const std::vector<Formula> &formulas, DomainCollection &domainCollection, FunctionCollection &functionCollection, bool connectedEval, bool reordering, bool shortCircuiting,
                       const std::map<std::string, std::array<double, 2>> &predicateCosts);

    /**
     * Resets monitor variables to default values.
     */
    void resetMonitors();

    /**
     * Setter for trace.
     *
     * @param tra Trace.
     */
    void setTraceMonitors(const Trace &tra);

    std::vector<std::shared_ptr<Hydra2Monitor>>
        monitors; //**< List of monitors corresponding to the formulas which should be evaluated. */
};
