#include "not.h"
#include <utility>

std::shared_ptr<Expression> Not::clone() { return std::make_shared<Not>(*this); }

std::shared_ptr<Expression> Not::accept(expression_visitor &visitor, Trace &trace, int index) {
    return visitor.visitNot(*this, trace, index);
}
