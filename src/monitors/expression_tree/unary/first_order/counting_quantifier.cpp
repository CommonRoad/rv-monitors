#include "counting_quantifier.h"

CountingQuantifier::CountingQuantifier(std::shared_ptr<Expression> &left, std::vector<Variable> &variables,
                                       CountingQuantifier::Operator op, size_t num)
    : Unary(countingSymbol, left), FirstOrderOperator(variables), op(op), num(num) {}

CountingQuantifier::CountingQuantifier(std::shared_ptr<Expression> &left, Variable &variable,
                                       CountingQuantifier::Operator op, size_t num)
    : Unary(countingSymbol, left), FirstOrderOperator(variable), op(op), num(num) {}

std::string CountingQuantifier::to_string() const {
    std::string opStr;
    if (op == Operator::LESS_EQ)
        opStr = "<=";
    else if (op == Operator::EQ)
        opStr = "=";
    else
        opStr = ">=";
    std::string str = getSymbol() + opStr + std::to_string(num) + " ";

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

std::shared_ptr<Hydra2Monitor> CountingQuantifier::accept(HydraCreationVisitor2 &visitor, size_t parentId,
                                                         bool connectedEval) {
    return visitor.visitCountingQuantifier(*this, parentId, connectedEval);
}

const CountingQuantifier::Operator &CountingQuantifier::getOperator() const { return op; }

size_t CountingQuantifier::getNum() const { return num; }
