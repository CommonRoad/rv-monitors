#include "monitors/utils/generators/formula_generator/formula_generator.h"
#include "monitors/expression_tree/atomic/atomic.h"
#include "monitors/expression_tree/atomic/predicate.h"

std::shared_ptr<Expression> FormulaGenerator::generateUnaryExpression(const std::string &op,
                                                                      const std::shared_ptr<Expression> &exprLeft,
                                                                      bool hasInterval, size_t tStart, size_t tEnd) {

    if (hasInterval)
        return generateUnaryExpressionWithInterval(op, exprLeft, tStart, tEnd);
    else
        return generateUnaryExpressionWithoutInterval(op, exprLeft);
}

std::shared_ptr<Expression> FormulaGenerator::generateBinaryExpression(const std::string &op,
                                                                       const std::shared_ptr<Expression> &exprLeft,
                                                                       const std::shared_ptr<Expression> &exprRight,
                                                                       bool hasInterval, size_t tStart, size_t tEnd) {
    if (hasInterval)
        return generateBinaryExpressionWithInterval(op, exprLeft, exprRight, tStart, tEnd);
    else
        return generateBinaryExpressionWithoutInterval(op, exprLeft, exprRight);
}

std::shared_ptr<Expression> FormulaGenerator::generateUnaryExpressionWithInterval(
    const std::string &op, const std::shared_ptr<Expression> &exprLeft, size_t tStart, size_t tEnd) {
    if (op == futureSymbol)
        return std::make_shared<Future>(exprLeft, std::make_shared<Interval>(tStart, tEnd));
    else if (op == globallySymbol)
        return std::make_shared<Globally>(exprLeft, std::make_shared<Interval>(tStart, tEnd));
    else if (op == nextStrongSymbol)
        return std::make_shared<NextStrong>(exprLeft, std::make_shared<Interval>(tStart, tEnd));
    else if (op == nextWeakSymbol)
        return std::make_shared<NextWeak>(exprLeft, std::make_shared<Interval>(tStart, tEnd));
    else if (op == historicallySymbol)
        return std::make_shared<Historically>(exprLeft, std::make_shared<Interval>(tStart, tEnd));
    else if (op == onceSymbol)
        return std::make_shared<Once>(exprLeft, std::make_shared<Interval>(tStart, tEnd));
    else if (op == previousSymbol)
        return std::make_shared<Previously>(exprLeft, std::make_shared<Interval>(tStart, tEnd));
    else
        throw std::logic_error("FormulaGenerator::generateUnaryExpressionWithInterval: Unknown operator!");
}

std::shared_ptr<Expression>
FormulaGenerator::generateUnaryExpressionWithoutInterval(const std::string &op,
                                                         const std::shared_ptr<Expression> &exprLeft) {
    if (op == notSymbol)
        return std::make_shared<Not>(exprLeft);
    else if (op == futureSymbol)
        return std::make_shared<Future>(exprLeft);
    else if (op == globallySymbol)
        return std::make_shared<Globally>(exprLeft);
    else if (op == nextStrongSymbol)
        return std::make_shared<NextStrong>(exprLeft);
    else if (op == nextWeakSymbol)
        return std::make_shared<NextWeak>(exprLeft);
    else if (op == historicallySymbol)
        return std::make_shared<Historically>(exprLeft);
    else if (op == onceSymbol)
        return std::make_shared<Once>(exprLeft);
    else if (op == previousSymbol)
        return std::make_shared<Previously>(exprLeft);
    else
        throw std::logic_error("FormulaGenerator::generateUnaryExpressionWithoutInterval: Unknown operator!");
}

std::shared_ptr<Expression> FormulaGenerator::generateBinaryExpressionWithInterval(
    const std::string &op, const std::shared_ptr<Expression> &exprLeft, const std::shared_ptr<Expression> &exprRight,
    size_t tStart, size_t tEnd) {
    if (op == sinceSymbol)
        return std::make_shared<Since>(exprRight, exprLeft, std::make_shared<Interval>(tStart, tEnd));
    else if (op == untilSymbol)
        return std::make_shared<Until>(exprRight, exprLeft, std::make_shared<Interval>(tStart, tEnd));
    else if (op == releaseSymbol)
        return std::make_shared<Release>(exprRight, exprLeft, std::make_shared<Interval>(tStart, tEnd));
    else
        throw std::logic_error("FormulaGenerator::generateBinaryExpressionWithInterval: Unknown operator!");
}

