#include "monitors/rv_algorithms/hydra/hydra_monitors/constant_monitor.h"
#include <monitors/expression_tree/atomic/constant.h>

Verdict ConstantMonitorHydra::concreteStep() {
    if (traceIdx < trace.value()->getLength()) {
        auto event{trace.value()->getEvent(traceIdx)};
        auto verdict{
            Verdict(event.getTimeStep(), (*std::dynamic_pointer_cast<Constant>(formula.getExpr())).getValue())};
        ++traceIdx;
        return verdict;
    } else
        return {false};
}

void ConstantMonitorHydra::setTrace(const std::shared_ptr<Trace> &tra) { trace = tra; }

void ConstantMonitorHydra::reset() { traceIdx = 0; }

std::string ConstantMonitorHydra::to_string() { return formula.getExpr()->getSymbol(); }
