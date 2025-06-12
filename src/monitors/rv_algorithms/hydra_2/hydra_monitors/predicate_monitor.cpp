#include "monitors/rv_algorithms/hydra_2/hydra_monitors/predicate_monitor.h"
#include "monitors/expression_tree/atomic/predicate.h"
#include <cassert>

Verdict PredicateMonitor::concreteStep(size_t parentId, const size_t stepNumber) {
    if (stepNumber < trace->getLength()) {
        auto verdict{Verdict(false)};
        if (!trace->getTraceRef().at(stepNumber).hasVerdict(domainName)) {
            trace->updateEvent(stepNumber, name, orderedValues);
            verdict = trace->verdictAtTimeStep(stepNumber, name, domainName);
            orderedValues.erase(orderedValues.begin());
        } else
            verdict = trace->verdictAtTimeStep(stepNumber, name, domainName);
        return verdict;
    }
    return {false};
}

std::shared_ptr<Hydra2Monitor> PredicateMonitor::clone(const std::vector<std::string> &domValNames) {
    if (auto tmpCopy{Hydra2Monitor::clone(domValNames)}; tmpCopy != nullptr)
        return tmpCopy;
    const auto clonedMonitor{std::make_shared<PredicateMonitor>(*this)};
    return resetClonedMonitor(clonedMonitor, domValNames);
}

void PredicateMonitor::setTrace(const std::shared_ptr<Trace> &tra,
                                const std::vector<std::shared_ptr<DomainStorage>> &domains) {
    if (trace == nullptr) {
        trace = tra;
        for (const auto &con : std::dynamic_pointer_cast<Predicate>(this->formula.getExpr())->getConstParameters())
            constArguments.push_back(tra->getConstantCollection().getConstant(con));
    }
}

std::string PredicateMonitor::to_string() { return formula.to_string(); }

void PredicateMonitor::addDomainValues(const std::vector<DomainValue> &vals) {
    domainValueArguments = vals;
    orderedValues.clear();
    for (const auto &arg : relevantArguments) {
        for (const auto &val : domainValueArguments)
            if (arg == val.getVariable()) {
                orderedValues.push_back(val);
                break;
            }
        for (const auto &val : constArguments)
            if (arg == val.getName()) {
                orderedValues.emplace_back(val.getName(), val.getValue(), true, val.getIdFunction());
                break;
            }
    }
    assert(orderedValues.size() == relevantArguments.size());
    domainName = name.substr(0, name.find('(')) + "(";
    for (const auto &val : orderedValues)
        domainName += "'" + val.getUniqueID() + "'" + ", ";
    domainName.pop_back();
    domainName.pop_back();
    domainName += ")";
}

PredicateMonitor::PredicateMonitor(Formula formula, const size_t monitorId, const double evalCost,
                                   const double satisfactionProbability)
    : Hydra2Monitor(std::move(formula), monitorId, evalCost, satisfactionProbability),
      name(std::dynamic_pointer_cast<Predicate>(this->formula.getExpr())->to_string()),
      relevantArguments(std::dynamic_pointer_cast<Predicate>(this->formula.getExpr())->getParameters()) {}

void PredicateMonitor::reset() {
    Hydra2Monitor::reset();
    domainValueArguments.clear();
    orderedValues.clear();
    constArguments.clear();
    trace = nullptr;
}
