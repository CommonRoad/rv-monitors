#include "first_order_operator.h"

#include <utility>
Variable::Variable(std::string &name, std::string &domain) : name(name), domain(domain)
// Constructor
{}

const std::string &Variable::getName() const
// Returns name of variable
{ return name; }

const std::string &Variable::getDomain() const
// Returns name of collections
{ return domain; }

FirstOrderOperator::FirstOrderOperator(std::vector<std::shared_ptr<Variable>> &variables) : variables(variables)
// First constructor
{}

FirstOrderOperator::FirstOrderOperator(std::shared_ptr<Variable> &variable)
// Second constructor
{
    variables.push_back(variable);
}

void FirstOrderOperator::addVariable(std::string &variable)
// Adds variable
{}

const std::vector<std::shared_ptr<Variable>> &FirstOrderOperator::getVariables() const
// Returns all variables bounded by operator
{ return variables; }

std::string FirstOrderOperator::to_string() const {
    std::string str;

    const auto &vars = getVariables();
    for (size_t idx = 0; idx < vars.size(); idx++) {
        std::shared_ptr<Variable> variable = vars[idx];
        str += variable->getName() + " in " + variable->getDomain();
        if (idx != vars.size() - 1)
            str += ", ";
        else
            str += ". ";
    }

    return str;
}
