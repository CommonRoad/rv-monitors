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

class PreviousMonitor : public HydraMonitor {
  public:
    /**
     *
     * @param formula
     * @param subMonitor
     * @param from
     * @param to
     */
    PreviousMonitor(const Formula &formula, const std::shared_ptr<HydraMonitor> &subMonitor, size_t from, size_t to)
        : HydraMonitor(formula, from, to, subMonitor){};

    /**
     *
     * @return
     */
    Verdict step() override;

    void setTrace(const Trace &tr) override;
};
