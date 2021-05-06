#include "or.h"

std::shared_ptr<Expression> Or::clone() { return std::make_shared<Or>(*this); }

std::shared_ptr<Expression> Or::accept(expression_visitor &visitor, Trace &trace, int index) {
    return visitor.visitOr(*this, trace, index);
}
