#include "monitors/expression_tree/binary/bool/equivalence.h"

std::shared_ptr<Expression> Equivalence::clone() { return std::make_shared<Equivalence>(*this); }

std::shared_ptr<Expression> Equivalence::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitEquivalence(*this, trace, index);
}

std::shared_ptr<Hydra2Monitor> Equivalence::accept(HydraCreationVisitor2 &visitor, size_t parentId,
                                                   bool connectedEval) {
    return nullptr;
}

std::shared_ptr<HydraMonitor> Equivalence::accept(HydraCreationVisitor &visitor) { return nullptr; }
