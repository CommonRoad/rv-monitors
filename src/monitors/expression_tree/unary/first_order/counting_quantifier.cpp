#include "counting_quantifier.h"

ProbabilityInterval::ProbabilityInterval(float min, float max) : min(min), max(max)
// Constructor
{}

float ProbabilityInterval::getMin() const
// Returns minimum of probability.
{ return min; }

float ProbabilityInterval::getMax() const
// Returns maximum of probability.
{ return max; }

CountingQuantifier::CountingQuantifier(std::shared_ptr<Expression> &left, std::vector<Variable> &variables, ProbabilityInterval &probInterval)
    : Unary(countingSymbol, left), FirstOrderOperator(variables), probInterval(probInterval)
// First constructor
{}

CountingQuantifier::CountingQuantifier(std::shared_ptr<Expression> &left, Variable &variable, ProbabilityInterval &probInterval)
    : Unary(countingSymbol, left), FirstOrderOperator(variable), probInterval(probInterval)
// Second constructor
{}

std::string CountingQuantifier::to_string() const {
    std::string str = getSymbol() + "[" + std::to_string(getProbInterval().getMin()) + ", " +
        std::to_string(getProbInterval().getMax()) + "] ";

    str += FirstOrderOperator::to_string();

    str += getLeft()->to_string();

    return str;
}

std::shared_ptr<Expression> CountingQuantifier::clone() {
    return std::make_shared<CountingQuantifier>(*this);
}

std::shared_ptr<Expression> CountingQuantifier::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitCountingQuantifier(*this, trace, index);
}

std::shared_ptr<HydraMonitor> CountingQuantifier::accept(HydraCreationVisitor &visitor, size_t parentId, bool connectedEval) {
    return visitor.visitCountingQuantifier(*this, parentId, connectedEval);
}

const ProbabilityInterval &CountingQuantifier::getProbInterval() const
// Returns probability interval
{ return probInterval; }
