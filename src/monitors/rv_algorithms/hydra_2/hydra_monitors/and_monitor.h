//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once
#include "hydra_monitor.h"

/**
 * Hydra monitor representing and-operator.
 */
class AndMonitor : public HydraMonitor {
  public:
    /**
     * Constructor of and-monitor.
     *
     * @param formula Formula to be monitored.
     * @param subMonitors Child monitors.
     * @param monitorId ID of monitor.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     * @param shortCircuiting Boolean indicating whether short circuiting should be used.
     */
    AndMonitor(Formula formula, const std::vector<std::shared_ptr<HydraMonitor>> &subMonitors, size_t monitorId,
               bool connectedEval, bool shortCircuiting)
        : HydraMonitor(std::move(formula), monitorId, connectedEval, computeCost(subMonitors),
                       computeSatisfactionProbability(subMonitors)),
          subMonitors(subMonitors), shortCircuiting(shortCircuiting){};

    /**
     * Performs concrete step of and-monitor.
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
    bool shortCircuiting; //**< Boolean indicating whether short-circuiting should be used. */

    /**
     * Computes normalized cost over all provided monitors.
     *
     * @param monitors List of monitors which should be used for predicate evaluation cost computation.
     * @return Normalized evaluation cost of or-monitor in interval [0, 100]
     */
    static double computeCost(const std::vector<std::shared_ptr<HydraMonitor>> &monitors);

    /**
     * Computest satisfaction probability over all provided monitors.
     *
     * @param monitors List of monitors which should be used for predicate satisfaction probability computation.
     * @return Satisfaction probability of or-monitor in interval [0, 1]
     */
    static double computeSatisfactionProbability(const std::vector<std::shared_ptr<HydraMonitor>> &monitors);
};
