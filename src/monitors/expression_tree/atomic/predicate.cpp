//
// Created by Sebastian Maierhofer on 08.06.21.
//

#include "predicate.h"

std::string Predicate::to_string() const {
    std::string s{name + "("};
    for(const auto &param : parameters)
        s += param + ", ";
    s.erase(s.size() - 2);
    s += ")";
    return s;
}

std::shared_ptr<Expression> Predicate::clone() { return std::make_shared<Predicate>(*this); }

std::shared_ptr<Expression> Predicate::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitPredicate(*this, trace, index);
}

std::shared_ptr<HydraMonitor> Predicate::accept(HydraCreationVisitor &visitor, size_t parentId, bool connectedEval) {
    return visitor.visitPredicate(*this, parentId, connectedEval);
}

const std::string &Predicate::getName() const { return name; }

const std::vector<std::string> &Predicate::getParameters() const { return parameters; }
