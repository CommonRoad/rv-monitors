#include "until.h"

std::shared_ptr<Expression> Until::clone() { return std::make_shared<Until>(*this); }

std::shared_ptr<Expression> Until::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitUntil(*this, trace, index);
}

std::shared_ptr<Hydra2Monitor> Until::accept(HydraCreationVisitor2 &visitor, size_t parentId, bool connectedEval,
                                             OperatorType opType, std::optional<IntervalTemporalOperator> intv) {
    return visitor.visitUntil(*this, parentId, connectedEval, opType, intv);
}

std::shared_ptr<HydraMonitor> Until::accept(HydraCreationVisitor &visitor) { return visitor.visitUntil(*this); }
