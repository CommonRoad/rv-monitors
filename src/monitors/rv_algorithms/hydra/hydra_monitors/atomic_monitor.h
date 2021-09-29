//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include "monitors/rv_algorithms/hydra/hydra_monitors/hydra_monitor.h"

class AtomicMonitor : public HydraMonitor {
  public:
    AtomicMonitor(Formula formula) : HydraMonitor(std::move(formula)){};

    Verdict step() override;

    void setTrace(const Trace &trace) override;

    void reset() override;

  private:
    size_t traceIdx{0};
    std::optional<Trace> trace;
};
