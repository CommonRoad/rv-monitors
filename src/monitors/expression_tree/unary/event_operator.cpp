#include "event_operator.h"

std::shared_ptr<Expression> EventOperator::clone() { return std::make_shared<EventOperator>(*this); }

std::shared_ptr<Expression> EventOperator::accept(ExpressionVisitor &visitor, Trace &tra, size_t idx) {
    return visitor.visitEventOperator(*this, tra, idx);
}

Event<bool> EventOperator::getEvent() { return trace.getBooleanEvent(index); }

std::string EventOperator::to_string() const {
    return "E(" + getLeft()->to_string() + trace.getBooleanTrace().at(index).to_string() + ")";
}
std::shared_ptr<Hydra2Monitor> EventOperator::accept(HydraCreationVisitor2 &visitor, size_t parentId,
                                                     bool connectedEval) {
    return nullptr;
}

std::shared_ptr<HydraMonitor> EventOperator::accept(HydraCreationVisitor &visitor) { return nullptr; }
