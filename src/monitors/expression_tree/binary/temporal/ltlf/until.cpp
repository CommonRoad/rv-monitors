#include "until.h"

std::shared_ptr<Expression> Until::clone() { return std::make_shared<Until>(*this); }

std::shared_ptr<Expression> Until::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitUntil(*this, trace, index);
}
std::shared_ptr<HydraMonitor> Until::accept(HydraCreationVisitor &visitor) { return std::shared_ptr<HydraMonitor>(); }
