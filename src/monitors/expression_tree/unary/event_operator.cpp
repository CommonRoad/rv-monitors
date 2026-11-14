#include "monitors/expression_tree/unary/event_operator.h"

std::shared_ptr<Expression> EventOperator::clone() { return std::make_shared<EventOperator>(*this); }

std::shared_ptr<Expression> EventOperator::accept(ExpressionVisitor &visitor, const std::shared_ptr<Trace> &tra,
                                                  size_t idx) {
    return visitor.visitEventOperator(*this, tra, idx);
}

Event EventOperator::getEvent() { return trace->getEvent(index); }

std::string EventOperator::to_string() const {
    return "E(" + getLeft()->to_string() + trace->getTrace().at(index).to_string() + ")";
}
std::shared_ptr<Hydra2Monitor> EventOperator::accept(HydraCreationVisitor2 &visitor, size_t parentId,
                                                     bool connectedEval) {
    return nullptr;
}

std::shared_ptr<HydraMonitor> EventOperator::accept(HydraCreationVisitor &visitor) { return nullptr; }
