//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "or_monitor.h"

Verdict OrMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    Verdict verdict{subMonitors.at(0)->step(monitorId, stepNumber)};
    if (shortCircuiting) {
        if (verdict.b) {
            if (!verdict.ts.has_value()) // happens when we are behind last trace element with submonitor
                return {true};
            else
                return {verdict.ts.value(), true};
        }
        auto current{subMonitors.at(0)->getTraceIdxByParentId(monitorId)};
        for (size_t i{1}; i < subMonitors.size(); ++i) {
            if (subMonitors.at(i)->step(monitorId, current - 1).b) {
                if (!verdict.ts.has_value())
                    return {true};
                else
                    return {verdict.ts.value(), true};
            }
        }
    } else {
        for (size_t i{1}; i < subMonitors.size(); ++i)
            verdict = verdict || subMonitors.at(i)->step(monitorId, stepNumber);
    }
    return verdict;
}

void OrMonitor::setTrace(const Trace &tra) {
    for (const auto &mon : subMonitors)
        mon->setTrace(tra);
}

void OrMonitor::setVariableValues(std::map<std::string, Value> values) {
    for (auto &mon : subMonitors)
        mon->setVariableValues(values);
}

void OrMonitor::reset(std::optional<size_t> stepNumber) {
    HydraMonitor::reset(stepNumber);
    for (const auto &mon : subMonitors)
        mon->reset(stepNumber);
}

std::string OrMonitor::to_string() {
    std::string str{"("};
    for (const auto &mon : subMonitors)
        str += mon->to_string() + " | ";
    str.erase(str.size() - 3);
    str += ")";
    return str;
}

double OrMonitor::computeCost(const std::vector<std::shared_ptr<HydraMonitor>> &monitors) {
    double cost{0.0};
    for (const auto &mon : monitors)
        cost += mon->getEvalCost();
    return cost / static_cast<double>(monitors.size());
}

double OrMonitor::computeSatisfactionProbability(const std::vector<std::shared_ptr<HydraMonitor>> &monitors) {
    double prob{monitors.at(0)->getSatisfactionProbability()};
    for (size_t idx{1}; idx < monitors.size(); ++idx)
        prob *= monitors.at(idx)->getSatisfactionProbability() / 10.0;
    return prob;
}
