#include "monitors/expression_tree/binary/temporal/until.h"

std::shared_ptr<Expression> Until::clone() { return std::make_shared<Until>(*this); }

std::shared_ptr<Expression> Until::accept(ExpressionVisitor &visitor, const std::shared_ptr<Trace> &trace,
                                          size_t index) {
    return visitor.visitUntil(*this, trace, index);
}

std::shared_ptr<Hydra2Monitor> Until::accept(HydraCreationVisitor2 &visitor, size_t parentId, bool connectedEval) {
    return visitor.visitUntil(*this, parentId, connectedEval);
}

std::shared_ptr<HydraMonitor> Until::accept(HydraCreationVisitor &visitor) { return visitor.visitUntil(*this); }
