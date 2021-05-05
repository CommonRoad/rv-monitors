//
// Created by Sebastian Maierhofer on 14.01.21.
//

#include "since.h"

std::shared_ptr<Expression> Since::clone() { return std::make_shared<Since>(*this); }

std::shared_ptr<Expression> Since::accept(expression_visitor &visitor, Trace &trace, int index) {
    return visitor.visitSince(*this, trace, index);
}