#include "universal_monitor.h"

#include <utility>

UniversalMonitor::UniversalMonitor(Formula formula, std::shared_ptr<HydraMonitor> subMonitor, size_t monitorId,
                                   std::map<std::string, std::string> variables, DomainCollection &domains,
                                   bool connectedEval, bool shortCircuiting)
    : QuantifierMonitor(std::move(formula), subMonitor, monitorId, std::move(variables), domains,
                        connectedEval, shortCircuiting, computeCost(subMonitor), computeSatisfactionProbability(subMonitor)) {}

Verdict UniversalMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    Verdict verdict{true};
    if (traceIdx[parentId] < trace.getLength())
        verdict.ts = trace.getEvent(traceIdx[parentId]).getTimeStep();
    else
        return Verdict{false};

    if (!checkNonEmptiness())
        return verdict;

    while (nextCombination()) {
        Verdict subVerdict = subMonitor->step(monitorId, stepNumber);
        if (shortCircuiting)
            if (!subVerdict.b) {
                resetIterators();
                return subVerdict;
            }
        verdict = verdict && subVerdict;
        subMonitor->reset(traceIdx[parentId]);
    }

    subMonitor->reset(traceIdx[parentId] + 1);
    return verdict;
}

std::string UniversalMonitor::to_string() { return formula.to_string(); }

double UniversalMonitor::computeCost(const std::shared_ptr<HydraMonitor> &monitor) { return 0; }

double UniversalMonitor::computeSatisfactionProbability(const std::shared_ptr<HydraMonitor> &monitor) {
    return 0;
}
