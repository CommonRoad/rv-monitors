#include "release.h"

std::shared_ptr<Expression> Release::clone() { return std::make_shared<Release>(*this); }

std::shared_ptr<Expression> Release::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitRelease(*this, trace, index);
}
std::shared_ptr<HydraMonitor> Release::accept(HydraCreationVisitor &visitor, size_t parentId, bool connectedEval) {
    return std::shared_ptr<HydraMonitor>();
}
