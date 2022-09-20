//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include "hydra_monitor.h"

/**
 * Monitor representing negation Hydra2 operator.
 */
class NegationMonitor : public Hydra2Monitor {
  public:
    /**
     * Constructor.
     *
     * @param formula Formula to be monitored.
     * @param subMonitor Child monitor.
     * @param monitorId ID of monitor.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     */
    NegationMonitor(Formula formula, std::shared_ptr<Hydra2Monitor> subMonitor, size_t monitorId, bool connectedEval)
        : Hydra2Monitor(std::move(formula), monitorId, connectedEval, subMonitor->getEvalCost(),
                        1 - subMonitor->getSatisfactionProbability()),
          subMonitor(std::move(subMonitor)){};

    /**
     * Performs concrete step of negation operator monitor.
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
     * Setter for the current values of the quantified variables.
     *
     * @param values Current values.
     */
    void setVariableValues(std::map<std::string, Value> values) override;

    /**
     * Resets monitor variables to default values at a specified step number.
     *
     * @param stepNumber Optional step number.
     */
    void reset(std::optional<size_t> stepNumber = std::nullopt) override;

    /**
     * Clones this monitor.
     * @return New shared_ptr of this monitor.
     */
    std::shared_ptr<Hydra2Monitor> clone() override;

    /**
     * Creates string representation of monitor.
     *
     * @return String representing monitor.
     */
    std::string to_string() override;

  private:
    std::shared_ptr<Hydra2Monitor> subMonitor; //**< Child monitor. */
};
