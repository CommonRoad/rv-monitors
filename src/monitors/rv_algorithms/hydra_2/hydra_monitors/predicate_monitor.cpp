#include "monitors/rv_algorithms/hydra_2/hydra_monitors/predicate_monitor.h"
#include "monitors/expression_tree/atomic/predicate.h"
#include <cassert>
#include <iostream>

Verdict PredicateMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    if (stepNumber < trace.getLength()) {
        if (!domainValueArguments.empty())
            updateTrace(stepNumber);
        return trace.verdictAtTimeStep(stepNumber, name);
    } else
        return {false};
}

void PredicateMonitor::updateTrace(size_t stepNumber) {
    std::vector<DomainValue> orderedValues;
    for (const auto &arg : relevantArguments) {
        for (const auto &val : domainValueArguments)
            if (arg == val.getVariable()) {
                orderedValues.push_back(val);
                break;
            }
        for (const auto &val : constArguments)
            if (arg == val) {
                orderedValues.emplace_back(val, val);
                break;
            }
    }
    assert(orderedValues.size() == (relevantArguments.size()));
    trace.updateEvent(stepNumber, name, orderedValues);
}

std::shared_ptr<Hydra2Monitor> PredicateMonitor::clone() { return std::make_shared<PredicateMonitor>(*this); }

void PredicateMonitor::setTrace(const Trace &tra) { trace = tra; }

std::string PredicateMonitor::to_string() { return formula.to_string(); }

void PredicateMonitor::addDomainValues(const std::vector<DomainValue> &vals) { domainValueArguments = vals; }

PredicateMonitor::PredicateMonitor(Formula formula, size_t monitorId, bool connectedEval, double evalCost,
                                   double satisfactionProbability)
    : Hydra2Monitor(std::move(formula), monitorId, connectedEval, evalCost, satisfactionProbability),
      name(std::dynamic_pointer_cast<Predicate>(this->formula.getExpr())->to_string()),
      relevantArguments(std::dynamic_pointer_cast<Predicate>(this->formula.getExpr())->getParameters()),
      constArguments(std::dynamic_pointer_cast<Predicate>(this->formula.getExpr())->getConstParameters()) {}

void PredicateMonitor::reset() {
    Hydra2Monitor::reset();
    domainValueArguments.clear();
}
