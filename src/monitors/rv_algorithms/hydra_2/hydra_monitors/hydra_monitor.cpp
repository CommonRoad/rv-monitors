#include "monitors/rv_algorithms/hydra_2/hydra_monitors/hydra_monitor.h"
#include <cassert>
#include <spdlog/spdlog.h>
#include <utility>

Hydra2Monitor::Hydra2Monitor(Formula formula, const size_t monitorId, const bool connectedEval, const double evalCost,
                             const double satisfactionProbability)
    : formula(std::move(formula)), monitorId(monitorId), connectedEval(connectedEval), evalCost(evalCost),
      satisfactionProbability(satisfactionProbability) {
    assert(evalCost >= 0 and evalCost <= 100 and satisfactionProbability >= 0 and satisfactionProbability <= 1 &&
           "Invalid predicate costs/probabilities Hydra2 Monitor.");
}

void Hydra2Monitor::reset() {
    verdictHistory = {};
    //  verdictHistoryPopCounter = 0;
    allCopies = {};
    for (auto &[_, id] : traceIdx)
        id = 0;
}

void Hydra2Monitor::registerParent(const size_t parentId) { traceIdx[parentId] = 0; }

size_t Hydra2Monitor::getMaxTraceIdx() {
    if (traceIdx.empty())
        return 0;
    return std::max_element(traceIdx.begin(), traceIdx.end(),
                            [](const std::pair<size_t, size_t> &elemA, const std::pair<size_t, size_t> &elemB) -> bool {
                                return elemA.second < elemB.second;
                            })
        ->second;
}

Verdict Hydra2Monitor::step(const size_t parentId, size_t stepNumber) {
    spdlog::debug("Hydra2Monitor::step: " + formula.getExpr()->getSymbol() + " - " + std::to_string(monitorId) + " - " +
                  subID + " - " + std::to_string(parentId) + " - " + std::to_string(stepNumber));
    auto verdict{Verdict(false)};
    if (traceIdx.size() > 1 and
        verdictHistory.find(stepNumber) != verdictHistory.end()) { // required step was already performed based on other
                                                                   // parent monitor, use already computed verdict
        verdict = verdictHistory.at(stepNumber);
    } else {
        verdict = concreteStep(parentId, stepNumber);
        if (traceIdx[parentId] == getMaxTraceIdx())
            verdictHistory.insert({stepNumber, verdict});
        ++traceIdx[parentId];
    }
    // note: verdict time step can be different from step number if time steps are no equidistant

    return verdict;
}

const std::string &Hydra2Monitor::getFormulaId() const { return formulaId; }

void Hydra2Monitor::setFormulaId(const std::string &formula_id) { formulaId = formula_id; }

double Hydra2Monitor::getEvalCost() const { return evalCost; }

double Hydra2Monitor::getSatisfactionProbability() const { return satisfactionProbability; }

std::map<size_t, std::vector<std::vector<DomainValue>>> Hydra2Monitor::getViolatingDomainValues() const { return {}; }

void Hydra2Monitor::setStepNumber(const size_t stepNumber, const size_t parentId) {
    if (stepNumber > traceIdx[parentId])
        traceIdx[parentId] = stepNumber;
}

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
        clonedMonitor->subID += "_" + val;
    allCopies.emplace_back(domValNames, clonedMonitor);
    return clonedMonitor;
}
bool Hydra2Monitor::stepNumberValid(const size_t stepNumber, const size_t parentId) {
    if (stepNumber < traceIdx[parentId])
        return false;
    return true;
}
