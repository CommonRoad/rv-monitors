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
    PredicateMonitor(Formula formula, size_t id, bool connectedEval, size_t evalCost = 0,
                     double satisfactionProbability = 100.0)
        : HydraMonitor(std::move(formula), id, connectedEval, evalCost, satisfactionProbability){};

    Verdict concreteStep(size_t parentId, size_t stepNumber) override;

    void setTrace(const Trace &trace) override;

    std::string to_string() override;

  private:
    Trace trace;
};
