#include "and.h"

std::shared_ptr<Expression> And::clone() { return std::make_shared<And>(*this); }

std::shared_ptr<Expression> And::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitAnd(*this, trace, index);
}
