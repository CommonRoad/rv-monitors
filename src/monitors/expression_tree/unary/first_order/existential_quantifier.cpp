#include "existential_quantifier.h"

ExistentialQuantifier::ExistentialQuantifier(std::shared_ptr<Expression> &left, std::vector<Variable> &variables)
    : Unary(existentialSymbol, left), FirstOrderOperator(variables)
// First constructor
{}

ExistentialQuantifier::ExistentialQuantifier(std::shared_ptr<Expression> &left, Variable &variable)
    : Unary(existentialSymbol, left), FirstOrderOperator(variable)
// Second constructor
{}

std::string ExistentialQuantifier::to_string() const {
    std::string str = getSymbol() + " ";

    str += FirstOrderOperator::to_string();

    str += getLeft()->to_string();

    return str;
}

std::shared_ptr<Expression> ExistentialQuantifier::clone() {
    return std::make_shared<ExistentialQuantifier>(*this);
}

std::shared_ptr<Expression> ExistentialQuantifier::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitExistentialQuantifier(*this, trace, index);
}

std::shared_ptr<Hydra2Monitor> ExistentialQuantifier::accept(HydraCreationVisitor2 &visitor, size_t parentId, bool connectedEval) {
    return visitor.visitExistentialQuantifier(*this, parentId, connectedEval);
}
