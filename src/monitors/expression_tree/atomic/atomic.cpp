#include "atomic.h"

std::string Atomic::to_string() const { return this->getSymbol(); }

std::shared_ptr<Expression> Atomic::clone() { return std::make_shared<Atomic>(*this); }

std::shared_ptr<Expression> Atomic::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitAtomic(*this, trace, index);
}

std::shared_ptr<HydraMonitor> Atomic::accept(HydraCreationVisitor &visitor) {
    return HydraCreationVisitor::visitAtomic(*this);
}
