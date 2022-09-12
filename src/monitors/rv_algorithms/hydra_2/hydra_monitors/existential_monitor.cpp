#include "existential_monitor.h"

ExistentialMonitor::ExistentialMonitor(Formula formula, std::shared_ptr<HydraMonitor> subMonitor, size_t monitorId,
                                       std::map<std::string, std::string> variables, DomainCollection &domains,
                                       bool connectedEval, bool shortCircuiting)
    : QuantifierMonitor(std::move(formula), subMonitor, monitorId, std::move(variables), domains,
                        connectedEval, shortCircuiting, 0.0, 0.0) {
    evalCost = computeCost(subMonitor, numCombinations());
    satisfactionProbability = computeSatisfactionProbability(subMonitor, numCombinations());
}

Verdict ExistentialMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    Verdict verdict{false};
    if (traceIdx[parentId] < trace.getLength())
        verdict.ts = trace.getEvent(traceIdx[parentId]).getTimeStep();
    else
        return Verdict{false};

    if (!checkNonEmptiness())
        return verdict;

    while (nextCombination()) {
        verdict = subMonitor->step(monitorId, stepNumber);
        if (verdict.b)
            break;
        subMonitor->reset(traceIdx[parentId]);
    }

    subMonitor->reset(traceIdx[parentId] + 1);
    return verdict;
}

std::string ExistentialMonitor::to_string() { return formula.to_string(); }
