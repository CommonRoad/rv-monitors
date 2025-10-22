#include "monitors/rv_algorithms/hydra_2/hydra_monitors/quote_monitor.h"

#include <cmath>
QuoteMonitor::QuoteMonitor(Formula formula, const std::shared_ptr<Hydra2Monitor> &subMonitor, const size_t monitorId,
                           std::map<std::string, std::shared_ptr<DomainStorage>> variableValues, const float minQuote,
                           const float maxQuote, const bool shortCircuiting)
    : QuantifierMonitor(std::move(formula), subMonitor, monitorId, variableValues, shortCircuiting, 0.0, 0.0),
      minQuote(minQuote), maxQuote(maxQuote) {
    const auto numberCombinations{numCombinations()};
    evalCost = computeCost(subMonitor, numberCombinations);
    satisfactionProbability = computeSatisfactionProbability(subMonitor, numberCombinations);
}

float QuoteMonitor::computeRatio(const size_t trueVerdicts) const {
    return static_cast<float>(trueVerdicts) / static_cast<float>(numCombinations());
}

Verdict QuoteMonitor::quantifierStep(const size_t parentId, const size_t stepNumber, const size_t numberCombinations) {
    auto verdict{initializeStepTrue(stepNumber)};
    if (!verdict.timeStamp.has_value())
        return verdict;
    float totalQuote;
    size_t trueVerdicts{0};
    for (size_t count{0}; count != numberCombinations; count++) { // loop over all possible combinations
        const std::shared_ptr<Hydra2Monitor> evalMonitor =
            nextEvalMonitor(stepNumber); // request monitor for the current combination

        if (const Verdict subVerdict = evalMonitor->step(monitorId, stepNumber);
            subVerdict.value) // count number of true verdicts
            trueVerdicts++;

        if (shortCircuiting) {
            totalQuote = computeRatio(trueVerdicts); // current quote of true verdicts
            const float possibleQuote =
                computeRatio(trueVerdicts + numberCombinations - count - 1); // still reachable quote of true verdicts
            if (totalQuote > maxQuote or possibleQuote < minQuote) {
                verdict.value = false;
                resetIterators();
                return verdict;
            }
            if (minQuote <= totalQuote and totalQuote <= maxQuote) {
                if (possibleQuote <= maxQuote) {
                    resetIterators();
                    return verdict;
                }
            }
        }
    }
    totalQuote = computeRatio(trueVerdicts);
    if (minQuote > totalQuote or maxQuote < totalQuote) // if quote of true verdicts is outside the quote interval, then
                                                        // a false verdict will be returned
        verdict.value = false;

    return verdict;
}

std::shared_ptr<Hydra2Monitor> QuoteMonitor::clone(const std::vector<std::string> &domValNames) {
    if (auto tmpCopy{Hydra2Monitor::clone(domValNames)}; tmpCopy != nullptr)
        return tmpCopy;
    const auto clonedMonitor = std::make_shared<QuoteMonitor>(*this);
    clonedMonitor->subMonitor = subMonitor->clone(domValNames);
    clonedMonitor->resetIterators();
    return resetClonedMonitor(clonedMonitor, domValNames);
}

std::string QuoteMonitor::to_string() { return formula.to_string(); }

unsigned long long QuoteMonitor::binomialCoefficient(const size_t n, const size_t k) {
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

double QuoteMonitor::computeSatisfactionProbability(const std::shared_ptr<Hydra2Monitor> &monitor,
                                                    const size_t numEval) {
    double probability = 0.0;
    const size_t num = static_cast<int>(std::ceil(minQuote * static_cast<double>(numEval)));

    for (size_t k = num; k <= numEval; ++k) {
        const unsigned long long binom = binomialCoefficient(num, k);
        const double term = static_cast<double>(binom) * std::pow(monitor->getSatisfactionProbability(), k) *
                            std::pow(1 - monitor->getSatisfactionProbability(), numEval - k);
        probability += term;
    }

    return probability;
}
