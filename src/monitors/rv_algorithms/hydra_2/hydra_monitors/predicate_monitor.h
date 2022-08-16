//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include "hydra_monitor.h"

class PredicateMonitor : public HydraMonitor {
  public:
    PredicateMonitor(Formula formula, size_t id, FunctionCollection &functions,
                     bool connectedEval, size_t evalCost = 0, double satisfactionProbability = 100.0)
        : HydraMonitor(std::move(formula), id, connectedEval, evalCost, satisfactionProbability),
          functions(functions) {};

    Verdict concreteStep(size_t parentId, size_t stepNumber) override;

    void setTrace(const Trace &trace) override;

    void setVariableValues(std::map<std::string, Value> values) override;

    void reset(std::optional<size_t> stepNumber = std::nullopt) override;

    std::string to_string() override;

  private:
    Trace trace;
    std::map<std::string, Value> variableValues;
    FunctionCollection functions;

};
