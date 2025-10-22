#include "monitors/rv_algorithms/hydra_2/hydra_monitors/negation_monitor.h"

Verdict NegationMonitor::concreteStep(size_t parentId, const size_t stepNumber) {
    return !subMonitor->step(monitorId, stepNumber);
}

void NegationMonitor::setTrace(const std::shared_ptr<Trace> &tra,
                               const std::vector<std::shared_ptr<DomainStorage>> &domains) {
    subMonitor->setTrace(tra, domains);
    trace = tra;
}

void NegationMonitor::reset() {
    Hydra2Monitor::reset();
    subMonitor->reset();
}

std::shared_ptr<Hydra2Monitor> NegationMonitor::clone(const std::vector<std::string> &domValNames) {
    if (auto tmpCopy{Hydra2Monitor::clone(domValNames)}; tmpCopy != nullptr)
        return tmpCopy;
    const auto clonedMonitor = std::make_shared<NegationMonitor>(*this);
    clonedMonitor->subMonitor = subMonitor->clone(domValNames);
    return resetClonedMonitor(clonedMonitor, domValNames);
}

std::string NegationMonitor::to_string() { return "!(" + subMonitor->to_string() + ")"; }

void NegationMonitor::addDomainValues(const std::vector<DomainValue> &vals) { subMonitor->addDomainValues(vals); }

void NegationMonitor::setStepNumber(const size_t stepNumber, const size_t parentId) {
    Hydra2Monitor::setStepNumber(stepNumber, parentId);
    if (!stepNumberValid(stepNumber, parentId))
        return;
    subMonitor->setStepNumber(stepNumber, monitorId);
}

void NegationMonitor::to_tree(std::map<size_t, std::tuple<std::string, std::vector<size_t>>> &tree) {
    if (tree.find(monitorId) != tree.end())
        return;
    tree.insert({monitorId, {formula.getExpr()->getSymbol(), {subMonitor->getMonitorID()}}});
    subMonitor->to_tree(tree);
}
