//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include "monitors/rv_algorithms/hydra/hydra_monitors/hydra_monitor.h"

class PredicateMonitor : public HydraMonitor {
  public:
    PredicateMonitor(Formula formula, size_t id) : HydraMonitor(std::move(formula), id) {};

    Verdict concreteStep(size_t parentId) override;

    void setTrace(const Trace &trace) override;

  private:
    std::map<size_t, size_t> traceIdx;
    Trace trace;
};
