#include "monitors/expression_tree/domain.h"
#include "monitors/expression_tree/atomic/predicate.h"

Domain::Domain(std::string symbol, std::string name, std::vector<std::string> parameters,
               std::vector<std::string> parameterVariables)
    : Predicate(std::move(symbol), std::move(name), std::move(parameters), std::move(parameterVariables)) {}

bool Domain::isFunctionDomain() const { return !parameters.empty(); }

std::string Domain::to_string() const {
    if (isFunctionDomain())
        return Predicate::to_string();
    else
        return name;
}
