#include "monitors/expression_tree/unary/temporal/globally.h"

std::shared_ptr<Expression> Globally::clone() { return std::make_shared<Globally>(*this); }

std::shared_ptr<Expression> Globally::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitGlobally(*this, trace, index);
}

std::shared_ptr<Hydra2Monitor> Globally::accept(HydraCreationVisitor2 &visitor, size_t parentId, bool connectedEval) {
    return nullptr;
}

std::shared_ptr<HydraMonitor> Globally::accept(HydraCreationVisitor &visitor) { return nullptr; };
