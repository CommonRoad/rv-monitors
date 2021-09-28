//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include "monitors/rv_algorithms/hydra/hydra_monitors/hydra_monitor.h"

class AndMonitor : public HydraMonitor {
  public:
    AndMonitor(Formula formula, std::vector<std::shared_ptr<HydraMonitor>> subMonitors)
        : HydraMonitor(std::move(formula)), subMonitors(std::move(subMonitors)){};

    Verdict step() override;

    void setTrace(const Trace &trace) override;

  private:
    std::vector<std::shared_ptr<HydraMonitor>> subMonitors;
};
