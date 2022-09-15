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
        verdict.timeStamp = trace.getEvent(traceIdx[parentId]).getTimeStep();
    else
        return Verdict{false};

    if (!checkNonEmptiness())
        return verdict;

    size_t count{0};
    while (nextCombination()) {
        std::shared_ptr<HydraMonitor> evalMonitor;
        if (traceIdx[parentId] == 0) {
            evalMonitor = subMonitor->clone();
            evalMonitors.push_back(evalMonitor);
        } else
            evalMonitor = evalMonitors[count];

        Verdict subVerdict = evalMonitor->step(monitorId, stepNumber);

        verdict = verdict || subVerdict;

        count++;
    }

    return verdict;
}

std::shared_ptr<HydraMonitor> ExistentialMonitor::clone() {
    auto clonedMonitor = std::make_shared<ExistentialMonitor>(*this);
    clonedMonitor->subMonitor = subMonitor->clone();

    clonedMonitor->iters.clear();
    for (auto &varDomain : clonedMonitor->varDomains)
        clonedMonitor->iters.push_back(varDomain.second.begin());

    return clonedMonitor;
}

std::string ExistentialMonitor::to_string() { return formula.to_string(); }
