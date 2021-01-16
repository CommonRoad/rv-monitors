#include "Implication.h"

std::shared_ptr<Expression> Implication::clone() {
    return std::make_shared<Implication>(*this);
}

std::shared_ptr<Expression> Implication::accept(ExpressionVisitor &visitor, Trace &trace, int index) {
    return visitor.visitImplication(*this, trace, index);
}
