#include "equality_operator_monitor.h"
#include "monitors/expression_tree/unary/first_order/function/equality_operator.h"

Verdict EqualityOperatorMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    if (stepNumber < trace.getLength()) {
        const EqualityOperator &equalOp = *std::dynamic_pointer_cast<EqualityOperator>(formula.getExpr());
        auto event{trace.getEvent(traceIdx[parentId])};

        std::string leftValue;
        if (equalOp.getLeftConst()) // extract value of left variable or constant
            leftValue = equalOp.getLeft();
        else {
            auto iter = variableValues.find(equalOp.getLeft());
            if (iter != variableValues.end())
                leftValue = iter->second.getVal();
            else
                std::runtime_error("Undefined left variable " + equalOp.getLeft() + "!");
        }

        std::string rightValue;
        if (equalOp.getRightConst()) // extract value of right variable or constant
            rightValue = equalOp.getRight();
        else {
            auto iter = variableValues.find(equalOp.getRight());
            if (iter != variableValues.end())
                rightValue = iter->second.getVal();
            else
                std::runtime_error("Undefined right variable " + equalOp.getRight() + "!");
        }

        return Verdict{event.getTimeStep(), leftValue == rightValue};
    } else
        return Verdict{false};
}

void EqualityOperatorMonitor::setTrace(const Trace &tra) { trace = tra; }

void EqualityOperatorMonitor::setVariableValues(std::map<std::string, Value> values) { variableValues = values; }

void EqualityOperatorMonitor::reset() {
    Hydra2Monitor::reset();
    variableValues.clear();
}

std::shared_ptr<Hydra2Monitor> EqualityOperatorMonitor::clone() {
    return std::make_shared<EqualityOperatorMonitor>(*this);
}

std::string EqualityOperatorMonitor::to_string() { return formula.to_string(); }
