#include "universal_monitor.h"

#include <utility>

UniversalMonitor::UniversalMonitor(Formula formula, std::shared_ptr<HydraMonitor> subMonitor, size_t monitorId,
                                   std::map<std::string, std::string> variables, DomainCollection &domains,
                                   bool connectedEval, bool shortCircuiting)
    : QuantifierMonitor(std::move(formula), subMonitor, monitorId, std::move(variables), domains,
                        connectedEval, shortCircuiting, 0.0, 0.0) {
          evalCost = computeCost(subMonitor, numCombinations());
          satisfactionProbability = computeSatisfactionProbability(subMonitor, numCombinations());
}

Verdict UniversalMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    Verdict verdict{true};
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
        /*if (shortCircuiting)
            if (!subVerdict.value) {
                resetIterators();
                return subVerdict;
            }*/

        verdict = verdict && subVerdict;

        count++;
    }

    return verdict;
}

std::shared_ptr<HydraMonitor> UniversalMonitor::clone() {
    auto clonedMonitor = std::make_shared<UniversalMonitor>(*this);
    clonedMonitor->subMonitor = subMonitor->clone();

    clonedMonitor->iters.clear();
    for (auto &varDomain : clonedMonitor->varDomains)
        clonedMonitor->iters.push_back(varDomain.second.begin());

    return clonedMonitor;
}

std::string UniversalMonitor::to_string() { return formula.to_string(); }
