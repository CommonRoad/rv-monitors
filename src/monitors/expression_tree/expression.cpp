#include "expression.h"

std::string Expression::to_string() const { return ""; }

const std::string &Expression::getSymbol() const { return symbol; }

void Expression::setSymbol(const std::string &symb) { Expression::symbol = symb; }

bool operator==(const Expression &expr1, const Expression &expr2) {
    bool ret = expr1.to_string() == expr2.to_string();
    return ret;
}

bool operator<(const Expression &expr1, const Expression &expr2) { return expr1.to_string() < expr2.to_string(); }

bool Expression::isConstTrue() const { return to_string() == "true"; }

bool Expression::isConstFalse() const { return to_string() == "false"; }

bool Expression::isConst(bool constant) const {
    if (constant) {
        return isConstTrue();
    }
    return isConstFalse();
}