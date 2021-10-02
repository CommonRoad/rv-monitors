//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include <utility>

#include "monitors/rv_algorithms/hydra/hydra_monitors/hydra_monitor.h"

class NextMonitor : public HydraMonitor {
  public:
    NextMonitor(const Formula &formula, std::shared_ptr<HydraMonitor> subMonitor, size_t from, size_t to, size_t id, bool connectedEval)
        : HydraMonitor(formula, id, connectedEval), subMonitor(std::move(subMonitor)), from(from), to(to){};

    Verdict concreteStep(size_t parentId) override;

    void setTrace(const Trace &tr) override;

    void reset() override;

  private:
    std::shared_ptr<HydraMonitor> subMonitor;
    size_t from, to;
    std::optional<size_t> currentTime;
};
