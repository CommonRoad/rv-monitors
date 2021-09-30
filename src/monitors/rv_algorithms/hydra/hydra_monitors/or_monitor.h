//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include "monitors/rv_algorithms/hydra/hydra_monitors/hydra_monitor.h"

class OrMonitor : public HydraMonitor {
  public:
    OrMonitor(Formula formula, std::vector<std::shared_ptr<HydraMonitor>> subMonitors, size_t *maxId)
        : HydraMonitor(std::move(formula), maxId), subMonitors(std::move(subMonitors)){};

    Verdict concreteStep(size_t parentId) override;

    void setTrace(const Trace &trace) override;

    void reset() override;

  private:
    std::vector<std::shared_ptr<HydraMonitor>> subMonitors;
};
