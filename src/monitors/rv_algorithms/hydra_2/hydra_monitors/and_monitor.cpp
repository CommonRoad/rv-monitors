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
            return {false};
        else
            return {verdict.ts.value(), false};
    }
    for (size_t i{1}; i < subMonitors.size(); ++i)
        if (!subMonitors.at(i)->step(monitorId, stepNumber).b) {
            if (!verdict.ts.has_value())
                return {false};
            else
                return {verdict.ts.value(), false};
        }
    return verdict;
}

void AndMonitor::setTrace(const Trace &tra) {
    for (const auto &mon : subMonitors)
        mon->setTrace(tra);
}

void AndMonitor::reset() {
    HydraMonitor::reset();
    for (const auto &mon : subMonitors)
        mon->reset();
}

std::string AndMonitor::to_string() {
    std::string str{"("};
    for (const auto &mon : subMonitors)
        str += mon->to_string() + " & ";
    str.erase(str.size() - 3);
    str += ")";
    return str;
}

double AndMonitor::computeCost(const std::vector<std::shared_ptr<HydraMonitor>> &monitors) {
    double cost{0.0};
    for (const auto &mon : monitors)
        cost += mon->getEvalCost();
    return cost / static_cast<double>(monitors.size());
}

double AndMonitor::computeSatisfactionProbability(const std::vector<std::shared_ptr<HydraMonitor>> &monitors) {
    double prob{monitors.at(0)->getSatisfactionProbability()};
    for (size_t idx{1}; idx < monitors.size(); ++idx)
        prob *= monitors.at(idx)->getSatisfactionProbability() / 10.0;
    return prob;
}
