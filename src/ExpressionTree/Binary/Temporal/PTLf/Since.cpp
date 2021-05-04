//
// Created by Sebastian Maierhofer on 14.01.21.
//

#include "Since.h"

std::shared_ptr<Expression> Since::clone() { return std::make_shared<Since>(*this); }

std::shared_ptr<Expression> Since::accept(ExpressionVisitor &visitor, Trace &trace, int index) {
    return visitor.visitSince(*this, trace, index);
}