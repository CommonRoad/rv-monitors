//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include "monitors/rv_algorithms/hydra/hydra_monitors/hydra_monitor.h"
#include <memory>
#include <optional>
#include <utility>

class ConstantMonitor : public HydraMonitor {
  public:
    ConstantMonitor(Formula formula, size_t id, bool connectedEval)
        : HydraMonitor(std::move(formula), id, connectedEval){};

    Verdict concreteStep(size_t parentId) override;

    void setTrace(const Trace &trace) override;

    std::string to_string() override;

  private:
    Trace trace;
};
