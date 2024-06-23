#include "monitors/rv_algorithms/hydra_2/hydra_monitors/previous_monitor.h"

Verdict PreviousMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    Verdict subVerdict = subMonitor->step(monitorId, stepNumber);

    if (currentVerdict.has_value() and subVerdict.timeStamp.has_value()) {
        Verdict lastSubVerdict = *currentVerdict;
        currentVerdict = subVerdict;
        if (!interval.isFinite() and
            interval.getStart() <= subVerdict.timeStamp.value() - lastSubVerdict.timeStamp.value())
            return {subVerdict.timeStamp.value(), lastSubVerdict.value};
        else if (interval.isFinite() and
                 Utils::isInInterval(subVerdict.timeStamp.value() - lastSubVerdict.timeStamp.value(),
                                     interval.getStart(), interval.getEnd()))
            return {subVerdict.timeStamp.value(), lastSubVerdict.value};
        else
            return {subVerdict.timeStamp.value(), false};
    } else if (subVerdict.timeStamp.has_value()) { // first trace element
        currentVerdict = subVerdict;
        return {subVerdict.timeStamp.value(), false};
    } else // behind trace (no more trace element exist)
        return {false};
}
void PreviousMonitor::setTrace(const Trace &tra, const std::vector<std::shared_ptr<DomainStorage>> &domains) {
    subMonitor->setTrace(tra, domains);
}

void PreviousMonitor::reset() {
    Hydra2Monitor::reset();
    subMonitor->reset();
    currentVerdict = {};
}

std::shared_ptr<Hydra2Monitor> PreviousMonitor::clone() {
    auto clonedMonitor = std::make_shared<PreviousMonitor>(*this);
    clonedMonitor->subMonitor = subMonitor->clone();
    return clonedMonitor;
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

void PreviousMonitor::setStepNumber(size_t stepNumber, size_t parentId) {
    Hydra2Monitor::setStepNumber(stepNumber, parentId);
    subMonitor->setStepNumber(stepNumber, monitorId);
}

void PreviousMonitor::to_tree(std::map<size_t, std::tuple<std::string, std::vector<size_t>>> &tree) {
    if (tree.find(monitorId) != tree.end())
        return;
    tree.insert({monitorId, {formula.getExpr()->getSymbol(), {subMonitor->getMonitorID()}}});
    subMonitor->to_tree(tree);
}
