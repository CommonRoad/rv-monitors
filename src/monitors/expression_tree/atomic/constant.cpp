#include "constant.h"

std::string Constant::to_string() const { return this->getSymbol(); }

std::shared_ptr<Expression> Constant::clone() { return std::make_shared<Constant>(*this); }

std::shared_ptr<Expression> Constant::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitConstant(*this, trace, index);
}

std::shared_ptr<Hydra2Monitor> Constant::accept(HydraCreationVisitor2 &visitor, size_t parentId, bool connectedEval) {
    return visitor.visitConstant(*this, parentId, connectedEval);
}

std::shared_ptr<HydraMonitor> Constant::accept(HydraCreationVisitor &visitor) { return visitor.visitConstant(*this); }

bool Constant::getValue() const { return value; }
