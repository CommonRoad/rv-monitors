//
// Created by Sebastian Maierhofer on 08.06.21.
//

#include "predicate.h"

std::string Predicate::to_string() const { return this->getSymbol(); }

std::shared_ptr<Expression> Predicate::clone() { return std::make_shared<Predicate>(*this); }

std::shared_ptr<Expression> Predicate::accept(ExpressionVisitor &visitor, Trace &trace, int index) {
    return visitor.visitPredicate(*this, trace, index);
}
const std::string &Predicate::getName() const { return name; }

const std::vector<std::string> &Predicate::getParameters() const { return parameters; }
