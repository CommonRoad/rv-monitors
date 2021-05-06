#include "equivalence.h"

std::shared_ptr<Expression> Equivalence::clone() { return std::make_shared<Equivalence>(*this); }

std::shared_ptr<Expression> Equivalence::accept(expression_visitor &visitor, Trace &trace, int index) {
    return visitor.visitEquivalence(*this, trace, index);
}
