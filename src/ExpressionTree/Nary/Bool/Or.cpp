#include "Or.h"

std::shared_ptr<Expression> Or::clone() {
    return std::make_shared<Or>(*this);
}

std::shared_ptr<Expression> Or::accept(ExpressionVisitor &visitor, Trace &trace) {
    return visitor.visitOr(*this, trace);
}
