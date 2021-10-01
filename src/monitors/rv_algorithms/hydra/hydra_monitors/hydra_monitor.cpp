//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "hydra_monitor.h"

#include <utility>
const Formula &HydraMonitor::getFormula() const { return formula; }

HydraMonitor::HydraMonitor(Formula formula, size_t id) : formula(std::move(formula)), monitorId((id)) {}

void HydraMonitor::reset() {
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

Verdict HydraMonitor::step(size_t parentId) {
    if (traceIdx[parentId] == getMaxTraceIdx()) {
        auto verdict{concreteStep(parentId)};
        verdictHistory.push_back(verdict);
        ++traceIdx[parentId];
        return verdict;
    } else {
        auto verdict{verdictHistory[traceIdx[parentId]]};
        ++traceIdx[parentId];
        return verdict;
    }
}
const std::string &HydraMonitor::getFormulaId() const { return formulaId; }

void HydraMonitor::setFormulaId(const std::string &formula_id) { formulaId = formula_id; }
