#include "monitors/rv_algorithms/hydra_2/hydra_monitors/atomic_monitor.h"

Verdict AtomicMonitor::concreteStep(size_t parentId, const size_t stepNumber) {
    if (const auto tracePtr = trace.lock(); !tracePtr)
        return {false};
    if (const auto tracePtr = trace.lock(); stepNumber < tracePtr->getLength()) {
        if (!domainValue.empty()) {
            tracePtr->updateEvent(stepNumber, domainValue);
            return tracePtr->verdictAtTimeStep(stepNumber, domainValue);
        }
        return tracePtr->verdictAtTimeStep(stepNumber, name);
    }
    return {false};
}

void AtomicMonitor::setTrace(const std::shared_ptr<Trace> &tra,
                             const std::vector<std::shared_ptr<DomainStorage>> &domains) {
    trace = tra;
}

std::string AtomicMonitor::to_string() { return formula.to_string(); }

void AtomicMonitor::addDomainValues(const std::vector<DomainValue> &vals) {
    for (const auto &dom : vals)
        if (dom.getVariable() == name)
            domainValue = std::any_cast<const char *>(dom.getValue());
}

std::shared_ptr<Hydra2Monitor> AtomicMonitor::clone(const std::vector<std::string> &domValNames) {
    if (auto tmpCopy{Hydra2Monitor::clone(domValNames)}; tmpCopy != nullptr)
        return tmpCopy;
    auto clonedMonitor{std::make_shared<AtomicMonitor>(*this)};
    clonedMonitor->verdictHistory.clear();
    allCopies.emplace_back(domValNames, clonedMonitor);
    return clonedMonitor;
}
