//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include <utility>

#include "hydra_monitor.h"

class NextMonitor : public HydraMonitor {
  public:
    NextMonitor(const Formula &formula, std::shared_ptr<HydraMonitor> subMonitor, size_t intervalFrom,
                size_t intervalTo, size_t monitorId, bool connectedEval)
        : HydraMonitor(formula, monitorId, connectedEval, subMonitor->getEvalCost(),
                       subMonitor->getSatisfactionProbability()),
          subMonitor(std::move(subMonitor)), from(intervalFrom), to(intervalTo){};

    Verdict concreteStep(size_t parentId, size_t stepNumber) override;

    void setTrace(const Trace &tra) override;

    void setVariableValues(std::map<std::string, Value> values) override;

    void reset(std::optional<size_t> stepNumber = std::nullopt) override;

    std::string to_string() override;

  private:
    std::shared_ptr<HydraMonitor> subMonitor;
    size_t from, to;
    std::optional<size_t> currentTime;
};
