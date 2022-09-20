//
// Created by Sebastian Maierhofer on 14.01.21.
//

#include "since.h"

std::shared_ptr<Expression> Since::clone() { return std::make_shared<Since>(*this); }

std::shared_ptr<Expression> Since::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitSince(*this, trace, index);
}

std::shared_ptr<Hydra2Monitor> Since::accept(HydraCreationVisitor2 &visitor, size_t parentId, bool connectedEval) {
    return visitor.visitSince(*this, parentId, connectedEval);
}

std::shared_ptr<HydraMonitor> Since::accept(HydraCreationVisitor &visitor) { return visitor.visitSince(*this); }
