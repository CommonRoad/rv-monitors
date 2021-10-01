#include "not.h"

std::shared_ptr<Expression> Not::clone() { return std::make_shared<Not>(*this); }

std::shared_ptr<Expression> Not::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitNot(*this, trace, index);
}
std::shared_ptr<HydraMonitor> Not::accept(HydraCreationVisitor &visitor, size_t parentId) {
    return visitor.visitNot(*this, parentId);
}
