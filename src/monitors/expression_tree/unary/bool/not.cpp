#include "not.h"

std::shared_ptr<Expression> Not::clone() { return std::make_shared<Not>(*this); }

std::shared_ptr<Expression> Not::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitNot(*this, trace, index);
}
std::shared_ptr<Hydra2Monitor> Not::accept(HydraCreationVisitor2 &visitor, size_t parentId, bool connectedEval,
                                           OperatorType opType, std::optional<IntervalTemporalOperator> intv) {
    return visitor.visitNot(*this, parentId, connectedEval, opType, intv);
}

std::shared_ptr<HydraMonitor> Not::accept(HydraCreationVisitor &visitor) { return visitor.visitNot(*this); }
