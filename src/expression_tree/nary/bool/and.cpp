#include "and.h"

std::shared_ptr<Expression> And::clone() { return std::make_shared<And>(*this); }

std::shared_ptr<Expression> And::accept(expression_visitor &visitor, Trace &trace, int index) {
    return visitor.visitAnd(*this, trace, index);
}
