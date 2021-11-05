//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include <utility>

#include "monitors/rv_algorithms/hydra_2/hydra_monitors/hydra_monitor.h"

class SinceMonitor : public HydraMonitor {
  public:
    SinceMonitor(const Formula &formula, std::shared_ptr<HydraMonitor> subMonitorF,
                 std::shared_ptr<HydraMonitor> subMonitorG, size_t from, size_t to, size_t id, bool connectedEval)
        : HydraMonitor(formula, id), subMonitorF(std::move(subMonitorF)), subMonitorG(std::move(subMonitorG)),
          from(from), to(to){};

    Verdict concreteStep(size_t parentId, size_t stepNumber) override;

    void setTrace(const Trace &tr) override;

    void reset() override;

    std::string to_string() override;

  private:
    std::shared_ptr<HydraMonitor> subMonitorF, subMonitorG;
    std::optional<Verdict> subVerdictF, subVerdictG;
    size_t from, to;
    HeadPos positionMonitorF, positionMonitorG;
    std::optional<std::pair<size_t, int>> lastSatisfaction;
    std::optional<std::pair<size_t, int>> lastViolation;
};
