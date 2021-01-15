//
// Created by Sebastian Maierhofer on 14.01.21.
//

#include "ResolvingOperator.h"

std::shared_ptr<Expression> ResolvingOperator::clone() {
    return std::make_shared<ResolvingOperator>(*this);
}

std::shared_ptr<Expression> ResolvingOperator::accept(ExpressionVisitor &visitor, Trace &trace) {
    return visitor.visitResolvingOperator(*this, trace);
}

Event ResolvingOperator::getEvent() {
    return event;
}

std::string ResolvingOperator::to_string() const {
    return "RE(" + getLeft()->to_string() + event.to_string() + ")";
}