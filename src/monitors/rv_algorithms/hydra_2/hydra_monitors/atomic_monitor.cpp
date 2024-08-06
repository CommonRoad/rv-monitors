#include "monitors/rv_algorithms/hydra_2/hydra_monitors/atomic_monitor.h"

Verdict AtomicMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    if (stepNumber < trace->getLength()) {
        if (!domainValue.empty()) {
            trace->updateEvent(stepNumber, domainValue);
            return trace->verdictAtTimeStep(stepNumber, domainValue);
        } else
            return trace->verdictAtTimeStep(stepNumber, name);
    } else
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
    auto tmpCopy{Hydra2Monitor::clone(domValNames)};
    if (tmpCopy != nullptr)
        return tmpCopy;
    auto clonedMonitor{std::make_shared<AtomicMonitor>(*this)};
    allCopies.emplace_back(domValNames, clonedMonitor);
    return clonedMonitor;
}
