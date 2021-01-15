#include "Not.h"
#include <utility>

std::shared_ptr<Expression> Not::clone() {
    return std::make_shared<Not>(*this);
}

std::shared_ptr<Expression> Not::accept(ExpressionVisitor &visitor, Trace &trace) {
    return visitor.visitNot(*this, trace);
}
