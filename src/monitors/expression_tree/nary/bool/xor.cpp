#include "monitors/expression_tree/nary/bool/xor.h"

std::shared_ptr<Expression> Xor::accept(ExpressionVisitor &visitor, const std::shared_ptr<Trace> &trace, size_t index) {
    return visitor.visitXor(*this, trace, index);
}

std::shared_ptr<Expression> Xor::clone() { return std::make_shared<Xor>(*this); }

std::shared_ptr<Hydra2Monitor> Xor::accept(HydraCreationVisitor2 &visitor, size_t parentId, bool connectedEval) {
    return nullptr;
}

std::shared_ptr<HydraMonitor> Xor::accept(HydraCreationVisitor &visitor) { return nullptr; }
