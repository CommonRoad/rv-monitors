#pragma once

#include "hydra_monitor.h"

class SameFuncMonitor : public HydraMonitor {

  public:
    SameFuncMonitor(Formula formula, size_t monitorId, bool connectedEval, double evalCost = 0, double satisfactionProbability = 100.0)
        : HydraMonitor(std::move(formula), monitorId, connectedEval, evalCost, satisfactionProbability) {}

    Verdict concreteStep(size_t parentId, size_t stepNumber) override;

    void setTrace(const Trace &tra) override;

    void setVariableValues(std::map<std::string, Value> values) override;

    void reset(std::optional<size_t> stepNumber = std::nullopt) override;

    std::string to_string() override;

  private:
    Trace trace;
    std::map<std::string, Value> variableValues;

};
