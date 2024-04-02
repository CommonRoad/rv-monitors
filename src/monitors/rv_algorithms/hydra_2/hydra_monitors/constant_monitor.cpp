#include "monitors/rv_algorithms/hydra_2/hydra_monitors/constant_monitor.h"
#include <monitors/expression_tree/atomic/constant.h>

Verdict ConstantMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    if (stepNumber < trace.getLength()) {
        auto event{trace.getEvent(stepNumber)};
        auto verdict{
            Verdict(event.getTimeStep(), (*std::dynamic_pointer_cast<Constant>(formula.getExpr())).getValue())};
        return verdict;
    } else
        return {false};
}

void ConstantMonitor::setTrace(const Trace &tra, const std::vector<std::shared_ptr<DomainStorage>> &domains) {
    trace = tra;
}

std::shared_ptr<Hydra2Monitor> ConstantMonitor::clone() { return std::make_shared<ConstantMonitor>(*this); }

std::string ConstantMonitor::to_string() { return formula.getExpr()->getSymbol(); }

void ConstantMonitor::addDomainValues(const std::vector<DomainValue> &vals) {}
