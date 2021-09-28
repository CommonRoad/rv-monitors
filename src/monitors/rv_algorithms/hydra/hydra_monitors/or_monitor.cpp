//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "or_monitor.h"

Verdict OrMonitor::step() {
    Verdict verdict{subMonitors.at(0)->step()};
    if (verdict.b)
        return verdict;
    for (size_t i{1}; i < subMonitors.size(); ++i) {
        verdict = subMonitors.at(i)->step();
        if (verdict.b)
            return verdict;
    }
    return verdict;
}

void OrMonitor::setTrace(const Trace &tr) {
    for (const auto &mon : subMonitors)
        mon->setTrace(tr);
}
