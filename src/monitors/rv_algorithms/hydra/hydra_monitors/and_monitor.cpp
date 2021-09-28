//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "and_monitor.h"

Verdict AndMonitor::step() {
    Verdict verdict{subMonitors.at(0)->step()};
    for (size_t i{1}; i < subMonitors.size(); ++i)
        verdict = verdict && subMonitors.at(i)->step();
    return verdict;
}

void AndMonitor::setTrace(const Trace &tr) {
    for (const auto &mon : subMonitors)
        mon->setTrace(tr);
}