std::shared_ptr<Expression> FormulaGenerator::generateBinaryExpressionWithoutInterval(
    const std::string &op, const std::shared_ptr<Expression> &exprLeft, const std::shared_ptr<Expression> &exprRight) {
    if (op == andSymbol)
        return std::make_shared<And>(
            std::set<std::shared_ptr<Expression>, NaryInterface::SharedPtrExprCompare>{exprLeft, exprRight});
    else if (op == orSymbol)
        return std::make_shared<Or>(
            std::set<std::shared_ptr<Expression>, NaryInterface::SharedPtrExprCompare>{exprLeft, exprRight});
    else if (op == xorSymbol)
        return std::make_shared<Xor>(
            std::multiset<std::shared_ptr<Expression>, NaryInterface::SharedPtrExprCompare>{exprLeft, exprRight});
    else if (op == implicationSymbol)
        return std::make_shared<Implication>(exprRight, exprLeft);
    else if (op == equivalenceSymbol)
        return std::make_shared<Equivalence>(exprRight, exprLeft);
    else if (op == sinceSymbol)
        return std::make_shared<Since>(exprRight, exprLeft);
    else if (op == untilSymbol)
        return std::make_shared<Until>(exprRight, exprLeft);
    else if (op == releaseSymbol)
        return std::make_shared<Release>(exprRight, exprLeft);
    else
        throw std::logic_error("FormulaGenerator::generateBinaryExpressionWithoutInterval: Unknown operator!");
}

std::shared_ptr<Expression> FormulaGenerator::generateAtomOrPredicate() {
    double coin = rReal(0, m_sumWeightsAtomic + m_sumWeightsPredicate);
    double w = 0.0;
    for (auto &iter : m_atoms) {
        w += iter.m_weight;
        if (w >= coin)
            return iter.m_atomic;
    }
    for (auto &iter : m_predicates) {
        w += iter.m_weight;
        if (w >= coin)
            return iter.m_predicate;
    }

    return m_predicates[rInt(0, m_predicates.size() - 1)].m_predicate;
}

void FormulaGenerator::pickOperator(size_t &pos) {
    double coin = rReal(0, m_sumWeightsOperator);
    pos = m_operators.size() - 1;
    double w = 0.0;
    for (size_t i = 0; i < m_operators.size(); ++i) {
        w += m_operators[i].m_weight;
        if (w >= coin) {
            pos = i;
            break;
        }
    }
}

std::shared_ptr<Expression> FormulaGenerator::generateExpression(const size_t nrOperators) {
    /**
     * (BASE CASE) Formula consists of only a proposition or a predicate
     */
    if (nrOperators == 0) {
        return generateAtomOrPredicate();
    }
    /**
     * Operator to be added
     */
    else {
        size_t pos;
        pickOperator(pos);
        size_t tStart;
        size_t tEnd;
        auto oi = m_operators[pos];
        if (oi.m_hasInterval)
            generateInterval(oi, tStart, tEnd);

        /**
         * If unary operator
         */
        if (oi.m_name == notSymbol || oi.m_name == futureSymbol || oi.m_name == globallySymbol ||
            oi.m_name == nextStrongSymbol || oi.m_name == nextWeakSymbol || oi.m_name == historicallySymbol ||
            oi.m_name == onceSymbol || oi.m_name == previousSymbol) {
            auto exprLeft = generateExpression(nrOperators - 1);

            return generateUnaryExpression(oi.m_name, exprLeft, oi.m_hasInterval, tStart, tEnd);

        }
        /**
         * If binary operator
         */
        else if (oi.m_name == andSymbol || oi.m_name == orSymbol || oi.m_name == xorSymbol ||
                 oi.m_name == implicationSymbol || oi.m_name == equivalenceSymbol || oi.m_name == sinceSymbol ||
                 oi.m_name == untilSymbol || oi.m_name == releaseSymbol) {
            auto nrLeft = rInt(0, nrOperators - 1);
            auto exprLeft = generateExpression(nrLeft);
            auto exprRight = generateExpression(nrOperators - 1 - nrLeft);

            return generateBinaryExpression(oi.m_name, exprLeft, exprRight, oi.m_hasInterval, tStart, tEnd);
        }
    }

    return std::make_shared<Atomic>("z");
}
