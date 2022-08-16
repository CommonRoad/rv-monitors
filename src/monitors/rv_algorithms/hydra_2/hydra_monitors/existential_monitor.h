#ifndef RVMONITORS_EXISTENTIAL_MONITOR_H
#define RVMONITORS_EXISTENTIAL_MONITOR_H

#include "quantifier_monitor.h"
#include "monitors/utils/utils.h"
#include "monitors/collections/value.h"

class ExistentialMonitor : public QuantifierMonitor {

  public:
    ExistentialMonitor(Formula formula, std::shared_ptr<HydraMonitor> subMonitor, size_t monitorId,
                       std::map<std::string, std::string> variables, DomainCollection &domains,
                       bool connectedEval, bool shortCircuiting);

    Verdict concreteStep(size_t parentId, size_t stepNumber) override;

    std::string to_string() override;

    static double computeCost(const std::shared_ptr<HydraMonitor> &monitor);

    static double computeSatisfactionProbability(const std::shared_ptr<HydraMonitor> &monitor);

};

#endif // RVMONITORS_EXISTENTIAL_MONITOR_H
