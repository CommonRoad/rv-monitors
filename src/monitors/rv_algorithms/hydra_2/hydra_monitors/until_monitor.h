//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include <utility>

#include "monitors/rv_algorithms/hydra_2/hydra_monitors/hydra_monitor.h"

class UntilMonitor : public HydraMonitor {
  public:
    UntilMonitor(const Formula &formula, std::shared_ptr<HydraMonitor> subMonitorF,
                 std::shared_ptr<HydraMonitor> subMonitorG, std::shared_ptr<HydraMonitor> subMonitorH, size_t from,
                 size_t to, size_t id, bool connectedEval)
        : HydraMonitor(formula, id, connectedEval), subMonitorF(std::move(subMonitorF)),
          subMonitorG(std::move(subMonitorG)), subMonitorH(std::move(subMonitorH)), from(from), to(to){};

    Verdict concreteStep(size_t parentId, size_t stepNumber) override;

    void setTrace(const Trace &tr) override;

    void reset() override;

    std::string to_string() override;

  private:
    void readFG(size_t stepNumber);
    void handleEof();

    std::shared_ptr<HydraMonitor> subMonitorF, subMonitorG, subMonitorH;
    std::optional<Verdict> subVerdictF, subVerdictG, subVerdictH;
    size_t from, to;
    HeadPos positionMonitorF, positionMonitorG, positionMonitorH;
    std::optional<std::pair<size_t, int>> lastSatisfaction;
    std::optional<std::pair<size_t, int>> lastViolation;
};
