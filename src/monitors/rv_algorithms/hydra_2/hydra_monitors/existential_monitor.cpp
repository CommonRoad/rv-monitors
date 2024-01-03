#include "monitors/rv_algorithms/hydra_2/hydra_monitors/existential_monitor.h"

ExistentialMonitor::ExistentialMonitor(const Formula &formula, const std::shared_ptr<Hydra2Monitor> &subMonitor,
                                       size_t monitorId,
                                       std::map<std::string, std::shared_ptr<DomainStorage>> variableValues,
                                       bool connectedEval, bool shortCircuiting)
    : QuantifierMonitor(formula, subMonitor, monitorId, variableValues, connectedEval, shortCircuiting, 0.0, 0.0) {
    evalCost = computeCost(subMonitor, numCombinations());
    satisfactionProbability = computeSatisfactionProbability(subMonitor, numCombinations());
}

Verdict ExistentialMonitor::quantifierStep(size_t parentId, size_t stepNumber, size_t numberCombinations) {
    auto verdict{initializeStepFalse(parentId)};
    if (!verdict.timeStamp.has_value())
        return verdict;
    for (size_t count{0}; count != numberCombinations; count++) { // loop over all possible combinations
        std::shared_ptr<Hydra2Monitor> evalMonitor =
            nextEvalMonitor(parentId, count); // request monitor for the current combination

        verdict = evalMonitor->step(monitorId, stepNumber, true);

        if (verdict.value) // if a true verdict results, then the step evaluation will be stopped immediately
            break;
    }
    return verdict;
}

std::shared_ptr<Hydra2Monitor> ExistentialMonitor::clone() {
    auto clonedMonitor = std::make_shared<ExistentialMonitor>(*this);
    clonedMonitor->subMonitor = subMonitor->clone();
    clonedMonitor->resetIterators();
    return clonedMonitor;
}

std::string ExistentialMonitor::to_string() { return formula.to_string(); }
