#include "universal_monitor.h"

#include <utility>

UniversalMonitor::UniversalMonitor(Formula formula, const std::shared_ptr<Hydra2Monitor> &subMonitor, size_t monitorId,
                                   const std::map<std::string, std::string> &variables, DomainCollection &domains,
                                   bool connectedEval, bool shortCircuiting)
    : QuantifierMonitor(std::move(formula), subMonitor, monitorId, variables, domains, connectedEval, shortCircuiting,
                        0.0, 0.0) {
    evalCost = computeCost(subMonitor, numberCombinations);
    satisfactionProbability = computeSatisfactionProbability(subMonitor, numberCombinations);
}

Verdict UniversalMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    Verdict verdict{true};
    if (traceIdx[parentId] < trace.getLength())
        verdict.timeStamp = trace.getEvent(traceIdx[parentId]).getTimeStep();
    else
        return Verdict{false};

    if (!checkNonEmptiness()) // if there is an empty domain, then a true verdict will be returned by definition
        return verdict;

    evalMonitors.reserve(numberCombinations);
    for (size_t count{0}; count != numberCombinations; count++) { // loop over all possible combinations
        std::shared_ptr<Hydra2Monitor> evalMonitor =
            nextEvalMonitor(parentId, count); // request monitor for current combination

        Verdict subVerdict = evalMonitor->step(monitorId, stepNumber);
        if (shortCircuiting)
            if (!subVerdict.value) // if a wrong verdict results, then the step evaluation will be stopped immediately
                return subVerdict;

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
