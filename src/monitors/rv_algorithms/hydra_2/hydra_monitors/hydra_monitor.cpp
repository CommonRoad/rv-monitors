#include "monitors/rv_algorithms/hydra_2/hydra_monitors/hydra_monitor.h"

#include "monitors/rv_algorithms/hydra_2/hydra_monitors/atomic_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/constant_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/predicate_monitor.h"

#include <algorithm>
#include <cassert>
#include <spdlog/spdlog.h>
#include <utility>

Hydra2Monitor::Hydra2Monitor(Formula formula, const size_t monitorId, const double evalCost,
                             const double satisfactionProbability)
    : formula(std::move(formula)), monitorId(monitorId), evalCost(evalCost),
      satisfactionProbability(satisfactionProbability) {
    assert(evalCost >= 0 and satisfactionProbability >= 0 and satisfactionProbability <= 1 &&
           "Invalid predicate costs/probabilities Hydra2 Monitor.");
}

void Hydra2Monitor::reset() {
    verdictHistory = {};
    lastPruneStep = {};
    numEvaluations = 0;
    //  verdictHistoryPopCounter = 0;
    allCopies = {};
    for (auto &[_, id] : traceIdx)
        id = 0;
}

void Hydra2Monitor::registerParent(const size_t parentId) {
    traceIdx[parentId] = 0;
    ++numParents;
}

void Hydra2Monitor::configureVerdictStorage(const VerdictRetention &requested) {
    // A monitor that is reachable via several parents is configured once per path; it is only revisited if the new path
    // requires a longer retention.
    if (retention and retention->covers(requested))
        return;
    retention = retention ? VerdictRetention{std::max(retention->steps, requested.steps),
                                             std::max(retention->time, requested.time)}
                          : requested;
    if (numParents > 1)
        storeVerdicts = true;
    const auto retentionChildren{childRetention(*retention)};
    for (const auto &child : getChildren()) {
        if (requestsChildVerdictsRepeatedly())
            child->storeVerdicts = true;
        child->configureVerdictStorage(retentionChildren);
    }
}

void Hydra2Monitor::pruneVerdictHistory(const size_t stepNumber, const size_t timeStamp) {
    if (lastPruneStep == stepNumber)
        return;
    lastPruneStep = stepNumber;
    const auto keep{retention.value_or(VerdictRetention{})};
    // verdicts are ordered by step; the time stamps are non-decreasing with the step
    for (auto iter{verdictHistory.begin()}; iter != verdictHistory.end();) {
        const auto &[step, verdict]{*iter};
        if (step + keep.steps >= stepNumber or
            (verdict.timeStamp.has_value() and verdict.timeStamp.value() + keep.time >= timeStamp))
            break;
        iter = verdictHistory.erase(iter);
    }
    for (const auto &child : getChildren())
        child->pruneVerdictHistory(stepNumber, timeStamp);
}

size_t Hydra2Monitor::getVerdictHistorySize() const { return verdictHistory.size(); }

size_t Hydra2Monitor::getNumStoredVerdicts(std::set<const Hydra2Monitor *> &visited) const {
    if (!visited.insert(this).second)
        return 0;
    size_t numVerdicts{verdictHistory.size()};
    for (const auto &child : getChildren())
        numVerdicts += child->getNumStoredVerdicts(visited);
    return numVerdicts;
}

size_t Hydra2Monitor::getNumEvaluations(std::set<const Hydra2Monitor *> &visited) const {
    if (!visited.insert(this).second)
        return 0;
    size_t evaluations{numEvaluations};
    for (const auto &child : getChildren())
        evaluations += child->getNumEvaluations(visited);
    return evaluations;
}

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
    // The verdict at a step only depends on the trace and the domain values of the monitor, so the evaluation is
    // independent of which steps were skipped by short-circuiting and of how many parents request the step. Verdicts
    // that can be requested several times are stored, so they are computed at most once (as long as they are not
    // pruned).
    if (const auto iter{verdictHistory.find(stepNumber)}; iter != verdictHistory.end())
        verdict = iter->second;
    else {
        verdict = concreteStep(parentId, stepNumber);
        ++numEvaluations;
        if (storeVerdicts)
            verdictHistory.insert({stepNumber, verdict});
    }
    if (stepNumber + 1 > traceIdx[parentId])
        traceIdx[parentId] = stepNumber + 1;
    // note: verdict time step can be different from step number if time steps are no equidistant
    if (const auto tracePtr = trace.lock(); verdict.timeStamp.has_value() && !dynamic_cast<PredicateMonitor *>(this) &&
                                            !dynamic_cast<ConstantMonitor *>(this) &&
                                            !dynamic_cast<AtomicMonitor *>(this) && tracePtr &&
                                            tracePtr->getTraceRef().size() > stepNumber) {
        tracePtr->getTraceRef()
            .at(stepNumber)
            .getEventMapRef()
            .insert({formula.to_string(), EventValue(verdict.value)});
    }
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
    clonedMonitor->verdictHistory.clear();
    clonedMonitor->lastPruneStep = {};
    clonedMonitor->numEvaluations = 0;
    for (const auto &val : domValNames)
        clonedMonitor->subID += "_" + val;
    allCopies.emplace_back(domValNames, clonedMonitor);
    return clonedMonitor;
}

bool Hydra2Monitor::validAt(const std::vector<DomainValue> &domainValues, const size_t timeStamp) {
    return std::all_of(domainValues.begin(), domainValues.end(),
                       [timeStamp](const DomainValue &val) { return val.isConst() or val.checkValidity(timeStamp); });
}

bool Hydra2Monitor::stepNumberValid(const size_t stepNumber, const size_t parentId) {
    if (stepNumber < traceIdx[parentId])
        return false;
    return true;
}
