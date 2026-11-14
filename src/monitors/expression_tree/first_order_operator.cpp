#include "monitors/expression_tree/first_order_operator.h"

#include <algorithm>
#include <utility>

Variable::Variable(std::string name, Domain domain) : name(std::move(name)), domain(std::move(domain)) {}

std::string Variable::getName() const { return name; }

Domain Variable::getDomain() const { return domain; }

FirstOrderOperator::FirstOrderOperator(std::vector<Variable> &variables) : variables(variables) {}

FirstOrderOperator::FirstOrderOperator(Variable &variable) { variables.push_back(variable); }

void FirstOrderOperator::addVariable(const std::string &variable, const Domain &domain) {
    variables.emplace_back(variable, domain);
}

void FirstOrderOperator::removeVariable(std::string variable) {
    auto isVal = [&variable](const Variable &var) -> bool { return var.getName() == variable; };
    variables.erase(std::remove_if(variables.begin(), variables.end(), isVal), variables.end());
}

std::vector<Variable> FirstOrderOperator::getVariables() const { return variables; }

std::string FirstOrderOperator::to_string() const {
    std::string str;

    const auto &vars = getVariables();
    for (size_t idx = 0; idx < vars.size(); idx++) {
        Variable variable = vars[idx];
        str += variable.getName() + " in " + variable.getDomain().to_string();
        if (idx != vars.size() - 1)
            str += ", ";
        else
            str += ". ";
    }
    return str;
}

std::set<std::string> FirstOrderOperator::getVariableNames() const {
    std::set<std::string> names;
    const auto &vars = getVariables();
    for (const auto &var : getVariables()) {
        names.insert(var.getName());
    }
    return names;
}
