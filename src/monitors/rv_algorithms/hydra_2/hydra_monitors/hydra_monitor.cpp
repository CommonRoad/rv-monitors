//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "hydra_monitor.h"

#include <iostream>
#include <utility>
const Formula &HydraMonitor::getFormula() const { return formula; }

HydraMonitor::HydraMonitor(Formula formula, size_t id, bool connectedEval)
    : formula(std::move(formula)), monitorId((id)), connectedEval(connectedEval) {}

void HydraMonitor::reset() {
    verdictHistory = {};
    for (auto &[_, id] : traceIdx)
        id = 0;
}

void HydraMonitor::registerParent(size_t parentId) { traceIdx[parentId] = 0; }

size_t HydraMonitor::getMaxTraceIdx() {
    if (traceIdx.empty())
        return 0;
    else
        return std::max_element(traceIdx.begin(), traceIdx.end(),
                                [](const std::pair<size_t, size_t> &a, const std::pair<size_t, size_t> &b) -> bool {
                                    return a.second < b.second;
                                })
            ->second;
}

Verdict HydraMonitor::step(size_t parentId, size_t stepNumber, bool ret) {
    auto verdict{Verdict(0, false)};
    if (stepNumber < traceIdx[parentId] or ret) {
        if (!connectedEval) {
            verdict = concreteStep(parentId, stepNumber);
            ++traceIdx[parentId];
        } else if (traceIdx[parentId] == getMaxTraceIdx()) {
            verdict = concreteStep(parentId, stepNumber);
            verdictHistory.push_back(verdict);
            ++traceIdx[parentId];
        } else {
            verdict = verdictHistory[traceIdx[parentId]];
            ++traceIdx[parentId];
        }
        return verdict;
    }
    while (stepNumber >= traceIdx[parentId]) {
        if (!connectedEval) {
            verdict = concreteStep(parentId, traceIdx[parentId]);
            ++traceIdx[parentId];
        } else if (traceIdx[parentId] == getMaxTraceIdx()) {
            verdict = concreteStep(parentId, traceIdx[parentId]);
            verdictHistory.push_back(verdict);
            ++traceIdx[parentId];
        } else {
            verdict = verdictHistory[traceIdx[parentId]];
            ++traceIdx[parentId];
        }
    }
    return verdict;
}
const std::string &HydraMonitor::getFormulaId() const { return formulaId; }

void HydraMonitor::setFormulaId(const std::string &formula_id) { formulaId = formula_id; }

//void HydraMonitor::updateIdx(size_t parentId){ ++traceIdx[parentId]; }
