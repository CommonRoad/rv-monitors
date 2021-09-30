//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "or_monitor.h"

Verdict OrMonitor::step(size_t parentId) {
    Verdict verdict{subMonitors.at(0)->step(id)};
    for (size_t i{1}; i < subMonitors.size(); ++i)
        verdict = verdict || subMonitors.at(i)->step(id);
    return verdict;
}

void OrMonitor::setTrace(const Trace &tr) {
    for (const auto &mon : subMonitors)
        mon->setTrace(tr);
}
void OrMonitor::reset() {
    for (const auto &mon : subMonitors)
        mon->reset();
}
