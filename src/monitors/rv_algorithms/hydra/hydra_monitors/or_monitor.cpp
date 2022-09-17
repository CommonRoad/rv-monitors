//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "or_monitor.h"

Verdict OrMonitorHydra::concreteStep() {
    Verdict verdict{subMonitors.at(0)->concreteStep()};
    for (size_t i{1}; i < subMonitors.size(); ++i)
        verdict = verdict || subMonitors.at(i)->concreteStep();
    return verdict;
}

void OrMonitorHydra::setTrace(const Trace &tra) {
    for (const auto &mon : subMonitors)
        mon->setTrace(tra);
}
void OrMonitorHydra::reset() {
    for (const auto &mon : subMonitors)
        mon->reset();
}

std::string OrMonitorHydra::to_string() {
    std::string str{"("};
    for (const auto &mon : subMonitors)
        str += mon->to_string() + " | ";
    str.erase(str.size() - 3);
    str += ")";
    return str;
}
