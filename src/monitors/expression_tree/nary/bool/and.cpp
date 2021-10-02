#include "and.h"

std::shared_ptr<Expression> And::clone() { return std::make_shared<And>(*this); }

std::shared_ptr<Expression> And::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitAnd(*this, trace, index);
}
std::shared_ptr<HydraMonitor> And::accept(HydraCreationVisitor &visitor, size_t parentId, bool connectedEval) {
    return visitor.visitAnd(*this, parentId, connectedEval);
}
