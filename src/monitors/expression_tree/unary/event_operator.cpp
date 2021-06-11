#include "event_operator.h"

std::shared_ptr<Expression> EventOperator::clone() { return std::make_shared<EventOperator>(*this); }

std::shared_ptr<Expression> EventOperator::accept(ExpressionVisitor &visitor, Trace &tr, int idx) {
    return visitor.visitEventOperator(*this, tr, idx);
}

Event EventOperator::getEvent() { return trace.getEvent(index); }

std::string EventOperator::to_string() const {
    return "E(" + getLeft()->to_string() + trace.getTrace().at(index).to_string() + ")";
}
