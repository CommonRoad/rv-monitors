//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "or_monitor.h"

<<<<<<< HEAD:src/monitors/rv_algorithms/hydra/hydra_monitors/or_monitor.cpp
Verdict OrMonitor::concreteStep(size_t parentId) {
    Verdict verdict{subMonitors.at(0)->step(monitorId)};
    for (size_t i{1}; i < subMonitors.size(); ++i)
        verdict = verdict || subMonitors.at(i)->step(monitorId);
=======
Verdict OrMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    Verdict verdict{subMonitors.at(0)->step(monitorId, stepNumber)};
    if (verdict.b) {
        if (!verdict.ts.has_value())
            return Verdict(true);
        else
            return Verdict(verdict.ts.value(), true);
    }
    for (size_t i{1}; i < subMonitors.size(); ++i)
        if (subMonitors.at(i)->step(monitorId, stepNumber).b) {
            if (!verdict.ts.has_value())
                return Verdict(true);
            else
                return Verdict(verdict.ts.value(), true);
        }
>>>>>>> refs/remotes/origin/hydra_2:src/monitors/rv_algorithms/hydra_2/hydra_monitors/or_monitor.cpp
    return verdict;
}

void OrMonitor::setTrace(const Trace &tr) {
    for (const auto &mon : subMonitors)
        mon->setTrace(tr);
}
void OrMonitor::reset() {
    HydraMonitor::reset();
    for (const auto &mon : subMonitors)
        mon->reset();
}
std::string OrMonitor::to_string() { return std::string(); }
