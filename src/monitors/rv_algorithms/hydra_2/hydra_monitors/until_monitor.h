//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include <utility>

#include "hydra_monitor.h"

class UntilMonitor : public HydraMonitor {
  public:
    UntilMonitor(const Formula &formula, std::shared_ptr<HydraMonitor> subMonitorF,
                 std::shared_ptr<HydraMonitor> subMonitorG, std::shared_ptr<HydraMonitor> subMonitorH,
                 size_t intervalFrom, size_t intervalTo, size_t monitorId, bool connectedEval)
        : HydraMonitor(formula, monitorId, connectedEval, (subMonitorG->getEvalCost() + subMonitorF->getEvalCost()) / 2,
                       (subMonitorG->getSatisfactionProbability() + subMonitorG->getSatisfactionProbability()) / 2),
          subMonitorF(std::move(subMonitorF)), subMonitorG(std::move(subMonitorG)), subMonitorH(std::move(subMonitorH)),
          from(intervalFrom), to(intervalTo){};

    Verdict concreteStep(size_t parentId, size_t stepNumber) override;

    void setTrace(const Trace &tra) override;

    void setVariableValues(std::map<std::string, Value> values) override;

    void reset(std::optional<size_t> stepNumber = std::nullopt) override;

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
