//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "or_monitor.h"
#include <iostream>

Verdict OrMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    Verdict verdict{subMonitors.at(0)->step(monitorId, stepNumber)};
    if (shortCircuiting) {
        if (verdict.value) {
            if (!verdict.timeStamp.has_value()) // happens when we are behind last trace element with submonitor
                return {true};
            else
                return {verdict.timeStamp.value(), true};
        }
        for (size_t i{1}; i < subMonitors.size(); ++i) { // verdict of first monitor is evaluated above
            if (subMonitors.at(i)->step(monitorId, stepNumber).value) {
                if (!verdict.timeStamp.has_value())
                    return {true};
                else
                    return {verdict.timeStamp.value(), true};
            }
        }
    } else {                                           // if no short-circuiting, check all verdicts of submonitors
        for (size_t i{1}; i < subMonitors.size(); ++i) // verdict of first monitor is evaluated above
            verdict = verdict || subMonitors.at(i)->step(monitorId, stepNumber);
    }
    return verdict;
}

void OrMonitor::setTrace(const Trace &tra) {
    for (const auto &mon : subMonitors)
        mon->setTrace(tra);
}
void OrMonitor::reset() {
    HydraMonitor::reset();
    for (const auto &mon : subMonitors)
        mon->reset();
}

std::string OrMonitor::to_string() {
    std::string str{"("};
    for (const auto &mon : subMonitors)
        str += mon->to_string() + " | ";
    str.erase(str.size() - 3);
    str += ")";
    return str;
}

double OrMonitor::computeEvaluationCost(const std::vector<std::shared_ptr<HydraMonitor>> &monitors) {
    double cost{monitors.at(0)->getEvalCost()};
    for (size_t idx{0}; idx < monitors.size(); ++idx)
        cost += monitors.at(idx)->getEvalCost() * monitors.at(idx)->getSatisfactionProbability();
    return cost / static_cast<double>(monitors.size()); // normalize cost
}

double OrMonitor::computeSatisfactionProbability(const std::vector<std::shared_ptr<HydraMonitor>> &monitors) {
    std::set<double> probs;
    for (const auto &mon : monitors) {
        probs.insert(mon->getSatisfactionProbability());
    }
    return *probs.rbegin();
}
