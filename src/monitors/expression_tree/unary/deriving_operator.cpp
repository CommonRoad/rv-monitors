#include "monitors/expression_tree/unary/deriving_operator.h"

std::shared_ptr<Expression> DerivingOperator::clone() { return std::make_shared<DerivingOperator>(*this); }

std::shared_ptr<Expression> DerivingOperator::accept(ExpressionVisitor &visitor, const std::shared_ptr<Trace> &tr,
                                                     size_t idx) {
    return visitor.visitDerivingOperator(*this, tr, idx);
}

Event DerivingOperator::getEvent() { return trace->getEvent(index); }

std::string DerivingOperator::to_string() const {
    return "D(" + getLeft()->to_string() + ")";
    // return "D(" + getLeft()->to_string()+ trace->getTrace().at(index).to_string() + ")";
}

std::shared_ptr<Hydra2Monitor> DerivingOperator::accept(HydraCreationVisitor2 &visitor, size_t parentId,
                                                        bool connectedEval) {
    return nullptr;
}

std::shared_ptr<HydraMonitor> DerivingOperator::accept(HydraCreationVisitor &visitor) { return nullptr; }
