#ifndef RVMONITORS_QUANTIFIER_MONITOR_H
#define RVMONITORS_QUANTIFIER_MONITOR_H

#include "hydra_monitor.h"

class QuantifierMonitor : public HydraMonitor {

  public:
    QuantifierMonitor(Formula formula, std::shared_ptr<HydraMonitor> subMonitor, size_t monitorId,
                      std::map<std::string, std::string> variables, DomainCollection &domains,
                      bool connectedEval, bool shortCircuiting, double cost, double satisfactionProbability);

    bool nextCombination();

    void resetIterators();

    size_t numCombinations() const;

    bool checkNonEmptiness() const;

    void setTrace(const Trace &tra) override;

    void setVariableValues(std::map<std::string, Value> values) override;

    void reset(std::optional<size_t> stepNumber = std::nullopt) override;

  protected:
    std::shared_ptr<HydraMonitor> subMonitor;
    bool shortCircuiting;
    std::map<std::string, Value> variableValues;
    Trace trace;

    std::vector<std::pair<std::string, Domain>> varDomains;
    std::vector<std::vector<Value>::iterator> iters;

};

#endif // RVMONITORS_QUANTIFIER_MONITOR_H
