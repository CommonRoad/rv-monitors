//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "hydra_monitor.h"

#include <utility>
const Formula &HydraMonitor::getFormula() const { return formula; }

HydraMonitor::HydraMonitor(Formula formula) : formula(std::move(formula)) {}

HydraMonitor::HydraMonitor(Formula formula, const size_t &from, const size_t &to,
                           const std::shared_ptr<HydraMonitor> &sub_monitor)
    : formula(std::move(formula)), from(from), to(to), subMonitor(sub_monitor) {}

const std::optional<size_t> &HydraMonitor::getFrom() const { return from; }

const std::optional<size_t> &HydraMonitor::getTo() const { return to; }

const std::optional<std::shared_ptr<HydraMonitor>> &HydraMonitor::getSubMonitor() const { return subMonitor; }

const std::optional<Verdict> &HydraMonitor::getCurrentVerdict() const { return currentVerdict; }
