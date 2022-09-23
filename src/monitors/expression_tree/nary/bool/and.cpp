#include "and.h"

std::shared_ptr<Expression> And::clone() { return std::make_shared<And>(*this); }

std::shared_ptr<Expression> And::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitAnd(*this, trace, index);
}

std::shared_ptr<Hydra2Monitor> And::accept(HydraCreationVisitor2 &visitor, size_t parentId, bool connectedEval,
                                           OperatorType opType, std::optional<IntervalTemporalOperator> intv) {
    return visitor.visitAnd(*this, parentId, connectedEval, opType, intv);
}

std::shared_ptr<HydraMonitor> And::accept(HydraCreationVisitor &visitor) { return visitor.visitAnd(*this); }
