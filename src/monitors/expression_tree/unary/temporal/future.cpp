#include "future.h"

std::shared_ptr<Expression> Future::clone() { return std::make_shared<Future>(*this); }

std::shared_ptr<Expression> Future::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitFuture(*this, trace, index);
}

std::shared_ptr<Hydra2Monitor> Future::accept(HydraCreationVisitor2 &visitor, size_t parentId, bool connectedEval,
                                              OperatorType opType, std::optional<IntervalTemporalOperator> intv) {
    return nullptr;
}

std::shared_ptr<HydraMonitor> Future::accept(HydraCreationVisitor &visitor) { return nullptr; };