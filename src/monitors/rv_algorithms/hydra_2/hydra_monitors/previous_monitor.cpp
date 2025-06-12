#include "monitors/rv_algorithms/hydra_2/hydra_monitors/previous_monitor.h"

Verdict PreviousMonitor::concreteStep(const size_t parentId, const size_t stepNumber) {
    if (traceIdx[parentId] < stepNumber)
        currentVerdict = subMonitor->step(monitorId, stepNumber - 1); // in case of short-circuiting

    if (Verdict subVerdict = subMonitor->step(monitorId, stepNumber);
        currentVerdict.has_value() and subVerdict.timeStamp.has_value()) {
        Verdict lastSubVerdict = *currentVerdict;
        currentVerdict = subVerdict;
        if ((!interval.isFinite() and
             interval.getStart() <= subVerdict.timeStamp.value() - lastSubVerdict.timeStamp.value()) or
            (interval.isFinite() and
             Utils::isInInterval(subVerdict.timeStamp.value() - lastSubVerdict.timeStamp.value(), interval.getStart(),
                                 interval.getEnd())))
            return {subVerdict.timeStamp.value(), lastSubVerdict.value};
        return {subVerdict.timeStamp.value(), false};
    } else {
        if (subVerdict.timeStamp.has_value()) { // first trace element
            currentVerdict = subVerdict;
            return {subVerdict.timeStamp.value(), false};
        } // behind trace (no more trace element exist)
        return {false};
    }
}
void PreviousMonitor::setTrace(const std::shared_ptr<Trace> &tra,
                               const std::vector<std::shared_ptr<DomainStorage>> &domains) {
    subMonitor->setTrace(tra, domains);
}

void PreviousMonitor::reset() {
    Hydra2Monitor::reset();
    subMonitor->reset();
    currentVerdict = {};
}

std::shared_ptr<Hydra2Monitor> PreviousMonitor::clone(const std::vector<std::string> &domValNames) {
    if (auto tmpCopy{Hydra2Monitor::clone(domValNames)}; tmpCopy != nullptr)
        return tmpCopy;
    const auto clonedMonitor = std::make_shared<PreviousMonitor>(*this);
    clonedMonitor->subMonitor = subMonitor->clone(domValNames);
    return resetClonedMonitor(clonedMonitor, domValNames);
}

std::string PreviousMonitor::to_string() {
    std::string itv{};
    std::string end{};
    if (!interval.isEmpty()) {
        if (!interval.isFinite())
            end = "inf";
        else
            end = std::to_string(interval.getEnd());
        itv = "[" + std::to_string(interval.getStart()) + ", " + end + "]";
    }
    return std::string{"P"} + itv + "(" + subMonitor->to_string() + ")";
}

void PreviousMonitor::addDomainValues(const std::vector<DomainValue> &vals) { subMonitor->addDomainValues(vals); }

void PreviousMonitor::setStepNumber(const size_t stepNumber, const size_t parentId) {
    Hydra2Monitor::setStepNumber(stepNumber, parentId);
    if (!stepNumberValid(stepNumber, parentId))
        return;
    subMonitor->setStepNumber(stepNumber, monitorId);
}

void PreviousMonitor::to_tree(std::map<size_t, std::tuple<std::string, std::vector<size_t>>> &tree) {
    if (tree.find(monitorId) != tree.end())
        return;
    tree.insert({monitorId, {formula.getExpr()->getSymbol(), {subMonitor->getMonitorID()}}});
    subMonitor->to_tree(tree);
}
