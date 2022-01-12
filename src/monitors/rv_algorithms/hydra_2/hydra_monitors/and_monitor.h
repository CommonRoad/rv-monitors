//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include "monitors/rv_algorithms/hydra_2/hydra_monitors/hydra_monitor.h"

class AndMonitor : public HydraMonitor {
  public:
    AndMonitor(Formula formula, std::vector<std::shared_ptr<HydraMonitor>> subMonitors, size_t id, bool connectedEval,
               bool shortCircuiting)
        : HydraMonitor(std::move(formula), id, connectedEval, computeCost(subMonitors),
                       computeSatisfactionProbability(subMonitors)),
          subMonitors(subMonitors), shortCircuiting(shortCircuiting){};

    Verdict concreteStep(size_t parentId, size_t stepNumber) override;

    void setTrace(const Trace &trace) override;

    void reset() override;

    std::string to_string() override;

  private:
    std::vector<std::shared_ptr<HydraMonitor>> subMonitors;
    bool shortCircuiting;

    static double computeCost(const std::vector<std::shared_ptr<HydraMonitor>> &monitors);

    static double computeSatisfactionProbability(const std::vector<std::shared_ptr<HydraMonitor>> &monitors);
};
