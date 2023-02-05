#include "monitors/expression_tree/binary/temporal/release.h"

std::shared_ptr<Expression> Release::clone() { return std::make_shared<Release>(*this); }

std::shared_ptr<Expression> Release::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitRelease(*this, trace, index);
}

std::shared_ptr<Hydra2Monitor> Release::accept(HydraCreationVisitor2 &visitor, size_t parentId, bool connectedEval) {
    return nullptr;
}

std::shared_ptr<HydraMonitor> Release::accept(HydraCreationVisitor &visitor) { return nullptr; }
