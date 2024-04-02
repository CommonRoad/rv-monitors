#include "monitors/rv_algorithms/hydra_2/hydra_monitors/since_monitor.h"

Verdict SinceMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    subVerdictLeft = subMonitorLeft->step(monitorId, stepNumber);
    if (subVerdictLeft->timeStamp.has_value())
        headPositionMonitorLeft.update(subVerdictLeft->timeStamp.value());
    if (!subVerdictLeft->value)
        lastViolation = HeadPosHydra2(headPositionMonitorLeft.timeStep, headPositionMonitorLeft.traceIdx);

    if (stepNumber == 0) {
        subVerdictRight = subMonitorRight->step(monitorId, stepNumberRight);
        stepNumberRight++;
        if (subVerdictRight->timeStamp.has_value())
            headPositionMonitorRight.update(subVerdictRight->timeStamp.value());
        if (!std::all_of(
                domainValueArguments.begin(), domainValueArguments.end(),
                [step = stepNumberRight](const DomainValue &dv) { return dv.isConst() or dv.checkValidity(step); }))
            endReached = true;
    }

    // loop until end is reached, right monitor time points equals (is greater) than left monitor's time point, or lower
    // time bound is violated
    while (!endReached && headPositionMonitorRight.traceIdx <= headPositionMonitorLeft.traceIdx &&
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
        if (!std::all_of(
                domainValueArguments.begin(), domainValueArguments.end(),
                [step = stepNumberRight](const DomainValue &dv) { return dv.isConst() or dv.checkValidity(step); })) {
            endReached = true; // just for completeness, could be removed
            break;
        }
    }

    // check whether right side of until was satisfied and whether satisfaction is before end of allowed time interval
    if (lastSatisfaction &&
        (!interval.isFinite() or headPositionMonitorLeft.timeStep - lastSatisfaction->timeStep <= interval.getEnd()) &&
        (!lastViolation || lastSatisfaction->traceIdx >= lastViolation->traceIdx))
        return {headPositionMonitorLeft.timeStep, true};
    else
        return {headPositionMonitorLeft.timeStep, false};
}

void SinceMonitor::setTrace(const Trace &tra, const std::vector<std::shared_ptr<DomainStorage>> &domains) {
    subMonitorLeft->setTrace(tra, domains);
    subMonitorRight->setTrace(tra, domains);
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
}

std::shared_ptr<Hydra2Monitor> SinceMonitor::clone() {
    auto clonedMonitor = std::make_shared<SinceMonitor>(*this);
    clonedMonitor->subMonitorRight = subMonitorRight->clone();
    clonedMonitor->subMonitorLeft = subMonitorLeft->clone();
    return clonedMonitor;
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
void SinceMonitor::setStepNumber(size_t stepNumber, size_t parentId) {
    if (stepNumber > stepNumberRight) {
        while (!domainValueArguments.at(0).isConst() and
               !domainValueArguments.at(0).checkValidity(stepNumberRight)) { // todo more efficient
            ++stepNumberRight;
            Hydra2Monitor::setStepNumber(stepNumberRight, parentId);
            subMonitorRight->setStepNumber(stepNumberRight, monitorId);
            subMonitorLeft->setStepNumber(stepNumberRight, monitorId);
        }
        if (!domainValueArguments.at(0).isConst() and domainValueArguments.at(0).checkValidity(stepNumber - 1))
            Hydra2Monitor::step(parentId, stepNumber - 1);
    }
}
