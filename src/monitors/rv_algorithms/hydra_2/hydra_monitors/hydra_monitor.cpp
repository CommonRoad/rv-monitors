//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "hydra_monitor.h"
#include <cassert>
#include <iostream>
#include <utility>

Hydra2Monitor::Hydra2Monitor(Formula formula, size_t monitorId, bool connectedEval, double evalCost,
                             double satisfactionProbability)
    : formula(std::move(formula)), monitorId((monitorId)), connectedEval(connectedEval), evalCost(evalCost),
      satisfactionProbability(satisfactionProbability) {
    assert(evalCost >= 0 or evalCost <= 100 or satisfactionProbability >= 0 or satisfactionProbability <= 1);
}

void Hydra2Monitor::reset() {
    verdictHistory = {};
    for (auto &[_, id] : traceIdx)
        id = 0;
}

void Hydra2Monitor::registerParent(size_t parentId) { traceIdx[parentId] = 0; }

size_t Hydra2Monitor::getMaxTraceIdx() {
    if (traceIdx.empty())
        return 0;
    else
        return std::max_element(
                   traceIdx.begin(), traceIdx.end(),
                   [](const std::pair<size_t, size_t> &elemA, const std::pair<size_t, size_t> &elemB) -> bool {
                       return elemA.second < elemB.second;
                   })
            ->second;
}

Verdict Hydra2Monitor::step(size_t parentId, size_t stepNumber, bool singleStep) {
    Verdict verdict{Verdict(false)};
    if (!connectedEval)
        if (stepNumber >= traceIdx[parentId]) { // required step equals stored step, perform next step directly
            verdict = simulateVerdict(parentId, stepNumber);
        } else // step number is behind head position (e.g., if child of until operator) or specific verdict
               // is directly required (only used by until operator)
            verdict = concreteStep(parentId, stepNumber);
    else if (singleStep) { // step number is behind head position (e.g., if child of until operator) or specific verdict
                           // is directly required (only used by until operator)
        if (stepNumber >= traceIdx[parentId]) {
            verdict = simulateVerdict(parentId, stepNumber);
        } else { // if required step was already performed based on other parent monitor, use already computed verdict
            verdict = verdictHistory[stepNumber];
        }
    } else if (stepNumber < traceIdx[parentId]) { // required step was already performed based on other parent monitor,
                                                  // use already computed verdict
        verdict = verdictHistory[stepNumber];
    } else
        verdict = simulateVerdict(parentId, stepNumber);
    return verdict;
}

Verdict Hydra2Monitor::simulateVerdict(size_t parentId, size_t stepNumber) {
    auto verdict{Verdict(0, false)};
    while (stepNumber >= traceIdx[parentId]) {
        if (!connectedEval) { // since no connected evaluation, perform step directly
            verdict = concreteStep(parentId, traceIdx[parentId]);
            ++traceIdx[parentId];
        } else if (traceIdx[parentId] ==
                   getMaxTraceIdx()) { // if requested trace index equals maximum trace index based on all monitor heads
                                       // based on parent monitors, perform new step and store verdict
            verdict = concreteStep(parentId, traceIdx[parentId]);
            verdictHistory.push_back(verdict);
            ++traceIdx[parentId];
        } else { // if required step was already performed based on other parent monitor, use already computed verdict
            verdict = verdictHistory[traceIdx[parentId]];
            ++traceIdx[parentId];
        }
    }
    return verdict;
}
const std::string &Hydra2Monitor::getFormulaId() const { return formulaId; }

void Hydra2Monitor::setFormulaId(const std::string &formula_id) { formulaId = formula_id; }

double Hydra2Monitor::getEvalCost() const { return evalCost; }

double Hydra2Monitor::getSatisfactionProbability() const { return satisfactionProbability; }
