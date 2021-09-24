#include "xor.h"

std::shared_ptr<Expression> Xor::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitXor(*this, trace, index);
}

std::shared_ptr<Expression> Xor::clone() { return std::make_shared<Xor>(*this); }
