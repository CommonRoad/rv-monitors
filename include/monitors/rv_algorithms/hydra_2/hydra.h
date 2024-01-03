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
    /**
     * Constructor
     * @param formulas List of formulas which should be monitored.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     * @param reordering Boolean indicating whether the formula should be reordered based on the predicate costs.
     * @param shortCircuiting Boolean indicating whether short circuiting should be used.
     * @param earlyTermination Boolean indicating whether the complete trace has to be evaluated or whether the
     * execution can be terminated as soon as a global verdict for a monitor can be provided.
     * @param predicateCosts Cost assigned to predicates consisting of evaluation cost in interval [0, 100] and
     * satisfaction probability in interval [0, 1].
     */
    Hydra2(const std::vector<Formula> &formulas, bool connectedEval = true, bool reordering = true,
           bool shortCircuiting = true, bool earlyTermination = false,
           const std::vector<std::shared_ptr<DomainStorage>> &domains = {},
           const std::map<std::string, std::array<double, 2>> &predicateCosts = {});

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
     * @return String representation of Hydra.
     */
    [[nodiscard]] std::string to_string() const override;

    /**
     * Getter for domains.
     *
     * @return List of registered domains.
     */
    [[nodiscard]] std::vector<std::shared_ptr<DomainStorage>> &getDomains();

    /**
     * Getter for predicate cost.
     *
     * @return Registered predicate costs.
     */
    std::map<std::string, std::array<double, 2>> &getPredicateCost();

  private:
    /**
     * Creates monitors for specified formulas based on provided configuration information.
     *
     * @param formulas List of formulas which should be monitored.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     * @param reordering Boolean indicating whether the formula should be reordered based on the predicate costs.
     * @param shortCircuiting Boolean indicating whether short circuiting should be used.
     * @param predicateCosts Cost assigned to predicates consisting of evaluation cost in interval [0, 100] and
     * satisfaction probability in interval [0, 1].
     */
    void createMonitor(const std::vector<Formula> &formulas, bool connectedEval, bool reordering, bool shortCircuiting);

    /**
     * Resets monitor variables to default values.
     */
    void resetMonitors();

    /**
     * Setter for trace.
     *
     * @param tra Trace object.
     */
    void setTraceMonitors(const Trace &tra);

    /**
     * Evaluate complete trace.
     *
     * @param trace Trace to be evaluated.
     * @param result Data structure for storing evaluation result.
     */
    void evalCompleteTrace(Trace &trace, std::map<std::string, std::vector<Verdict>> &result);

    /**
     * Evaluates trace until verdict can be provided.
     *
     * @param trace Trace to be evaluated.
     * @param result Data structure for storing evaluation result.
     */
    void evalEarlyTermination(Trace &trace, std::map<std::string, std::vector<Verdict>> &result);

    std::vector<std::shared_ptr<Hydra2Monitor>>
        monitors; //**< List of monitors corresponding to the formulas which should be evaluated. */
    mutable std::vector<std::shared_ptr<DomainStorage>> domains;         //**< List of domains. */
    mutable std::map<std::string, std::array<double, 2>> predicateCosts; //**< Predicate costs. */
};
