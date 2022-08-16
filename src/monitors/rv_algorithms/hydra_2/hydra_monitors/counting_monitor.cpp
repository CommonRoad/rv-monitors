#include "counting_monitor.h"
CountingMonitor::CountingMonitor(Formula formula, std::shared_ptr<HydraMonitor> subMonitor, size_t monitorId,
                                 std::map<std::string, std::string> variables, DomainCollection &domains,
                                 float minProbability, float maxProbability, bool connectedEval, bool shortCircuiting)
    : QuantifierMonitor(std::move(formula), subMonitor, monitorId, std::move(variables), domains,
                        connectedEval, shortCircuiting, computeCost(subMonitor), computeSatisfactionProbability(subMonitor)),
    minProbability(minProbability), maxProbability(maxProbability) {}

float CountingMonitor::computeRatio(size_t trueVerdicts) {
    return static_cast<float>(trueVerdicts) / static_cast<float>(numCombinations());
}

Verdict CountingMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    Verdict verdict{true};
    if (traceIdx[parentId] < trace.getLength() and minProbability <= maxProbability)
        verdict.ts = trace.getEvent(traceIdx[parentId]).getTimeStep();
    else
        return Verdict{false};

    if (!checkNonEmptiness())
        return verdict;

    size_t numCombs{numCombinations()};

    float totalProbability;

    size_t combinationCounter{0};
    size_t trueVerdicts{0};
    while (nextCombination()) {
        Verdict subVerdict = subMonitor->step(monitorId, stepNumber);
        if (subVerdict.b)
            trueVerdicts++;
        subMonitor->reset(traceIdx[parentId]);

        if (shortCircuiting) {
            combinationCounter++;
            totalProbability = computeRatio(trueVerdicts);
            float possibleProbability = computeRatio(trueVerdicts + numCombs - combinationCounter);
            if (totalProbability > maxProbability or possibleProbability < minProbability) {
                resetIterators();
                verdict.b = false;
                return verdict;
            } else if (minProbability <= totalProbability and totalProbability <= maxProbability) {
                if (possibleProbability <= maxProbability) {
                    resetIterators();
                    return verdict;
                }
            }
        }
    }

    totalProbability = computeRatio(trueVerdicts);
    if (minProbability > totalProbability or maxProbability < totalProbability)
        verdict.b = false;

    subMonitor->reset(traceIdx[parentId] + 1);
    return verdict;
}

std::string CountingMonitor::to_string() { return formula.to_string(); }

double CountingMonitor::computeCost(const std::shared_ptr<HydraMonitor> &monitor) { return 0; }

double CountingMonitor::computeSatisfactionProbability(const std::shared_ptr<HydraMonitor> &monitor) { return 0; }
