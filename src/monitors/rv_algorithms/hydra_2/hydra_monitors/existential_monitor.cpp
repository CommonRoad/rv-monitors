#include "monitors/rv_algorithms/hydra_2/hydra_monitors/existential_monitor.h"

#include <cmath>

ExistentialMonitor::ExistentialMonitor(const Formula &formula, const std::shared_ptr<Hydra2Monitor> &subMonitor,
                                       const size_t monitorId,
                                       std::map<std::string, std::shared_ptr<DomainStorage>> variableValues,
                                       const bool shortCircuiting)
    : QuantifierMonitor(formula, subMonitor, monitorId, variableValues, shortCircuiting, 0.0, 0.0) {
    evalCost = computeCost(subMonitor, numCombinations());
    satisfactionProbability = computeSatisfactionProbability(subMonitor, numCombinations());
}

Verdict ExistentialMonitor::quantifierStep(const size_t parentId, const size_t stepNumber,
                                           const size_t numberCombinations) {
    auto verdict{initializeStepFalse(parentId)};
    if (!verdict.timeStamp.has_value())
        return verdict;
    for (size_t count{0}; count != numberCombinations; count++) { // loop over all possible combinations
        const std::shared_ptr<Hydra2Monitor> evalMonitor =
            nextEvalMonitor(stepNumber); // request monitor for the current combination

        verdict = evalMonitor->step(monitorId, stepNumber);

        if (verdict.value) // if a true verdict results, then the step evaluation will be stopped immediately
            break;
    }
    return verdict;
}

std::shared_ptr<Hydra2Monitor> ExistentialMonitor::clone(const std::vector<std::string> &domValNames) {
    if (auto tmpCopy{Hydra2Monitor::clone(domValNames)}; tmpCopy != nullptr)
        return tmpCopy;
    const auto clonedMonitor = std::make_shared<ExistentialMonitor>(*this);
    clonedMonitor->subMonitor = subMonitor->clone(domValNames);
    clonedMonitor->resetIterators();
    return resetClonedMonitor(clonedMonitor, domValNames);
}

std::string ExistentialMonitor::to_string() { return formula.to_string(); }

double ExistentialMonitor::computeSatisfactionProbability(const std::shared_ptr<Hydra2Monitor> &monitor,
                                                          const size_t numEval) {
    return 1 - std::pow(1 - monitor->getSatisfactionProbability(), numEval);
}
