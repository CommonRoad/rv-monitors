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
    AtomicMonitor(const Formula &formula, size_t id) : HydraMonitor(formula, id){};

    Verdict concreteStep(size_t parentId) override;

    void setTrace(const Trace &trace) override;

  private:
    Trace trace;
};
