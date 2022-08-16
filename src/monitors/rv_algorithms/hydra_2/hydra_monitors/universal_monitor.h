#ifndef RVMONITORS_UNIVERSAL_MONITOR_H
#define RVMONITORS_UNIVERSAL_MONITOR_H

#include "quantifier_monitor.h"
#include "monitors/utils/utils.h"
#include "monitors/collections/value.h"

class UniversalMonitor : public QuantifierMonitor {

  public:
    UniversalMonitor(Formula formula, std::shared_ptr<HydraMonitor> subMonitor, size_t monitorId,
                     std::map<std::string, std::string> variables, DomainCollection &domains,
                     bool connectedEval, bool shortCircuiting);

    Verdict concreteStep(size_t parentId, size_t stepNumber) override;

    std::string to_string() override;

    static double computeCost(const std::shared_ptr<HydraMonitor> &monitor);

    static double computeSatisfactionProbability(const std::shared_ptr<HydraMonitor> &monitor);

};

#endif // RVMONITORS_UNIVERSAL_MONITOR_H
