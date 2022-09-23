#include "atomic.h"

std::string Atomic::to_string() const { return this->getSymbol(); }

std::shared_ptr<Expression> Atomic::clone() { return std::make_shared<Atomic>(*this); }

std::shared_ptr<Expression> Atomic::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitAtomic(*this, trace, index);
}

std::shared_ptr<Hydra2Monitor> Atomic::accept(HydraCreationVisitor2 &visitor, size_t parentId, bool connectedEval,
                                              OperatorType opType, std::optional<IntervalTemporalOperator> intv) {
    return visitor.visitAtomic(*this, parentId, connectedEval, opType, intv);
}

std::shared_ptr<HydraMonitor> Atomic::accept(HydraCreationVisitor &visitor) { return visitor.visitAtomic(*this); }
