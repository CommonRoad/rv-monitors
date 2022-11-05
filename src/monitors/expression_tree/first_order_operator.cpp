#include "first_order_operator.h"

#include <utility>
Variable::Variable(std::string &name, std::string &domain)
    : name(name), domain(domain)
// Constructor
{}

std::string Variable::getName() const
// Returns name of variable
{
    return name;
}

std::string Variable::getDomain() const
// Returns name of collections
{
    return domain;
}

FirstOrderOperator::FirstOrderOperator(std::vector<Variable> &variables)
    : variables(variables)
// First constructor
{}

FirstOrderOperator::FirstOrderOperator(Variable &variable)
// Second constructor
{
    variables.push_back(variable);
}

void FirstOrderOperator::addVariable(std::string variable, std::string domain)
// Adds variable
{
    variables.emplace_back(variable, domain);
}

void FirstOrderOperator::removeVariable(std::string variable)
// Removes variable
{
    auto isVal = [&variable](const Variable &var) -> bool { return var.getName() == variable; };
    variables.erase(std::remove_if(variables.begin(), variables.end(), isVal), variables.end());
}

const std::vector<Variable> &FirstOrderOperator::getVariables() const
// Returns all variables bounded by operator
{
    return variables;
}

std::string FirstOrderOperator::to_string() const {
    std::string str;

    const auto &vars = getVariables();
    for (size_t idx = 0; idx < vars.size(); idx++) {
        Variable variable = vars[idx];
        str += variable.getName() + " in " + variable.getDomain();
        if (idx != vars.size() - 1)
            str += ", ";
        else
            str += ". ";
    }

    return str;
}
