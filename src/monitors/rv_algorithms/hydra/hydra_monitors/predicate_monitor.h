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

class PredicateMonitor : public HydraMonitor {
  public:
    /**
     *
     * @param formula
     */
    PredicateMonitor(Formula formula) : HydraMonitor(std::move(formula)){};

    /**
     *
     * @return
     */
    Verdict step() override;

    void setTrace(const Trace &trace) override;

  private:
    size_t traceIdx;
};
