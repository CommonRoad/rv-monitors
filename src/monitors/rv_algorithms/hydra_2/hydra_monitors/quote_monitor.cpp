#include "monitors/rv_algorithms/hydra_2/hydra_monitors/quote_monitor.h"
QuoteMonitor::QuoteMonitor(Formula formula, const std::shared_ptr<Hydra2Monitor> &subMonitor, size_t monitorId,
                           std::map<std::string, std::shared_ptr<DomainStorage>> variableValues, float minQuote,
                           float maxQuote, bool connectedEval, bool shortCircuiting)
    : QuantifierMonitor(std::move(formula), subMonitor, monitorId, variableValues, connectedEval, shortCircuiting, 0.0,
                        0.0),
      minQuote(minQuote), maxQuote(maxQuote) {
    auto numberCombinations{numCombinations()};
    evalCost = computeCost(subMonitor, numberCombinations);
    satisfactionProbability = computeSatisfactionProbability(subMonitor, numberCombinations);
}

float QuoteMonitor::computeRatio(size_t trueVerdicts) {
    return static_cast<float>(trueVerdicts) / static_cast<float>(numCombinations());
}

Verdict QuoteMonitor::quantifierStep(size_t parentId, size_t stepNumber, size_t numberCombinations) {
    auto verdict{initializeStepTrue(parentId)};
    if (!verdict.timeStamp.has_value())
        return verdict;
    float totalQuote;
    size_t trueVerdicts{0};
    for (size_t count{0}; count != numberCombinations; count++) { // loop over all possible combinations
        std::shared_ptr<Hydra2Monitor> evalMonitor =
            nextEvalMonitor(stepNumber); // request monitor for the current combination

        Verdict subVerdict = evalMonitor->step(monitorId, stepNumber);
        if (subVerdict.value) // count number of true verdicts
            trueVerdicts++;

        if (shortCircuiting) {
            totalQuote = computeRatio(trueVerdicts); // current quote of true verdicts
            float possibleQuote =
                computeRatio(trueVerdicts + numberCombinations - count - 1); // still reachable quote of true verdicts
            if (totalQuote > maxQuote or possibleQuote < minQuote) {
                verdict.value = false;
                resetIterators();
                return verdict;
            } else if (minQuote <= totalQuote and totalQuote <= maxQuote) {
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
    auto tmpCopy{Hydra2Monitor::clone(domValNames)};
    if (tmpCopy != nullptr)
        return tmpCopy;
    auto clonedMonitor = std::make_shared<QuoteMonitor>(*this);
    clonedMonitor->subMonitor = subMonitor->clone(domValNames);
    clonedMonitor->resetIterators();
    allCopies.emplace_back(domValNames, clonedMonitor);
    return clonedMonitor;
}

std::string QuoteMonitor::to_string() { return formula.to_string(); }
