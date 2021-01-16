#include "Not.h"
#include <utility>

std::shared_ptr<Expression> Not::clone() {
    return std::make_shared<Not>(*this);
}

std::shared_ptr<Expression> Not::accept(ExpressionVisitor &visitor, Trace &trace, int index) {
    return visitor.visitNot(*this, trace, index);
}
