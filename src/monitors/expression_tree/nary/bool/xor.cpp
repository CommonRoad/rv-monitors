#include "xor.h"

std::shared_ptr<Expression> Xor::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitXor(*this, trace, index);
}

std::shared_ptr<Expression> Xor::clone() { return std::make_shared<Xor>(*this); }
std::shared_ptr<HydraMonitor> Xor::accept(HydraCreationVisitor &visitor, size_t parentId, bool connectedEval) {
    return std::shared_ptr<HydraMonitor>();
}
