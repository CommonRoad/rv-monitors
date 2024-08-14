#include "monitors/rv_algorithms/hydra_2/hydra_monitors/hydra_monitor.h"
#include <cassert>
#include <iostream>
#include <spdlog/spdlog.h>
#include <utility>

Hydra2Monitor::Hydra2Monitor(Formula formula, size_t monitorId, bool connectedEval, double evalCost,
                             double satisfactionProbability)
    : formula(std::move(formula)), monitorId((monitorId)), connectedEval(connectedEval), evalCost(evalCost),
      satisfactionProbability(satisfactionProbability) {
    assert((evalCost >= 0 and evalCost <= 100 and satisfactionProbability >= 0 and satisfactionProbability <= 1) &&
           "Invalid predicate costs/probabilities Hydra2 Monitor.");
}

void Hydra2Monitor::reset() {
    verdictHistory = {};
    //  verdictHistoryPopCounter = 0;
    allCopies = {};
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

size_t Hydra2Monitor::getMinTraceIdx() {
    if (traceIdx.empty())
        return 0;
    else
        return std::min_element(
                   traceIdx.begin(), traceIdx.end(),
                   [](const std::pair<size_t, size_t> &elemA, const std::pair<size_t, size_t> &elemB) -> bool {
                       return elemA.second < elemB.second;
                   })
            ->second;
}

bool Hydra2Monitor::traceIdxUnique(size_t idx) {
    size_t counter{0};
    for (const auto &elem : traceIdx) {
        if (elem.second == idx)
            counter++;
        if (counter > 1)
            return false;
    }
    return true;
}

Verdict Hydra2Monitor::step(size_t parentId, size_t stepNumber, bool singleStep) {
    spdlog::debug("Hydra2Monitor::step: " + formula.getExpr()->getSymbol() + " - " + std::to_string(monitorId) + " - " +
                  subID + " - " + std::to_string(parentId) + " - " + std::to_string(stepNumber));
    Verdict verdict{Verdict(false)};
    if (!singleStep and (!connectedEval or traceIdx.size() == 1))
        if (stepNumber >= (traceIdx[parentId])) { // required step equals stored step, perform next step directly
            verdict = simulateVerdict(parentId, stepNumber);
        } else // step number is behind head position (e.g., if child of until operator) or specific verdict
               // is directly required (only used by until operator)
            verdict = concreteStep(parentId, stepNumber);
    else if (singleStep) { // step number is behind head position (e.g., if child of until operator) or specific verdict
                           // is directly required (only used by until operator)
        if (stepNumber >= traceIdx[parentId]) {
            verdict = simulateVerdict(parentId, stepNumber);
        } else { // if required step was already performed based on other parent monitor, use already computed verdict
            verdict = verdictHistory.at(traceIdx.at(parentId));
        }
    } else if (stepNumber < traceIdx[parentId]) { // required step was already performed based on other parent monitor,
                                                  // use already computed verdict
        verdict = verdictHistory.at(stepNumber);
    } else
        verdict = simulateVerdict(parentId, stepNumber);
    // note: verdict time step can be different from step number if time steps are no equidistant
    return verdict;
}

Verdict Hydra2Monitor::simulateVerdict(size_t parentId, size_t stepNumber) {
    auto verdict{Verdict(0, false)};
    while (stepNumber >= (traceIdx[parentId])) {
        if (!connectedEval or traceIdx.size() == 1) { // since no connected evaluation, perform step directly
            verdict = concreteStep(parentId, traceIdx[parentId]);
            ++traceIdx[parentId];
        } else if (traceIdx[parentId] == getMaxTraceIdx() or
                   verdictHistory.find(traceIdx[parentId]) ==
                       verdictHistory
                           .end()) { // if requested trace index equals maximum trace index based on all monitor heads
                                     // based on parent monitors, perform new step and store verdict
            verdict = concreteStep(parentId, traceIdx[parentId]);
            verdictHistory.insert({traceIdx[parentId], verdict});
            ++traceIdx[parentId];
        } else { // if required step was already performed based on other parent monitor, use already computed verdict
            verdict = verdictHistory.at(traceIdx[parentId]);
            auto minIdx{getMinTraceIdx()};
            if (traceIdx[parentId] == minIdx and
                traceIdxUnique(
                    minIdx)) { // if current monitor uses minimum trace index and no other monitor has same trace index
                               // -> pop head of verdict history and decrement indices of all other monitors
                spdlog::debug("Hydra2Monitor::simulateVerdict: Erase verdict from history " +
                              formula.getExpr()->getSymbol() + " - " + std::to_string(parentId) + " - " +
                              std::to_string(stepNumber));
                verdictHistory.erase(verdictHistory.begin());
                ++traceIdx[parentId];
            } else
                ++traceIdx[parentId];
        }
    }
    // note: verdict time step can be different from step number if time steps are no equidistant
    return verdict;
}

const std::string &Hydra2Monitor::getFormulaId() const { return formulaId; }

void Hydra2Monitor::setFormulaId(const std::string &formula_id) { formulaId = formula_id; }

double Hydra2Monitor::getEvalCost() const { return evalCost; }

double Hydra2Monitor::getSatisfactionProbability() const { return satisfactionProbability; }

std::map<size_t, std::vector<std::vector<DomainValue>>> Hydra2Monitor::getViolatingDomainValues() const { return {}; }

void Hydra2Monitor::setStepNumber(size_t stepNumber, size_t parentId) { traceIdx[parentId] = stepNumber; }

void Hydra2Monitor::to_tree(std::map<size_t, std::tuple<std::string, std::vector<size_t>>> &tree) {
    if (tree.find(monitorId) != tree.end())
        return;
    tree.insert({monitorId, {formula.getExpr()->to_string(), {}}});
}

size_t Hydra2Monitor::getMonitorID() const { return monitorId; }

std::shared_ptr<Hydra2Monitor> Hydra2Monitor::clone(const std::vector<std::string> &domValNames) {
    if (domValNames.empty() and allCopies.empty())
        return nullptr;
    if (domValNames.empty() and allCopies.size() == 1 and std::get<0>(allCopies.at(0)).empty())
        return std::get<1>(allCopies.at(0));
    for (const auto &copy : allCopies) {
        if (domValNames.size() != std::get<0>(copy).size())
            continue;
        size_t counter{0};
        for (size_t idx{0}; idx < domValNames.size(); ++idx)
            if (std::get<0>(copy)[idx] == domValNames[idx])
                ++counter;
        if (counter == domValNames.size())
            return std::get<1>(copy);
    }
    return nullptr;
}

std::shared_ptr<Hydra2Monitor> Hydra2Monitor::resetClonedMonitor(std::shared_ptr<Hydra2Monitor> clonedMonitor,
                                                                 const std::vector<std::string> &domValNames) {
    clonedMonitor->allCopies.clear();
    for (const auto &val : domValNames)
        clonedMonitor->subID += ("_" + val);
    allCopies.emplace_back(domValNames, clonedMonitor);
    return clonedMonitor;
}
