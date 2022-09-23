#include "previously.h"

std::shared_ptr<Expression> Previously::clone() { return std::make_shared<Previously>(*this); }

std::shared_ptr<Expression> Previously::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitPreviously(*this, trace, index);
}

std::shared_ptr<Hydra2Monitor> Previously::accept(HydraCreationVisitor2 &visitor, size_t parentId, bool connectedEval,
                                                  OperatorType opType, std::optional<IntervalTemporalOperator> intv) {
    return visitor.visitPreviously(*this, parentId, connectedEval, opType, intv);
}

std::shared_ptr<HydraMonitor> Previously::accept(HydraCreationVisitor &visitor) {
    return visitor.visitPreviously(*this);
}
