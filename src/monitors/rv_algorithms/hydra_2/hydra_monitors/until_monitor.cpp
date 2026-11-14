#include "monitors/rv_algorithms/hydra_2/hydra_monitors/until_monitor.h"

#include <algorithm>
#include <cmath>

bool UntilMonitor::left(const size_t stepNumber) {
    return validAt(domainValueArguments, timeStepsTrace.at(stepNumber)) and
           subMonitorLeft->step(monitorId, stepNumber).value;
}

bool UntilMonitor::right(const size_t stepNumber) {
    return validAt(domainValueArguments, timeStepsTrace.at(stepNumber)) and
           subMonitorRight->step(monitorId, stepNumber).value;
}

bool UntilMonitor::evalUnbounded(const size_t stepNumber) {
    // (left U[0, inf] right) at j holds iff right holds at j, or left holds at j and the until holds at j + 1. The
    // steps up to the first step with a known result, a satisfied right side, or a violated left side share its result.
    if (untilUnbounded.size() != timeStepsTrace.size())
        untilUnbounded.assign(timeStepsTrace.size(), -1);
    size_t idx{stepNumber};
    bool result{false};
    while (true) {
        if (untilUnbounded.at(idx) >= 0) {
            result = untilUnbounded.at(idx) == 1;
            break;
        }
        if (right(idx)) {
            result = true;
            break;
        }
        if (!left(idx) or idx + 1 == timeStepsTrace.size()) {
            result = false;
            break;
        }
        ++idx;
    }
    for (size_t j{stepNumber}; j <= idx; ++j)
        untilUnbounded.at(j) = result ? 1 : 0;
    return result;
}

double UntilMonitor::computeSatisfactionProbability(const std::shared_ptr<Hydra2Monitor> &left,
                                                    const std::shared_ptr<Hydra2Monitor> &right,
                                                    const IntervalTemporalOperator &iv) {
    size_t end{0};
    if (iv.isFinite() and !iv.isEmpty())
        end = iv.getEnd();
    else
        end = 100; // todo provide value as parameter
    double probability{0.0};
    for (size_t i{iv.getStart()}; i <= end; ++i) {
        probability += right->getSatisfactionProbability() *
                       std::pow(1 - right->getSatisfactionProbability(), i - iv.getStart()) *
                       std::pow(left->getSatisfactionProbability(), i);
    }
    return std::clamp(probability, 0.0, 1.0); // the sum can slightly exceed 1 due to rounding
}

Verdict UntilMonitor::concreteStep(size_t /*parentId*/, const size_t stepNumber) {
    if (stepNumber >= timeStepsTrace.size())
        return {false};
    const size_t timeStamp{timeStepsTrace.at(stepNumber)};
    for (size_t idx{stepNumber}; idx < timeStepsTrace.size(); ++idx) {
        const size_t distance{timeStepsTrace.at(idx) - timeStamp};
        if (interval.isFinite() and distance > interval.getEnd())
            break;
        // from the first step within an unbounded interval on, the until behaves like (left U[0, inf] right)
        if (!interval.isFinite() and distance >= interval.getStart())
            return {timeStamp, evalUnbounded(idx)};
        if (interval.contains(distance) and right(idx))
            return {timeStamp, true};
        if (!left(idx))
            break;
    }
    return {timeStamp, false};
}

void UntilMonitor::setTrace(const std::shared_ptr<Trace> &tra,
                            const std::vector<std::shared_ptr<DomainStorage>> &domains) {
    subMonitorLeft->setTrace(tra, domains);
    subMonitorRight->setTrace(tra, domains);
    timeStepsTrace = {};
    untilUnbounded = {};
    for (const auto &event : tra->getTrace())
        timeStepsTrace.push_back(event.getTimeStep());
    trace = tra;
}

void UntilMonitor::reset() {
    Hydra2Monitor::reset();
    subMonitorRight->reset();
    subMonitorLeft->reset();
    timeStepsTrace = {};
    untilUnbounded = {};
}

std::shared_ptr<Hydra2Monitor> UntilMonitor::clone(const std::vector<std::string> &domValNames) {
    if (auto tmpCopy{Hydra2Monitor::clone(domValNames)}; tmpCopy != nullptr)
        return tmpCopy;
    const auto clonedMonitor = std::make_shared<UntilMonitor>(*this);
    clonedMonitor->subMonitorRight = subMonitorRight->clone(domValNames);
    clonedMonitor->subMonitorLeft = subMonitorLeft->clone(domValNames);
    clonedMonitor->untilUnbounded = {};
    return resetClonedMonitor(clonedMonitor, domValNames);
}

std::string UntilMonitor::to_string() {
    std::string itv{" "};
    std::string end{};
    if (!interval.isEmpty()) {
        if (!interval.isFinite())
            end = "inf";
        else
            end = std::to_string(interval.getEnd());
        itv = "[" + std::to_string(interval.getStart()) + ", " + end + "] ";
    }
    return "(" + subMonitorLeft->to_string() + " U" + itv + subMonitorRight->to_string() + ")";
}

void UntilMonitor::addDomainValues(const std::vector<DomainValue> &vals) {
    domainValueArguments = vals;
    subMonitorRight->addDomainValues(vals);
    subMonitorLeft->addDomainValues(vals);
}

void UntilMonitor::to_tree(std::map<size_t, std::tuple<std::string, std::vector<size_t>>> &tree) {
    if (tree.find(monitorId) != tree.end())
        return;
    tree.insert({monitorId,
                 {formula.getExpr()->getSymbol(), {subMonitorLeft->getMonitorID(), subMonitorRight->getMonitorID()}}});
    subMonitorLeft->to_tree(tree);
    subMonitorRight->to_tree(tree);
}
