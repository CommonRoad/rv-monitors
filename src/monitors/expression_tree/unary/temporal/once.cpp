#include "monitors/expression_tree/unary/temporal/once.h"

std::shared_ptr<Expression> Once::clone() { return std::make_shared<Once>(*this); }

std::shared_ptr<Expression> Once::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitOnce(*this, trace, index);
}

std::shared_ptr<Hydra2Monitor> Once::accept(HydraCreationVisitor2 &visitor, size_t parentId, bool connectedEval) {
    return nullptr;
}

[[nodiscard]] std::shared_ptr<HydraMonitor> Once::accept(HydraCreationVisitor &visitor) { return nullptr; }
