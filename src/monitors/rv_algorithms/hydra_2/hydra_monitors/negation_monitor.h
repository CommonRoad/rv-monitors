//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include "hydra_monitor.h"

class NegationMonitor : public HydraMonitor {
  public:
    NegationMonitor(Formula formula, std::shared_ptr<HydraMonitor> subMonitor, size_t monitorId, bool connectedEval)
        : HydraMonitor(std::move(formula), monitorId, connectedEval, subMonitor->getEvalCost(),
                       1 - subMonitor->getSatisfactionProbability()),
          subMonitor(std::move(subMonitor)){};

    Verdict concreteStep(size_t parentId, size_t stepNumber) override;

    void setTrace(const Trace &trace) override;

    void reset() override;

    std::string to_string() override;

  private:
    std::shared_ptr<HydraMonitor> subMonitor;
};
