//
// Created by Sebastian Maierhofer on 14.01.21.
//

#include "resolving_operator.h"

std::shared_ptr<Expression> ResolvingOperator::clone() { return std::make_shared<ResolvingOperator>(*this); }

std::shared_ptr<Expression> ResolvingOperator::accept(ExpressionVisitor &visitor, Trace &tr, size_t idx) {
    return visitor.visitResolvingOperator(*this, tr, idx);
}

Event ResolvingOperator::getEvent() { return trace.getEvent(index); }

std::string ResolvingOperator::to_string() const {
    return "RE(" + getLeft()->to_string() + trace.getTrace().at(index).to_string() + ")";
}
std::shared_ptr<HydraMonitor> ResolvingOperator::accept(HydraCreationVisitor &visitor, size_t parentId,
                                                        bool connectedEval) {
    return std::shared_ptr<HydraMonitor>();
}
