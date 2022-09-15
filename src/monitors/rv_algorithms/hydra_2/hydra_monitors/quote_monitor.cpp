#include "quote_monitor.h"
QuoteMonitor::QuoteMonitor(Formula formula, std::shared_ptr<HydraMonitor> subMonitor, size_t monitorId,
                                 std::map<std::string, std::string> variables, DomainCollection &domains,
                                 float minQuote, float maxQuote, bool connectedEval, bool shortCircuiting)
    : QuantifierMonitor(std::move(formula), subMonitor, monitorId, std::move(variables), domains,
                        connectedEval, shortCircuiting, 0.0, 0.0),
      minQuote(minQuote), maxQuote(maxQuote) {
    evalCost = computeCost(subMonitor, numCombinations());
    satisfactionProbability = computeSatisfactionProbability(subMonitor, numCombinations());
}

float QuoteMonitor::computeRatio(size_t trueVerdicts) {
    return static_cast<float>(trueVerdicts) / static_cast<float>(numCombinations());
}

Verdict QuoteMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    Verdict verdict{true};
    if (traceIdx[parentId] < trace.getLength() and minQuote <= maxQuote)
        verdict.timeStamp = trace.getEvent(traceIdx[parentId]).getTimeStep();
    else
        return Verdict{false};

    if (!checkNonEmptiness())
        return verdict;

    size_t count{0};
    size_t trueVerdicts{0};
    while (nextCombination()) {
        std::shared_ptr<HydraMonitor> evalMonitor;
        if (traceIdx[parentId] == 0) {
            evalMonitor = subMonitor->clone();
            evalMonitors.push_back(evalMonitor);
        } else
            evalMonitor = evalMonitors[count];

        Verdict subVerdict = evalMonitor->step(monitorId, stepNumber);
        if (subVerdict.value)
            trueVerdicts++;

        /*if (shortCircuiting) {
            combinationCounter++;
            totalQuote = computeRatio(trueVerdicts);
            float possibleProbability = computeRatio(trueVerdicts + numCombs - combinationCounter);
            if (totalQuote > maxQuote or possibleProbability < minQuote) {
                resetIterators();
                verdict.value = false;
                return verdict;
            } else if (minQuote <= totalQuote and totalQuote <= maxQuote) {
                if (possibleProbability <= maxQuote) {
                    resetIterators();
                    return verdict;
                }
            }
        }*/
        count++;
    }

    float totalQuote = computeRatio(trueVerdicts);
    if (minQuote > totalQuote or maxQuote < totalQuote)
        verdict.value = false;

    return verdict;
}

std::shared_ptr<HydraMonitor> QuoteMonitor::clone() {
    auto clonedMonitor = std::make_shared<QuoteMonitor>(*this);
    clonedMonitor->subMonitor = subMonitor->clone();

    clonedMonitor->iters.clear();
    for (auto &varDomain : clonedMonitor->varDomains)
        clonedMonitor->iters.push_back(varDomain.second.begin());

    return clonedMonitor;
}

std::string QuoteMonitor::to_string() { return formula.to_string(); }
