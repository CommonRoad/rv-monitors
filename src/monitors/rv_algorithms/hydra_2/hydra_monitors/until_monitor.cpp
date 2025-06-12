#include "monitors/rv_algorithms/hydra_2/hydra_monitors/until_monitor.h"

#include <cmath>

void UntilMonitor::stepLeftRightMonitors() {
    subVerdictLeft = subMonitorLeft->step(monitorId, stepNumberUntil);
    if (subVerdictLeft->timeStamp)
        headPositionMonitorLeft.update(subVerdictLeft->timeStamp.value());
    else {
        endReached = true;
        ++stepNumberUntil;
    }
    subVerdictRight = subMonitorRight->step(monitorId, stepNumberUntil);
    if (subVerdictRight->timeStamp)
        headPositionMonitorRight.update(subVerdictRight->timeStamp.value());
    else
        endReached = true;
    ++stepNumberUntil;
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
    return probability;
}

Verdict UntilMonitor::concreteStep(const size_t parentId, const size_t stepNumber) {
    if (stepNumber > traceIdx[parentId]) {
        if (const auto subVerdictBaseTimeStepTmp{subMonitorBaseTimeStep->step(monitorId, stepNumber - 1)};
            subVerdictBaseTimeStepTmp.timeStamp)
            headPositionMonitorBaseTimeStep =
                HeadPosHydra2(subVerdictBaseTimeStepTmp.timeStamp.value(), static_cast<int>(stepNumber - 1));
    }
    bool reset{false};
    // reset monitor after short-circuiting
    if (stepNumberUntil <= stepNumber and stepNumber > traceIdx[parentId]) {
        subVerdictLeft = {};
        subVerdictRight = {};
        lastSatisfaction = {};
        lastViolation = {};
        headPositionMonitorRight =
            HeadPosHydra2(headPositionMonitorBaseTimeStep.timeStep, static_cast<int>(stepNumber - 1));
        headPositionMonitorLeft =
            HeadPosHydra2(headPositionMonitorBaseTimeStep.timeStep, static_cast<int>(stepNumber - 1));
        stepNumberUntil = stepNumber;
        reset = true;
    }

    subVerdictBaseTimeStep =
        subMonitorBaseTimeStep->step(monitorId, stepNumber); // required in case of non-equidistant time steps
    if (subVerdictBaseTimeStep->timeStamp)
        headPositionMonitorBaseTimeStep.update(subVerdictBaseTimeStep->timeStamp.value());
    else
        return {false};

    if (lastSatisfaction &&
        lastSatisfaction->traceIdx <
            headPositionMonitorBaseTimeStep.traceIdx) // if last satisfaction (-> right side of until satisfied) is
                                                      // before current time step, reset last satisfaction
        lastSatisfaction = {};
    if (lastViolation &&
        lastViolation->traceIdx <
            headPositionMonitorBaseTimeStep.traceIdx) // if last violation (-> left side of until violated) is
                                                      // before current time step, reset last satisfaction
        lastViolation = {};

    // if not step was performed yet or last verdicts are behind base time step, execute step for current base step
    if (!endReached and (stepNumberUntil == 0 or reset)) { // only executed at first call of function
        stepLeftRightMonitors();
    }

    while (!endReached && !lastViolation &&
           (!interval.isFinite() or
            headPositionMonitorLeft.timeStep - headPositionMonitorBaseTimeStep.timeStep <= interval.getEnd() or
            headPositionMonitorLeft.timeStep <
                headPositionMonitorBaseTimeStep.timeStep)) { // loop until end is reached, until left side of monitor is
                                                             // false, or until upper bound of time
                                                             // interval is violated
        if (subVerdictRight->value) {
            lastSatisfaction = HeadPosHydra2(headPositionMonitorRight.timeStep, headPositionMonitorRight.traceIdx);
        }
        if (!subVerdictLeft->value) {
            lastViolation = HeadPosHydra2(headPositionMonitorLeft.timeStep, headPositionMonitorLeft.traceIdx);
        }
        if (timeStepsTrace.size() <= stepNumberUntil or
            !std::all_of(domainValueArguments.begin(), domainValueArguments.end(),
                         [step = timeStepsTrace.at(stepNumberUntil)](const DomainValue &dv) {
                             return dv.isConst() or dv.checkValidity(step);
                         })) {
            endReached = true; // just for completeness, could be removed
            break;
        }
        stepLeftRightMonitors();
    }
    // check whether right side of until was satisfied, whether satisfaction is after start of allowed time interval,
    // and whether left side of from is greater/equal zero (required since unsigned, and we have an overflow
    if (lastSatisfaction &&
        lastSatisfaction->timeStep - headPositionMonitorBaseTimeStep.timeStep >= interval.getStart() &&
        lastSatisfaction->timeStep >= headPositionMonitorBaseTimeStep.timeStep)
        return {headPositionMonitorBaseTimeStep.timeStep, true};
    return {headPositionMonitorBaseTimeStep.timeStep, false};
}

void UntilMonitor::setTrace(const std::shared_ptr<Trace> &tra,
                            const std::vector<std::shared_ptr<DomainStorage>> &domains) {
    subMonitorLeft->setTrace(tra, domains);
    subMonitorRight->setTrace(tra, domains);
    subMonitorBaseTimeStep->setTrace(tra, domains);
    timeStepsTrace = {};
    for (const auto &event : tra->getTrace())
        timeStepsTrace.push_back(event.getTimeStep());
}

void UntilMonitor::reset() {
    Hydra2Monitor::reset();
    subMonitorRight->reset();
    subMonitorLeft->reset();
    subMonitorBaseTimeStep->reset();
    subVerdictLeft = {};
    subVerdictRight = {};
    lastSatisfaction = {};
    lastViolation = {};
    headPositionMonitorRight.reset();
    headPositionMonitorLeft.reset();
    headPositionMonitorBaseTimeStep.reset();
    stepNumberUntil = 0;
    endReached = false;
    timeStepsTrace = {};
}

std::shared_ptr<Hydra2Monitor> UntilMonitor::clone(const std::vector<std::string> &domValNames) {
    if (auto tmpCopy{Hydra2Monitor::clone(domValNames)}; tmpCopy != nullptr)
        return tmpCopy;
    const auto clonedMonitor = std::make_shared<UntilMonitor>(*this);
    clonedMonitor->subMonitorRight = subMonitorRight->clone(domValNames);
    clonedMonitor->subMonitorLeft = subMonitorLeft->clone(domValNames);
    clonedMonitor->subMonitorBaseTimeStep = subMonitorBaseTimeStep->clone(domValNames);
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
    subMonitorBaseTimeStep->addDomainValues(vals);
}

void UntilMonitor::setStepNumber(const size_t stepNumber, const size_t parentId) {
    Hydra2Monitor::setStepNumber(stepNumber, parentId);
    if (!stepNumberValid(stepNumber, parentId))
        return;
    subMonitorRight->setStepNumber(stepNumber, monitorId);
    subMonitorLeft->setStepNumber(stepNumber, monitorId);
    subMonitorBaseTimeStep->setStepNumber(stepNumber, monitorId);
    stepNumberUntil = stepNumber;
}

void UntilMonitor::to_tree(std::map<size_t, std::tuple<std::string, std::vector<size_t>>> &tree) {
    if (tree.find(monitorId) != tree.end())
        return;
    tree.insert({monitorId,
                 {formula.getExpr()->getSymbol(), {subMonitorLeft->getMonitorID(), subMonitorRight->getMonitorID()}}});
    subMonitorLeft->to_tree(tree);
    subMonitorRight->to_tree(tree);
}
