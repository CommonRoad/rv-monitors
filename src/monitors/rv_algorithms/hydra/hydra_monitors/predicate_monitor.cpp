#include "monitors/rv_algorithms/hydra/hydra_monitors/predicate_monitor.h"
#include "monitors/expression_tree/atomic/predicate.h"

Verdict PredicateMonitorHydra::concreteStep() {
    if (traceIdx < trace.value()->getLength()) {
        auto event{trace.value()->getEvent(traceIdx)};
        auto verdict{
            Verdict(event.getTimeStep(), event.getValue(*std::dynamic_pointer_cast<Predicate>(formula.getExpr())))};
        ++traceIdx;
        return verdict;
    } else
        return {false};
}

void PredicateMonitorHydra::setTrace(const std::shared_ptr<Trace> &tra) { trace = tra; }

void PredicateMonitorHydra::reset() { traceIdx = 0; }

std::string PredicateMonitorHydra::to_string() { return formula.to_string(); }
