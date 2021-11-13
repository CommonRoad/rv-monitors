//
// Created by Evald Nexhipi.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "formula_generator.h"

std::shared_ptr<Expression> FormulaGenerator::generateUnaryExpression(std::string op,
                                                                      const std::shared_ptr<Expression> &exprLeft,
                                                                      bool hasInterval, size_t tstart, size_t tend) {

    if (op == notSymbol)
        return std::make_shared<Not>(exprLeft);

    if (op == futureSymbol) {
        if (hasInterval)
            return std::make_shared<Future>(exprLeft, std::make_shared<Interval>(tstart, tend));
        else
            return std::make_shared<Future>(exprLeft);
    }

    if (op == globallySymbol) {
        if (hasInterval)
            return std::make_shared<Globally>(exprLeft, std::make_shared<Interval>(tstart, tend));
        else
            return std::make_shared<Globally>(exprLeft);
    }

    if (op == nextStrongSymbol) {
        if (hasInterval)
            return std::make_shared<NextStrong>(exprLeft, std::make_shared<Interval>(tstart, tend));
        else
            return std::make_shared<NextStrong>(exprLeft);
    }

    if (op == nextWeakSymbol) {
        if (hasInterval)
            return std::make_shared<NextWeak>(exprLeft, std::make_shared<Interval>(tstart, tend));
        else
            return std::make_shared<NextWeak>(exprLeft);
    }

    if (op == historicallySymbol) {
        if (hasInterval)
            return std::make_shared<Historically>(exprLeft, std::make_shared<Interval>(tstart, tend));
        else
            return std::make_shared<Historically>(exprLeft);
    }

    if (op == onceSymbol) {
        if (hasInterval)
            return std::make_shared<Once>(exprLeft, std::make_shared<Interval>(tstart, tend));
        else
            return std::make_shared<Once>(exprLeft);
    }

    if (op == previousSymbol) {
        if (hasInterval)
            return std::make_shared<Previously>(exprLeft, std::make_shared<Interval>(tstart, tend));
        else
            return std::make_shared<Previously>(exprLeft);
    }
}

std::shared_ptr<Expression> FormulaGenerator::generateBinaryExpression(std::string op,
                                                                       const std::shared_ptr<Expression> &exprLeft,
                                                                       const std::shared_ptr<Expression> &exprRight) {
    if (op == andSymbol)
        return std::make_shared<And>(
            std::set<std::shared_ptr<Expression>, NaryInterface::SharedPtrExprCompare>{exprLeft, exprRight});
    if (op == orSymbol)
        return std::make_shared<Or>(
            std::set<std::shared_ptr<Expression>, NaryInterface::SharedPtrExprCompare>{exprLeft, exprRight});
    if (op == xorSymbol)
        return std::make_shared<Xor>(
            std::multiset<std::shared_ptr<Expression>, NaryInterface::SharedPtrExprCompare>{exprLeft, exprRight});
    if (op == implicationSymbol)
        return std::make_shared<Implication>(exprRight, exprLeft);
    if (op == equivalenceSymbol)
        return std::make_shared<Equivalence>(exprRight, exprLeft);
    if (op == sinceSymbol)
        return std::make_shared<Since>(exprRight, exprLeft);
}

std::shared_ptr<Expression> FormulaGenerator::generateAtomOrPredicate() {
    double coin = rreal(0, m_sumWeightsAtomic + m_sumWeightsPredicate);
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

    return m_predicates[rint(0, m_predicates.size() - 1)].m_predicate;
}
void FormulaGenerator::pickOperator(size_t &pos) {
    double coin = rreal(0, m_sumWeightsOperator);
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
        size_t tstart;
        size_t tend;
        auto oi = m_operators[pos];
        if (oi.m_hasInterval)
            generateInterval(oi, tstart, tend);

        /**
         * If unary operator
         */
        if (oi.m_name == notSymbol || oi.m_name == futureSymbol || oi.m_name == globallySymbol ||
            oi.m_name == nextStrongSymbol || oi.m_name == nextWeakSymbol || oi.m_name == historicallySymbol ||
            oi.m_name == onceSymbol || oi.m_name == previousSymbol) {
            auto exprLeft = generateExpression(nrOperators - 1);

            return generateUnaryExpression(oi.m_name, exprLeft, oi.m_hasInterval, tstart, tend);

        }
        /**
         * If binary operator
         */
        else if (oi.m_name == andSymbol || oi.m_name == orSymbol || oi.m_name == xorSymbol ||
                 oi.m_name == implicationSymbol || oi.m_name == equivalenceSymbol || oi.m_name == sinceSymbol) {
            auto nrLeft = rint(0, nrOperators - 1);
            auto exprLeft = generateExpression(nrLeft);
            auto exprRight = generateExpression(nrOperators - 1 - nrLeft);

            return generateBinaryExpression(oi.m_name, exprLeft, exprRight);
        }
    }

    return std::make_shared<Atomic>("z");
}
