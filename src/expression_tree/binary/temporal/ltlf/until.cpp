#include "until.h"

std::shared_ptr<Expression> Until::clone() { return std::make_shared<Until>(*this); }

std::shared_ptr<Expression> Until::accept(expression_visitor &visitor, Trace &trace, int index) {
    return visitor.visitUntil(*this, trace, index);
}
