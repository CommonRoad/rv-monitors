#include "implication.h"

std::shared_ptr<Expression> Implication::clone() { return std::make_shared<Implication>(*this); }

std::shared_ptr<Expression> Implication::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitImplication(*this, trace, index);
}
std::shared_ptr<Hydra2Monitor> Implication::accept(HydraCreationVisitor2 &visitor, size_t parentId,
                                                   bool connectedEval) {
    return nullptr;
}

std::shared_ptr<HydraMonitor> Implication::accept(HydraCreationVisitor &visitor) { return nullptr; }
