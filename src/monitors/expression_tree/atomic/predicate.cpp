//
// Created by Sebastian Maierhofer on 08.06.21.
//

#include "predicate.h"

std::string Predicate::to_string() const { return this->getSymbol(); }

std::shared_ptr<Expression> Predicate::clone() { return std::make_shared<Predicate>(*this); }

std::shared_ptr<Expression> Predicate::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitPredicate(*this, trace, index);
}

std::shared_ptr<HydraMonitor> Predicate::accept(HydraCreationVisitor &visitor, size_t *maxId, std::map<std::string, std::shared_ptr<HydraMonitor>> *baseline, size_t parentId) {
    return HydraCreationVisitor::visitPredicate(*this, maxId, baseline, parentId);
}

const std::string &Predicate::getName() const { return name; }

const std::vector<std::string> &Predicate::getParameters() const { return parameters; }
