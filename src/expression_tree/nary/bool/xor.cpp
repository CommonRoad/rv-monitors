#include "xor.h"

std::shared_ptr<Expression> Xor::accept(expression_visitor &visitor, Trace &trace, int index) {
    return visitor.visitXor(*this, trace, index);
}

std::shared_ptr<Expression> Xor::clone() { return std::make_shared<Xor>(*this); }
