#include "same_func_monitor.h"
#include "monitors/expression_tree/unary/first_order/function/same_func.h"

Verdict SameFuncMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    if (traceIdx[parentId] < trace.getLength()) {
        const SameFunc &sameFunc = *std::dynamic_pointer_cast<SameFunc>(formula.getExpr());
        auto event{trace.getEvent(traceIdx[parentId])};
        std::string leftValue;
        if (sameFunc.getLeftConst())
            leftValue = sameFunc.getLeft();
        else {
            auto iter = variableValues.find(sameFunc.getLeft());
            if (iter != variableValues.end())
                leftValue = iter->second.getVal();
            else
                std::runtime_error("Undefined variable " + sameFunc.getLeft() + "!");
        }
        std::string rightValue;
        if (sameFunc.getRightConst())
            rightValue = sameFunc.getRight();
        else {
            auto iter = variableValues.find(sameFunc.getRight());
            if (iter != variableValues.end())
                rightValue = iter->second.getVal();
            else
                std::runtime_error("Undefined variable " + sameFunc.getRight() + "!");
        }
        return Verdict{event.getTimeStep(), leftValue == rightValue};
    } else
        return Verdict{false};
}

void SameFuncMonitor::setTrace(const Trace &tra) { trace = tra; }

void SameFuncMonitor::setVariableValues(std::map<std::string, Value> values) { variableValues = values; }

void SameFuncMonitor::reset(std::optional<size_t> stepNumber) {
    HydraMonitor::reset(stepNumber);
    variableValues.clear();
}

std::string SameFuncMonitor::to_string() { return formula.to_string(); }
