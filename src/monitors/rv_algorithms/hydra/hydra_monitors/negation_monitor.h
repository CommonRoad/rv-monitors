//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include "monitors/rv_algorithms/hydra/hydra_monitors/hydra_monitor.h"

class NegationMonitor : public HydraMonitor {
  public:
    NegationMonitor(Formula formula, std::shared_ptr<HydraMonitor> subMonitor, size_t *maxId)
        : HydraMonitor(std::move(formula), maxId), subMonitor(std::move(subMonitor)){};

    Verdict step() override;

    void setTrace(const Trace &trace) override;

    void reset() override;

  private:
    std::shared_ptr<HydraMonitor> subMonitor;
};
