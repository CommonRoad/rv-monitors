#include "monitors/rv_algorithms/hydra_2/hydra_monitors/universal_monitor.h"

#include <utility>

UniversalMonitor::UniversalMonitor(Formula formula, const std::shared_ptr<Hydra2Monitor> &subMonitor, size_t monitorId,
                                   std::map<std::string, std::shared_ptr<DomainStorage>> variableValues,
                                   bool connectedEval, bool shortCircuiting)
    : QuantifierMonitor(std::move(formula), subMonitor, monitorId, variableValues, connectedEval, shortCircuiting, 0.0,
                        0.0) {
    auto numberCombinations{numCombinations()};
    evalCost = computeCost(subMonitor, numberCombinations);
    satisfactionProbability = computeSatisfactionProbability(subMonitor, numberCombinations);
}

Verdict UniversalMonitor::quantifierStep(size_t parentId, size_t stepNumber, size_t numberCombinations) {
    auto verdict{initializeStepTrue(parentId)};
    if (!verdict.timeStamp.has_value())
        return verdict;
    for (size_t count{0}; count != numberCombinations; count++) { // loop over all possible combinations
        std::shared_ptr<Hydra2Monitor> evalMonitor =
            nextEvalMonitor(parentId, count); // request monitor for current combination

        Verdict subVerdict = evalMonitor->step(monitorId, stepNumber);
        if (shortCircuiting)
            // if a wrong verdict results, then the step evaluation will be stopped immediately
            if (!subVerdict.value) {
                resetIterators();
                return subVerdict;
            }
        verdict = verdict && subVerdict; // all subverdicts have to be true so that a true verdict is returned
    }
    return verdict;
}

std::shared_ptr<Hydra2Monitor> UniversalMonitor::clone() {
    auto clonedMonitor = std::make_shared<UniversalMonitor>(*this);
    clonedMonitor->subMonitor = subMonitor->clone();
    clonedMonitor->resetIterators();
    return clonedMonitor;
}

std::string UniversalMonitor::to_string() { return formula.to_string(); }
