#include "Constant.h"

std::string Constant::to_string() const { return this->getSymbol(); }

std::shared_ptr<Expression> Constant::clone() { return std::make_shared<Constant>(*this); }

std::shared_ptr<Expression> Constant::accept(ExpressionVisitor &visitor, Trace &trace, int index) {
    return visitor.visitConstant(*this, trace, index);
}

bool Constant::getValue() const { return value; }
