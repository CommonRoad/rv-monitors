#include "quote_monitor.h"
QuoteMonitor::QuoteMonitor(Formula formula, std::shared_ptr<Hydra2Monitor> subMonitor, size_t monitorId,
                           std::map<std::string, std::string> variables, DomainCollection &domains, float minQuote,
                           float maxQuote, bool connectedEval, bool shortCircuiting)
    : QuantifierMonitor(std::move(formula), subMonitor, monitorId, std::move(variables), domains, connectedEval,
                        shortCircuiting, 0.0, 0.0),
      minQuote(minQuote), maxQuote(maxQuote) {
    evalCost = computeCost(subMonitor, numberCombinations);
    satisfactionProbability = computeSatisfactionProbability(subMonitor, numberCombinations);
}

float QuoteMonitor::computeRatio(size_t trueVerdicts) {
    return static_cast<float>(trueVerdicts) / static_cast<float>(numberCombinations);
}

Verdict QuoteMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    Verdict verdict{true};
    if (traceIdx[parentId] < trace.getLength() and minQuote <= maxQuote)
        verdict.timeStamp = trace.getEvent(traceIdx[parentId]).getTimeStep();
    else
        return Verdict{false};

    if (!checkNonEmptiness()) // if there is an empty domain, then a true verdict will be returned by definition
        return verdict;

    float totalQuote;

    size_t trueVerdicts{0};
    evalMonitors.reserve(numberCombinations);
    for (size_t count{0}; count != numberCombinations; count++) { // loop over all possible combinations
        std::shared_ptr<Hydra2Monitor> evalMonitor =
            nextEvalMonitor(parentId, count); // request monitor for the current combination

        Verdict subVerdict = evalMonitor->step(monitorId, stepNumber);
        if (subVerdict.value) // count number of true verdicts
            trueVerdicts++;

        if (shortCircuiting) {
            totalQuote = computeRatio(trueVerdicts); // current quote of true verdicts
            float possibleQuote =
                computeRatio(trueVerdicts + numberCombinations - count - 1); // still reachable quote of true verdicts
            if (totalQuote > maxQuote or possibleQuote < minQuote) {
                verdict.value = false;
                return verdict;
            } else if (minQuote <= totalQuote and totalQuote <= maxQuote) {
                if (possibleQuote <= maxQuote) {
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

std::shared_ptr<Hydra2Monitor> QuoteMonitor::clone() {
    auto clonedMonitor = std::make_shared<QuoteMonitor>(*this);
    clonedMonitor->subMonitor = subMonitor->clone();
    clonedMonitor->resetIterators();
    return clonedMonitor;
}

std::string QuoteMonitor::to_string() { return formula.to_string(); }
