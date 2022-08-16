#ifndef RVMONITORS_COUNTING_MONITOR_H
#define RVMONITORS_COUNTING_MONITOR_H

#include "quantifier_monitor.h"
#include "monitors/utils/utils.h"
#include "monitors/collections/value.h"

class CountingMonitor : public QuantifierMonitor {

  public:
    CountingMonitor(Formula formula, std::shared_ptr<HydraMonitor> subMonitor, size_t monitorId,
                    std::map<std::string, std::string> variables, DomainCollection &domains,
                    float minProbability, float maxProbability, bool connectedEval, bool shortCircuiting);

    float computeRatio(size_t trueVerdicts);

    Verdict concreteStep(size_t parentId, size_t stepNumber) override;

    std::string to_string() override;

    static double computeCost(const std::shared_ptr<HydraMonitor> &monitor);

    static double computeSatisfactionProbability(const std::shared_ptr<HydraMonitor> &monitor);

  private:
    float minProbability;
    float maxProbability;

};

#endif // RVMONITORS_COUNTING_MONITOR_H
