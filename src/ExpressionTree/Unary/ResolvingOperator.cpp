//
// Created by Sebastian Maierhofer on 14.01.21.
//

#include "ResolvingOperator.h"

std::shared_ptr<Expression> ResolvingOperator::clone() {
    return std::make_shared<ResolvingOperator>(*this);
}

std::shared_ptr<Expression> ResolvingOperator::accept(ExpressionVisitor &visitor, Trace &tr) {
    return visitor.visitResolvingOperator(*this, tr);
}

Event ResolvingOperator::getEvent() {
    return trace.getLastEvent();
}

std::string ResolvingOperator::to_string() const {
    return "RE(" + getLeft()->to_string() + trace.getLastEvent().to_string() + ")";
}