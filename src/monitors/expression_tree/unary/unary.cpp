#include "monitors/expression_tree/unary/unary.h"

std::string Unary::to_string() const { return this->getSymbol() + "(" + this->getLeft()->to_string() + ")"; }

const std::shared_ptr<Expression> &Unary::getLeft() const { return left; }

void Unary::setLeft(const std::shared_ptr<Expression> &leftExpr) { Unary::left = leftExpr; }

std::vector<Variable> Unary::extractVariables() const {
    std::vector<Variable> variablesTmp;
    auto variablesLeft{left->extractVariables()};
    variablesTmp.insert(variablesTmp.begin(), variablesLeft.begin(), variablesLeft.end());
    return variablesTmp;
}
