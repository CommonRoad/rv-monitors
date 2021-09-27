#include "constant.h"

std::string Constant::to_string() const { return this->getSymbol(); }

std::shared_ptr<Expression> Constant::clone() { return std::make_shared<Constant>(*this); }

std::shared_ptr<Expression> Constant::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitConstant(*this, trace, index);
}

std::shared_ptr<HydraMonitor> Constant::accept(HydraCreationVisitor &visitor) {
    return visitor.visitConstant(*this);
}

bool Constant::getValue() const { return value; }
