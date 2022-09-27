#include "next_weak.h"

std::shared_ptr<Expression> NextWeak::clone() { return std::make_shared<NextWeak>(*this); }

std::shared_ptr<Expression> NextWeak::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitNextWeak(*this, trace, index);
}

std::shared_ptr<Hydra2Monitor> NextWeak::accept(HydraCreationVisitor2 &visitor, size_t parentId, bool connectedEval) {
    return nullptr;
}

std::shared_ptr<HydraMonitor> NextWeak::accept(HydraCreationVisitor &visitor) { return nullptr; }
