#include "monitors/expression_tree/binary/binary.h"

const std::shared_ptr<Expression> &Binary::getLeft() const { return left; }

void Binary::setLeft(const std::shared_ptr<Expression> &leftExpr) { Binary::left = leftExpr; }

const std::shared_ptr<Expression> &Binary::getRight() const { return right; }

void Binary::setRight(const std::shared_ptr<Expression> &rightExpr) { Binary::right = rightExpr; }

std::string Binary::to_string() const {
    return "(" + this->getLeft()->to_string() + " " + this->getSymbol() + " " + this->getRight()->to_string() + ")";
}

std::vector<Variable> Binary::extractVariables() const {
    std::vector<Variable> variablesTmp;
    std::vector<Variable> variablesTmpLeft{left->extractVariables()};
    std::vector<Variable> variablesTmpRight{right->extractVariables()};
    variablesTmp.insert(variablesTmp.begin(), variablesTmpLeft.begin(), variablesTmpLeft.end());
    variablesTmp.insert(variablesTmp.begin(), variablesTmpRight.begin(), variablesTmpRight.end());
    return variablesTmp;
}
