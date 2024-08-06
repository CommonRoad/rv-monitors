#include "monitors/expression_tree/atomic/predicate.h"
#include <algorithm>
#include <utility>

std::string Predicate::to_string() const {
    std::string str{name + "("};
    for (const auto &param : parameters)
        if (std::any_of(parameterVariables.begin(), parameterVariables.end(),
                        [param](const std::string &var) { return param == var; }))
            str += param + ", ";
        else
            str += "'" + param + "', ";
    str.erase(str.size() - 2);
    str += ")";
    return str;
}

std::shared_ptr<Expression> Predicate::clone() { return std::make_shared<Predicate>(*this); }

std::shared_ptr<Expression> Predicate::accept(ExpressionVisitor &visitor, const std::shared_ptr<Trace> &trace,
                                              size_t index) {
    return visitor.visitPredicate(*this, trace, index);
}

std::shared_ptr<Hydra2Monitor> Predicate::accept(HydraCreationVisitor2 &visitor, size_t parentId, bool connectedEval) {
    return visitor.visitPredicate(*this, parentId, connectedEval);
}

std::shared_ptr<HydraMonitor> Predicate::accept(HydraCreationVisitor &visitor) {
    return HydraCreationVisitor::visitPredicate(*this);
}

const std::string &Predicate::getName() const { return name; }

const std::vector<std::string> &Predicate::getVarParameters() const { return parameterVariables; }

const std::vector<std::string> &Predicate::getConstParameters() const { return parameterConstants; }

const std::vector<std::string> &Predicate::getParameters() const { return parameters; }

Predicate::Predicate(std::string symbol, std::string name, std::vector<std::string> parameters,
                     std::vector<std::string> parameterVariables)
    : Expression(std::move(symbol)), name(std::move(name)), parameters(std::move(parameters)),
      parameterVariables(std::move(parameterVariables)) {
    for (const auto &param : this->parameters)
        if (!std::any_of(this->parameterVariables.begin(), this->parameterVariables.end(),
                         [param](const std::string &var) { return param == var; }))
            parameterConstants.push_back(param);
}
