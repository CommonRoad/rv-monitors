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

    if (!checkNonEmptiness()) // if there is an empty domain, then a false verdict will be returned by definition
        return verdict;

    size_t numComb{numCombinations()};

    size_t count{0};
    while (count != numComb) { // loop over all possible combinations
        std::shared_ptr<HydraMonitor> evalMonitor = nextEvalMonitor(parentId, count); // request monitor for the current combination

        verdict = evalMonitor->step(monitorId, stepNumber);

        if (verdict.value) // if a true verdict results, then the step evaluation will be stopped immediately
            break;

        count++;
    }

    return verdict;
}

std::shared_ptr<HydraMonitor> ExistentialMonitor::clone() {
    auto clonedMonitor = std::make_shared<ExistentialMonitor>(*this);
    clonedMonitor->subMonitor = subMonitor->clone();
    clonedMonitor->resetIterators();
    return clonedMonitor;
}

std::string ExistentialMonitor::to_string() { return formula.to_string(); }
