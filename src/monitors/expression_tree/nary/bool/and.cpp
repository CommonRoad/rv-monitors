#include "and.h"

std::shared_ptr<Expression> And::clone() { return std::make_shared<And>(*this); }

std::shared_ptr<Expression> And::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitAnd(*this, trace, index);
}
std::shared_ptr<Hydra2Monitor> And::accept(HydraCreationVisitor2 &visitor, size_t parentId, bool connectedEval) {
    return visitor.visitAnd(*this, parentId, connectedEval);
}
