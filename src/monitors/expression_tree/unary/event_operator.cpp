#include "event_operator.h"

std::shared_ptr<Expression> EventOperator::clone() { return std::make_shared<EventOperator>(*this); }

std::shared_ptr<Expression> EventOperator::accept(ExpressionVisitor &visitor, Trace &tr, size_t idx) {
    return visitor.visitEventOperator(*this, tr, idx);
}

Event EventOperator::getEvent() { return trace.getEvent(index); }

std::string EventOperator::to_string() const {
    return "E(" + getLeft()->to_string() + trace.getTrace().at(index).to_string() + ")";
}
std::shared_ptr<HydraMonitor> EventOperator::accept(HydraCreationVisitor &visitor, size_t *maxId, std::map<std::string, std::shared_ptr<HydraMonitor>> *baseline, size_t parentId) {
    return std::shared_ptr<HydraMonitor>();
}
