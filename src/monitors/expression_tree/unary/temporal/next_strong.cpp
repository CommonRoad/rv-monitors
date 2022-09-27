#include "next_strong.h"

std::shared_ptr<Expression> NextStrong::clone() { return std::make_shared<NextStrong>(*this); }

std::shared_ptr<Expression> NextStrong::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitNextStrong(*this, trace, index);
}

std::shared_ptr<Hydra2Monitor> NextStrong::accept(HydraCreationVisitor2 &visitor, size_t parentId, bool connectedEval) {
    return visitor.visitNext(*this, parentId, connectedEval);
}
std::shared_ptr<HydraMonitor> NextStrong::accept(HydraCreationVisitor &visitor) { return visitor.visitNext(*this); }