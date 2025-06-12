#include "monitors/rv_algorithms/hydra_2/hydra_monitors/counting_monitor.h"
#include <cmath>

CountingMonitor::CountingMonitor(Formula formula, const std::shared_ptr<Hydra2Monitor> &subMonitor,
                                 const size_t monitorId,
                                 std::map<std::string, std::shared_ptr<DomainStorage>> variableValues,
                                 const CountingQuantifier::Operator opr, const size_t num, const bool shortCircuiting)
    : QuantifierMonitor(std::move(formula), subMonitor, monitorId, variableValues, shortCircuiting, 0.0, 0.0), op(opr),
      num(num) {
    const auto numberCombinations{numCombinations()};
    evalCost = computeCost(subMonitor, numberCombinations);
    satisfactionProbability = computeSatisfactionProbability(subMonitor, numberCombinations);
}

Verdict CountingMonitor::quantifierStep(const size_t parentId, const size_t stepNumber,
                                        const size_t numberCombinations) {
    auto verdict{initializeStepFalse(parentId)};
    if (!verdict.timeStamp.has_value())
        return verdict;
    size_t trueVerdicts{0};
    for (size_t count{0}; count != numberCombinations; count++) { // loop over all possible combinations
        const std::shared_ptr<Hydra2Monitor> evalMonitor =
            nextEvalMonitor(stepNumber); // request monitor for the current combination

        if (const Verdict subVerdict = evalMonitor->step(monitorId, stepNumber);
            subVerdict.value) // count number of true verdicts
            trueVerdicts++;

        if (shortCircuiting) {
            const size_t unevalVerdicts{numberCombinations - count - 1}; // unevaluated verdicts
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

std::shared_ptr<Hydra2Monitor> CountingMonitor::clone(const std::vector<std::string> &domValNames) {
    if (auto tmpCopy{Hydra2Monitor::clone(domValNames)}; tmpCopy != nullptr)
        return tmpCopy;
    const auto clonedMonitor = std::make_shared<CountingMonitor>(*this);
    clonedMonitor->subMonitor = subMonitor->clone(domValNames);
    clonedMonitor->resetIterators();
    return resetClonedMonitor(clonedMonitor, domValNames);
}

std::string CountingMonitor::to_string() { return formula.to_string(); }

unsigned long long CountingMonitor::binomialCoefficient(const size_t n, const size_t k) {
    if (k > n)
        return 0;
    if (k == 0 || k == n)
        return 1;

    unsigned long long result = 1;
    for (int i = 1; i <= k; ++i) {
        result *= n - (k - i);
        result /= i;
    }
    return result;
}

double CountingMonitor::computeSatisfactionProbability(const std::shared_ptr<Hydra2Monitor> &monitor,
                                                       const size_t numEval) {
    double probability = 0.0;

    for (size_t k = num; k <= numEval; ++k) {
        const unsigned long long binom = binomialCoefficient(num, k);
        const double term = static_cast<double>(binom) * std::pow(monitor->getSatisfactionProbability(), k) *
                            std::pow(1 - monitor->getSatisfactionProbability(), numEval - k);
        probability += term;
    }

    return probability;
}
