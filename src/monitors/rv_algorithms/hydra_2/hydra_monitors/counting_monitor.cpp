#include "counting_monitor.h"

CountingMonitor::CountingMonitor(Formula formula, std::shared_ptr<HydraMonitor> subMonitor, size_t monitorId,
                                 std::map<std::string, std::string> variables, DomainCollection &domains,
                                 CountingQuantifier::Operator op, size_t num, bool connectedEval,
                                 bool shortCircuiting)
    : QuantifierMonitor(std::move(formula), subMonitor, monitorId, std::move(variables), domains,
                        connectedEval, shortCircuiting, 0.0, 0.0),
    op(op), num(num) {
    evalCost = computeCost(subMonitor, numCombinations());
    satisfactionProbability = computeSatisfactionProbability(subMonitor, numCombinations());
}

Verdict CountingMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    Verdict verdict{false};
    if (traceIdx[parentId] < trace.getLength())
        verdict.timeStamp = trace.getEvent(traceIdx[parentId]).getTimeStep();
    else
        return verdict;

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
            size_t unevalVerdicts{numCombs - combinationCounter};
            if (op == CountingQuantifier::LESS_EQ) {
                if (unevalVerdicts + trueVerdicts <= num) {
                    verdict.timeStamp = true;
                    return verdict;
                } else if (trueVerdicts > num)
                    return verdict;
            } else if (op == CountingQuantifier::EQ) {
                if (trueVerdicts > num)
                    return verdict;
            } else {
                if (trueVerdicts >= num) {
                    verdict.value = true;
                    return verdict;
                } else if (unevalVerdicts + trueVerdicts < num)
                    return verdict;
            }
        }*/
        count++;
    }

    verdict.value = (op == CountingQuantifier::LESS_EQ and trueVerdicts <= num) or
                    (op == CountingQuantifier::EQ and trueVerdicts == num) or
                    (op == CountingQuantifier::GREATER_EQ and trueVerdicts >= num);

    return verdict;
}

std::shared_ptr<HydraMonitor> CountingMonitor::clone() {
    auto clonedMonitor = std::make_shared<CountingMonitor>(*this);
    clonedMonitor->subMonitor = subMonitor->clone();

    clonedMonitor->iters.clear();
    for (auto &varDomain : clonedMonitor->varDomains)
        clonedMonitor->iters.push_back(varDomain.second.begin());

    return clonedMonitor;
}

std::string CountingMonitor::to_string() { return formula.to_string(); }
