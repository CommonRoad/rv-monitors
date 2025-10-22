#include "monitors/rv_algorithms/hydra_2/hydra_monitors/since_monitor.h"

#include <cmath>

bool SinceMonitor::checkTimeStepValidity(size_t concreteStep) {
    return !std::all_of(
        domainValueArguments.begin(), domainValueArguments.end(),
        [step = concreteStep](const DomainValue &dv) { return dv.isConst() or dv.checkValidity(step); });
}

Verdict SinceMonitor::concreteStep(const size_t parentId, const size_t stepNumber) {
    // propagate trace index and reset variables after short-circuiting or invalid dynamic domain value
    auto reset{updateTraceIndex(parentId, stepNumber)};

    while (traceIdx[parentId] <= stepNumber) {
        subVerdictLeft = subMonitorLeft->step(monitorId, stepNumber);
        if (subVerdictLeft->timeStamp.has_value())
            headPositionMonitorLeft.update(subVerdictLeft->timeStamp.value());
        if (!subVerdictLeft->value)
            lastViolation = HeadPosHydra2(headPositionMonitorLeft.timeStep, headPositionMonitorLeft.traceIdx);

        if (traceIdx[parentId] == 0 or reset) {
            subVerdictRight = subMonitorRight->step(monitorId, stepNumberRight);
            stepNumberRight++;
            if (subVerdictRight->timeStamp.has_value())
                headPositionMonitorRight.update(subVerdictRight->timeStamp.value());
            if (timeStepsTrace.size() <= stepNumberRight) {
                if (subVerdictRight->value)
                    lastSatisfaction =
                        HeadPosHydra2(headPositionMonitorRight.timeStep, headPositionMonitorRight.traceIdx);
                endReached = true;
            }
            reset = false;
        }

        // if-clause needed otherwise call of predicate with non-existing domain value possible if while loop is entered
        if (timeStepsTrace.size() <= stepNumberRight) {
            endReached = true;
            if (subVerdictRight->value)
                lastSatisfaction = HeadPosHydra2(headPositionMonitorRight.timeStep, headPositionMonitorRight.traceIdx);
            headPositionMonitorRight.traceIdx++;
        }

        // loop until end is reached, right monitor time points equals (is greater) than left monitor's time point, or
        // lower time bound is violated
        while (timeStepsTrace.size() > stepNumberRight and
               !checkTimeStepValidity(timeStepsTrace.at(stepNumberRight)) and !endReached &&
               headPositionMonitorRight.traceIdx <= headPositionMonitorLeft.traceIdx &&
               headPositionMonitorLeft.timeStep - headPositionMonitorRight.timeStep >= interval.getStart()) {
            if (subVerdictRight->value)
                lastSatisfaction = HeadPosHydra2(headPositionMonitorRight.timeStep, headPositionMonitorRight.traceIdx);
            subVerdictRight = subMonitorRight->step(monitorId, stepNumberRight);
            stepNumberRight++;
            if (subVerdictRight->timeStamp.has_value())
                headPositionMonitorRight.update(subVerdictRight->timeStamp.value());
            else {
                endReached = true;
                headPositionMonitorRight.traceIdx++;
            }
        }
        ++traceIdx[parentId];
    }
    --traceIdx[parentId];
    // check whether right side of until was satisfied and whether satisfaction is before end of allowed time interval
    if (lastSatisfaction &&
        (!interval.isFinite() or headPositionMonitorLeft.timeStep - lastSatisfaction->timeStep <= interval.getEnd()) &&
        (!lastViolation || lastSatisfaction->traceIdx >= lastViolation->traceIdx))
        return {headPositionMonitorLeft.timeStep, true};
    return {headPositionMonitorLeft.timeStep, false};
}

double SinceMonitor::computeSatisfactionProbability(const std::shared_ptr<Hydra2Monitor> &left,
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
    return probability;
}

