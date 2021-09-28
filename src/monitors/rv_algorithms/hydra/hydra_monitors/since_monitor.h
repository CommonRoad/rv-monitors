//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include <utility>

#include "monitors/rv_algorithms/hydra/hydra_monitors/hydra_monitor.h"

class SinceMonitor : public HydraMonitor {
  public:
    SinceMonitor(const Formula &formula, std::shared_ptr<HydraMonitor> subMonitorF,
                 std::shared_ptr<HydraMonitor> subMonitorG, size_t from, size_t to)
        : HydraMonitor(formula), subMonitorF(std::move(subMonitorF)), subMonitorG(std::move(subMonitorG)), from(from),
          to(to){};

    Verdict step() override;

    void setTrace(const Trace &tr) override;

  private:
    std::shared_ptr<HydraMonitor> subMonitorF, subMonitorG;
    std::optional<Verdict> subVerdictF, subVerdictG;
    size_t from, to;
    HeadPos positionMonitorF, positionMonitorG;
    std::optional<std::pair<size_t, int>> last_sat;
    std::optional<std::pair<size_t, int>> last_viol;
};
