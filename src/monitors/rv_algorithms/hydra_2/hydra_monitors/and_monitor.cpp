//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "and_monitor.h"
#include <iostream>

Verdict AndMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    Verdict verdict{subMonitors.at(0)->step(monitorId, stepNumber)};
    if (!verdict.b) {
        if (!verdict.ts.has_value())
            return Verdict(false);
        else
            return Verdict(verdict.ts.value(), false);
    }
    for (size_t i{1}; i < subMonitors.size(); ++i)
        if (!subMonitors.at(i)->step(monitorId, stepNumber).b) {
            if (!verdict.ts.has_value())
                return Verdict(false);
            else
                return Verdict(verdict.ts.value(), false);
        }
    return verdict;
}

void AndMonitor::setTrace(const Trace &tr) {
    for (const auto &mon : subMonitors)
        mon->setTrace(tr);
}

void AndMonitor::reset() {
    HydraMonitor::reset();
    for (const auto &mon : subMonitors)
        mon->reset();
}
std::string AndMonitor::to_string() {
    std::string s;
    for (const auto &mon : subMonitors)
        s += mon->to_string() + " and ";
    s.erase(s.size() - 5);
    return s;
}