bool SinceMonitor::updateTraceIndex(const size_t parentId, const size_t stepNumber) {
    // reset step number so that interval is covered after short-circuiting
    bool reset{false};
    if (interval.isFinite() and ((stepNumber > traceIdx[parentId] and stepNumber > interval.getEnd() and
                                  stepNumber - interval.getEnd() > traceIdx[parentId]) or
                                 checkTimeStepValidity(traceIdx[parentId]))) {
        while ((stepNumber > traceIdx[parentId] and stepNumber > interval.getEnd() and
                stepNumber - interval.getEnd() > traceIdx[parentId]) or
               (checkTimeStepValidity(traceIdx[parentId]) and traceIdx[parentId] <= stepNumber)) {
            ++traceIdx[parentId];
            headPositionMonitorLeft.update(traceIdx[parentId]);
            headPositionMonitorRight.update(traceIdx[parentId]);
        }
        lastSatisfaction = {};
        lastViolation = {};
        stepNumberRight = traceIdx[parentId];
        subVerdictRight = {};
        subVerdictLeft = {};
        reset = true;
    }
    // for infinite intervals, update trace index until valid domain values are reached
    if (stepNumber > traceIdx[parentId] and !interval.isFinite() and checkTimeStepValidity(traceIdx[parentId])) {
        while (stepNumber > traceIdx[parentId] and checkTimeStepValidity(traceIdx[parentId])) {
            ++traceIdx[parentId];
            headPositionMonitorLeft.update(traceIdx[parentId]);
            headPositionMonitorRight.update(traceIdx[parentId]);
        }
        lastSatisfaction = {};
        lastViolation = {};
        stepNumberRight = traceIdx[parentId];
        subVerdictRight = {};
        subVerdictLeft = {};
        reset = true;
    }
    return reset;
}

void SinceMonitor::setTrace(const std::shared_ptr<Trace> &tra,
                            const std::vector<std::shared_ptr<DomainStorage>> &domains) {
    subMonitorLeft->setTrace(tra, domains);
    subMonitorRight->setTrace(tra, domains);
    timeStepsTrace = {};
    for (const auto &event : tra->getTrace())
        timeStepsTrace.push_back(event.getTimeStep());
    trace = tra;
}
void SinceMonitor::reset() {
    Hydra2Monitor::reset();
    subMonitorRight->reset();
    subMonitorLeft->reset();
    subVerdictLeft = {};
    subVerdictRight = {};
    lastSatisfaction = {};
    lastViolation = {};
    headPositionMonitorRight.reset();
    headPositionMonitorLeft.reset();
    stepNumberRight = 0;
    endReached = false;
    timeStepsTrace = {};
}

std::shared_ptr<Hydra2Monitor> SinceMonitor::clone(const std::vector<std::string> &domValNames) {
    if (auto tmpCopy{Hydra2Monitor::clone(domValNames)}; tmpCopy != nullptr)
        return tmpCopy;
    const auto clonedMonitor = std::make_shared<SinceMonitor>(*this);
    clonedMonitor->subMonitorRight = subMonitorRight->clone(domValNames);
    clonedMonitor->subMonitorLeft = subMonitorLeft->clone(domValNames);
    return resetClonedMonitor(clonedMonitor, domValNames);
}

std::string SinceMonitor::to_string() {
    std::string itv{" "};
    std::string end{};
    if (!interval.isEmpty()) {
        if (!interval.isFinite())
            end = "inf";
        else
            end = std::to_string(interval.getEnd());
        itv = "[" + std::to_string(interval.getStart()) + ", " + end + "] ";
    }
    return "(" + subMonitorLeft->to_string() + " S" + itv + subMonitorRight->to_string() + ")";
}

void SinceMonitor::addDomainValues(const std::vector<DomainValue> &vals) {
    domainValueArguments = vals;
    subMonitorRight->addDomainValues(vals);
    subMonitorLeft->addDomainValues(vals);
}

void SinceMonitor::setStepNumber(const size_t stepNumber, const size_t parentId) {
    if (!stepNumberValid(stepNumber, parentId))
        return;
    subMonitorRight->setStepNumber(stepNumber, monitorId);
    subMonitorLeft->setStepNumber(stepNumber, monitorId);
}

void SinceMonitor::to_tree(std::map<size_t, std::tuple<std::string, std::vector<size_t>>> &tree) {
    if (tree.find(monitorId) != tree.end())
        return;
    tree.insert({monitorId,
                 {formula.getExpr()->getSymbol(), {subMonitorLeft->getMonitorID(), subMonitorRight->getMonitorID()}}});
    subMonitorLeft->to_tree(tree);
    subMonitorRight->to_tree(tree);
}
