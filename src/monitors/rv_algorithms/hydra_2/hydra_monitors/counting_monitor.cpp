#include "monitors/rv_algorithms/hydra_2/hydra_monitors/counting_monitor.h"

CountingMonitor::CountingMonitor(Formula formula, const std::shared_ptr<Hydra2Monitor> &subMonitor, size_t monitorId,
                                 std::map<std::string, std::shared_ptr<DomainStorage>> variableValues,
                                 CountingQuantifier::Operator opr, size_t num, bool connectedEval, bool shortCircuiting)
    : QuantifierMonitor(std::move(formula), subMonitor, monitorId, variableValues, connectedEval, shortCircuiting, 0.0,
                        0.0),
      op(opr), num(num) {
    auto numberCombinations{numCombinations()};
    evalCost = computeCost(subMonitor, numberCombinations);
    satisfactionProbability = computeSatisfactionProbability(subMonitor, numberCombinations);
}

Verdict CountingMonitor::quantifierStep(size_t parentId, size_t stepNumber, size_t numberCombinations) {
    auto verdict{initializeStepFalse(parentId)};
    if (!verdict.timeStamp.has_value())
        return verdict;
    size_t trueVerdicts{0};
    for (size_t count{0}; count != numberCombinations; count++) { // loop over all possible combinations
        std::shared_ptr<Hydra2Monitor> evalMonitor =
            nextEvalMonitor(parentId, count); // request monitor for the current combination

        Verdict subVerdict = evalMonitor->step(monitorId, stepNumber);
        if (subVerdict.value) // count number of true verdicts
            trueVerdicts++;

        if (shortCircuiting) {
            size_t unevalVerdicts{numberCombinations - count - 1}; // unevaluated verdicts
            if (op == CountingQuantifier::LESS_EQ) {
                if (unevalVerdicts + trueVerdicts <= num or trueVerdicts > num) {
                    if (unevalVerdicts + trueVerdicts <= num)
                        verdict.value = true;
                    resetIterators();
                    return verdict;
                }
            } else if (op == CountingQuantifier::EQ) {
                if (trueVerdicts > num or unevalVerdicts + trueVerdicts < num) {
                    resetIterators();
                    return verdict;
                }
            } else {
                if (trueVerdicts >= num or unevalVerdicts + trueVerdicts < num) {
                    if (trueVerdicts >= num)
                        verdict.value = true;
                    resetIterators();
                    return verdict;
                }
            }
        }
    }
    verdict.value = (op == CountingQuantifier::LESS_EQ and trueVerdicts <= num) or
                    (op == CountingQuantifier::EQ and trueVerdicts == num) or
                    (op == CountingQuantifier::GREATER_EQ and trueVerdicts >= num);

    return verdict;
}

std::shared_ptr<Hydra2Monitor> CountingMonitor::clone() {
    auto clonedMonitor = std::make_shared<CountingMonitor>(*this);
    clonedMonitor->subMonitor = subMonitor->clone();
    clonedMonitor->resetIterators();
    return clonedMonitor;
}

std::string CountingMonitor::to_string() { return formula.to_string(); }
