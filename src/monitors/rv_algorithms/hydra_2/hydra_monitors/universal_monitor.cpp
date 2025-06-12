#include "monitors/rv_algorithms/hydra_2/hydra_monitors/universal_monitor.h"

#include <cmath>
#include <utility>

UniversalMonitor::UniversalMonitor(Formula formula, const std::shared_ptr<Hydra2Monitor> &subMonitor,
                                   const size_t monitorId,
                                   std::map<std::string, std::shared_ptr<DomainStorage>> variableValues,
                                   const bool shortCircuiting)
    : QuantifierMonitor(std::move(formula), subMonitor, monitorId, variableValues, shortCircuiting, 0.0, 0.0) {
    const auto numberCombinations{numCombinations()};
    evalCost = computeCost(subMonitor, numberCombinations);
    satisfactionProbability = computeSatisfactionProbability(subMonitor, numberCombinations);
}

Verdict UniversalMonitor::quantifierStep(const size_t parentId, const size_t stepNumber,
                                         const size_t numberCombinations) {
    auto verdict{initializeStepTrue(parentId)};
    std::vector<std::vector<DomainValue>> violationsTmp;
    if (!verdict.timeStamp.has_value())
        return verdict;
    for (size_t count{0}; count != numberCombinations; count++) { // loop over all possible combinations
        const std::shared_ptr<Hydra2Monitor> evalMonitor =
            nextEvalMonitor(stepNumber); // request monitor for current combination

        Verdict subVerdict = evalMonitor->step(monitorId, stepNumber);
        if (shortCircuiting and not storeVariableResults)
            // if a wrong verdict results, then the step evaluation will be stopped immediately
            if (!subVerdict.value) {
                resetIterators();
                return subVerdict;
            }
        if (storeVariableResults and !subVerdict.value) {
            violationsTmp.push_back(currentDomainValues);
        }
        verdict = verdict && subVerdict; // all sub-verdicts have to be true so that a true verdict is returned
    }

    if (storeVariableResults and !violationsTmp.empty())
        violations.insert({verdict.timeStamp.value(), violationsTmp});

    return verdict;
}

std::shared_ptr<Hydra2Monitor> UniversalMonitor::clone(const std::vector<std::string> &domValNames) {
    if (auto tmpCopy{Hydra2Monitor::clone(domValNames)}; tmpCopy != nullptr)
        return tmpCopy;
    const auto clonedMonitor = std::make_shared<UniversalMonitor>(*this);
    clonedMonitor->subMonitor = subMonitor->clone(domValNames);
    clonedMonitor->resetIterators();
    return resetClonedMonitor(clonedMonitor, domValNames);
}

std::string UniversalMonitor::to_string() { return formula.to_string(); }

double UniversalMonitor::computeSatisfactionProbability(const std::shared_ptr<Hydra2Monitor> &monitor,
                                                        const size_t numEval) {
    return std::pow(monitor->getSatisfactionProbability(), numEval);
}
