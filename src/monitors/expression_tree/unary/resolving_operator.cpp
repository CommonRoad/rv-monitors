//
// Created by Sebastian Maierhofer on 14.01.21.
//

#include "resolving_operator.h"

std::shared_ptr<Expression> ResolvingOperator::clone() { return std::make_shared<ResolvingOperator>(*this); }

std::shared_ptr<Expression> ResolvingOperator::accept(ExpressionVisitor &visitor, Trace &tra, size_t idx) {
    return visitor.visitResolvingOperator(*this, tra, idx);
}

Event<bool> ResolvingOperator::getEvent() { return trace.getBooleanEvent(index); }

std::string ResolvingOperator::to_string() const {
    return "RE(" + getLeft()->to_string() + trace.getBooleanTrace().at(index).to_string() + ")";
}

std::shared_ptr<Hydra2Monitor> ResolvingOperator::accept(HydraCreationVisitor2 &visitor, size_t parentId,
                                                         bool connectedEval) {
    return nullptr;
}

std::shared_ptr<HydraMonitor> ResolvingOperator::accept(HydraCreationVisitor &visitor) { return nullptr; }