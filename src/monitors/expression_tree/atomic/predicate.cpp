//
// Created by Sebastian Maierhofer on 08.06.21.
//

#include "predicate.h"

std::string Predicate::to_string() const {
    std::string str{name + "("};
    for (const auto &param : parameters)
        str += param + ", ";
    str.erase(str.size() - 2);
    str += ")";
    return str;
}

std::shared_ptr<Expression> Predicate::clone() { return std::make_shared<Predicate>(*this); }

std::shared_ptr<Expression> Predicate::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitPredicate(*this, trace, index);
}

std::shared_ptr<Hydra2Monitor> Predicate::accept(HydraCreationVisitor2 &visitor, size_t parentId, bool connectedEval,
                                                 OperatorType opType, std::optional<IntervalTemporalOperator> intv) {
    return visitor.visitPredicate(*this, parentId, connectedEval, opType, intv);
}

std::shared_ptr<HydraMonitor> Predicate::accept(HydraCreationVisitor &visitor) { return visitor.visitPredicate(*this); }

const std::string &Predicate::getName() const { return name; }

const std::vector<std::string> &Predicate::getParameters() const { return parameters; }
