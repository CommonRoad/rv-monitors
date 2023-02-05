//
// Created by Sebastian Maierhofer on 26.01.21.
//

#include "monitors/expression_tree/unary/temporal/historically.h"

std::shared_ptr<Expression> Historically::clone() { return std::make_shared<Historically>(*this); }

std::shared_ptr<Expression> Historically::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitHistorically(*this, trace, index);
}

std::shared_ptr<Hydra2Monitor> Historically::accept(HydraCreationVisitor2 &visitor, size_t parentId,
                                                    bool connectedEval) {
    return nullptr;
}

std::shared_ptr<HydraMonitor> Historically::accept(HydraCreationVisitor &visitor) { return nullptr; };