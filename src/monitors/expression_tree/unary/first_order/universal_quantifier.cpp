#include "universal_quantifier.h"

UniversalQuantifier::UniversalQuantifier(std::shared_ptr<Expression> &left, std::vector<std::shared_ptr<Variable>> &variables)
    : Unary(universalSymbol, left), FirstOrderOperator(variables)
// First constructor
{}

UniversalQuantifier::UniversalQuantifier(std::shared_ptr<Expression> &left, std::shared_ptr<Variable> &variable)
    : Unary(universalSymbol, left), FirstOrderOperator(variable)
// Second constructor
{}

std::string UniversalQuantifier::to_string() const {
    std::string str = getSymbol() + " ";

    str += FirstOrderOperator::to_string();

    str += getLeft()->to_string();

    return str;
}

std::shared_ptr<Expression> UniversalQuantifier::clone() {
    return std::make_shared<UniversalQuantifier>(*this);
}

std::shared_ptr<Expression> UniversalQuantifier::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitUniversalQuantifier(*this, trace, index);
}

std::shared_ptr<HydraMonitor> UniversalQuantifier::accept(HydraCreationVisitor &visitor, size_t parentId, bool connectedEval) {
    return visitor.visitUniversalQuantifier(*this, parentId, connectedEval);
}
