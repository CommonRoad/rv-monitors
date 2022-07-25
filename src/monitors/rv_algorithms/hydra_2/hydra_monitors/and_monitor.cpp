//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "and_monitor.h"
#include <iostream>

Verdict AndMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    auto tmp{subMonitors.at(0)->getTraceIdxByParentId(monitorId)};
    Verdict verdict{subMonitors.at(0)->step(monitorId, tmp)};
    if (shortCircuiting) {
        if (!verdict.b) {
            if (!verdict.ts.has_value()) // happens when we are behind last trace element with submonitor
                return {false};
            else
                return {verdict.ts.value(), false};
        }
        auto current{subMonitors.at(0)->getTraceIdxByParentId(monitorId) - 1};
        std::cout << tmp << " | " << current << " | " << stepNumber << '\n';
        for (size_t i{1}; i < subMonitors.size(); ++i) // verdict of first monitor is evaluated above
            if (!subMonitors.at(i)->step(monitorId, current).b) {
                if (!verdict.ts.has_value())
                    return {false};
                else
                    return {verdict.ts.value(), false};
            }
    } else {
        for (size_t i{1}; i < subMonitors.size(); ++i)
            verdict = verdict && subMonitors.at(i)->step(monitorId, stepNumber);
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
    double cost{monitors.at(0)->getEvalCost()};
    for (size_t idx{0}; idx < monitors.size(); ++idx)
        cost += monitors.at(idx)->getEvalCost() * (1 - monitors.at(idx)->getSatisfactionProbability());
    return cost / static_cast<double>(monitors.size()); // normalize value
}

double AndMonitor::computeSatisfactionProbability(const std::vector<std::shared_ptr<HydraMonitor>> &monitors) {
    double prob{monitors.at(0)->getSatisfactionProbability()};
    for (size_t idx{1}; idx < monitors.size(); ++idx)
        prob *= monitors.at(idx)->getSatisfactionProbability();
    return prob;
}
