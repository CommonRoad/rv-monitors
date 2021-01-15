#include "EventOperator.h"

std::shared_ptr<Expression> EventOperator::clone() {
    return std::make_shared<EventOperator>(*this);
}

std::shared_ptr<Expression> EventOperator::accept(ExpressionVisitor &visitor, Trace &tr) {
    return visitor.visitEventOperator(*this, tr);
}

Event EventOperator::getEvent() {
    return trace.getLastEvent();
}

std::string EventOperator::to_string() const {
    return "E(" + getLeft()->to_string() + trace.getLastEvent().to_string() + ")";
}
