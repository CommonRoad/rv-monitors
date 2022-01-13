//
// Created by Sebastian Maierhofer on 14.01.21.
//

#include "deriving_operator.h"

std::shared_ptr<Expression> DerivingOperator::clone() { return std::make_shared<DerivingOperator>(*this); }

std::shared_ptr<Expression> DerivingOperator::accept(ExpressionVisitor &visitor, Trace &tr, size_t idx) {
    return visitor.visitDerivingOperator(*this, tr, idx);
}

Event DerivingOperator::getEvent() { return trace.getEvent(index); }

std::string DerivingOperator::to_string() const {
     return "D(" + getLeft()->to_string()+")";
    // return "D(" + getLeft()->to_string()+ trace.getTrace().at(index).to_string() + ")";
}
std::shared_ptr<HydraMonitor> DerivingOperator::accept(HydraCreationVisitor &visitor, size_t parentId,
                                                       bool connectedEval) {
    return std::shared_ptr<HydraMonitor>();
}
