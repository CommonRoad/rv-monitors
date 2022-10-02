#include "formula.h"
#include "monitors/expression_tree/binary/binary.h"
#include "monitors/expression_tree/nary/bool/xor.h"
#include "monitors/utils/utils.h"
#include "nff.h"
#include "monitors/expression_tree/atomic/atomic.h"

void Formula::toNnf() {
    auto before = deepCopy(expr);
    expr = NNF::evaluate(expr);
    while (!sameTree(before, expr)) {
        before = deepCopy(expr);
        expr = NNF::evaluate(before);
    }
}

[[nodiscard]] std::shared_ptr<Expression> Formula::getExpr() const { return expr; }

void Formula::setExpr(const std::shared_ptr<Expression> &expression) { expr = expression; }

[[nodiscard]] bool Formula::sameTree(const std::shared_ptr<Expression> &before,
                                     const std::shared_ptr<Expression> &after) {
    return before->to_string() == after->to_string();
}

[[nodiscard]] std::string Formula::to_string() const { return getExpr()->to_string(); }

[[nodiscard]] std::ostream &operator<<(std::ostream &ost, const Formula &formula) {
    ost << formula.to_string();
    return ost;
}

bool Formula::containsAtomic(const std::shared_ptr<Atomic> &atomic) const {
    auto atomics = Utils::getAtomics(this->getExpr());
    return atomics.find(atomic->to_string()) != atomics.end();
}

Formula Formula::deepCopy() const { return Formula(Formula::deepCopy(getExpr()), id); }

const std::string &Formula::getId() const { return id; }

bool Formula::checkCompatibility(std::vector<MonitoringProperties> formulaTypes) const {
    return std::any_of(formulaTypes.begin(), formulaTypes.end(),
                       [this](MonitoringProperties formulaType) { return this->checkCompatibility(formulaType); });
}

bool Formula::checkCompatibility(MonitoringProperties formulaType) const {
    auto formulaString = getExpr()->to_string();
    if (!formulaType.interval and formulaString.find('[') != std::string::npos)
        return false;
    switch (formulaType.formulaType) {
    case futureOnly:
        return !std::any_of(pastOperators.begin(), pastOperators.end(),
                            [formulaString](char oper) { return formulaString.find(oper) != std::string::npos; });
    case pastOnly:
        return !std::any_of(futureOperators.begin(), futureOperators.end(),
                            [formulaString](char oper) { return formulaString.find(oper) != std::string::npos; });
    case safetyGeneral:
        return this->expr->getSymbol().find(globallySymbol) != std::string::npos;
    case futureSafety:
        return this->expr->getSymbol() == globallySymbol and
               !std::any_of(pastOperators.begin(), pastOperators.end(),
                            [formulaString](char oper) { return formulaString.find(oper) != std::string::npos; });
    case pastSafety:
        return this->expr->getSymbol() == globallySymbol and
               std::count(formulaString.begin(), formulaString.end(), globallySymbol[0]) == 1 and
               !std::any_of(futureOperators.begin(), futureOperators.end(), [formulaString](char oper) {
                   return (formulaString.find(oper) != std::string::npos) and oper != globallySymbol[0];
               });
    case futurePastCombined:
        return true;
    }
    return false;
}

void Formula::setId(const std::string &identifier) { id = identifier; }

bool operator==(const Formula &formula1, const Formula &formula2) {
    return *(formula1.getExpr()) == *(formula2.getExpr());
}

bool operator<(const Formula &formula1, const Formula &formula2) {
    return *(formula1.getExpr()) < *(formula2.getExpr());
}
