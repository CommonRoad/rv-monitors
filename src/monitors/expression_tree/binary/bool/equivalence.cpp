#include "equivalence.h"

std::shared_ptr<Expression> Equivalence::clone() { return std::make_shared<Equivalence>(*this); }

std::shared_ptr<Expression> Equivalence::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitEquivalence(*this, trace, index);
}
std::shared_ptr<HydraMonitor> Equivalence::accept(HydraCreationVisitor &visitor, size_t *maxId) {
    return std::shared_ptr<HydraMonitor>();
}
