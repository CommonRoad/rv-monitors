#include "or.h"

std::shared_ptr<Expression> Or::clone() { return std::make_shared<Or>(*this); }

std::shared_ptr<Expression> Or::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitOr(*this, trace, index);
}

std::shared_ptr<Hydra2Monitor> Or::accept(HydraCreationVisitor2 &visitor, size_t parentId, bool connectedEval,
                                          OperatorType opType, std::optional<IntervalTemporalOperator> intv) {
    return visitor.visitOr(*this, parentId, connectedEval, opType, intv);
}

std::shared_ptr<HydraMonitor> Or::accept(HydraCreationVisitor &visitor) { return visitor.visitOr(*this); }