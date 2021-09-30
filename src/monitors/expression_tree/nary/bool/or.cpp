#include "or.h"

std::shared_ptr<Expression> Or::clone() { return std::make_shared<Or>(*this); }

std::shared_ptr<Expression> Or::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitOr(*this, trace, index);
}
std::shared_ptr<HydraMonitor> Or::accept(HydraCreationVisitor &visitor, size_t *maxId) {
    return visitor.visitOr(*this, maxId);
}
