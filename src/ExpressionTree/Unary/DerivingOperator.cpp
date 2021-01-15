//
// Created by Sebastian Maierhofer on 14.01.21.
//

#include "DerivingOperator.h"

std::shared_ptr<Expression> DerivingOperator::clone() {
    return std::make_shared<DerivingOperator>(*this);
}

std::shared_ptr<Expression> DerivingOperator::accept(ExpressionVisitor &visitor, Trace &trace) {
    return visitor.visitDerivingOperator(*this, trace);
}

Event DerivingOperator::getEvent() {
    return event;
}

std::string DerivingOperator::to_string() const {
    return "D(" + getLeft()->to_string() + event.to_string() + ")";
}
