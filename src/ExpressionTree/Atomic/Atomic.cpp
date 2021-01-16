#include "Atomic.h"

std::string Atomic::to_string() const {
    return this->getSymbol();
}

std::shared_ptr<Expression> Atomic::clone() {
    return std::make_shared<Atomic>(*this);
}

std::shared_ptr<Expression> Atomic::accept(ExpressionVisitor &visitor, Trace &trace, int index) {
    return visitor.visitAtomic(*this, trace, index);
}
