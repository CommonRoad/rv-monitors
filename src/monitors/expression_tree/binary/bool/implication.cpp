#include "implication.h"

std::shared_ptr<Expression> Implication::clone() { return std::make_shared<Implication>(*this); }

std::shared_ptr<Expression> Implication::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitImplication(*this, trace, index);
}
std::shared_ptr<HydraMonitor> Implication::accept(HydraCreationVisitor &visitor) {
    return std::shared_ptr<HydraMonitor>();
}
