//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include "monitors/rv_algorithms/hydra/hydra_monitors/hydra_monitor.h"

class NextMonitor : public HydraMonitor {
  public:
    NextMonitor(const Formula &formula, std::shared_ptr<HydraMonitor> subMonitor, size_t from, size_t to)
        : HydraMonitor(formula), subMonitor(subMonitor), from(from), to(to){};

    std::optional<Verdict> step() override;

    void setTrace(const Trace &tr) override;

  private:
    std::optional<std::shared_ptr<HydraMonitor>> subMonitor;
    std::optional<size_t> from, to, currentTime;
};
