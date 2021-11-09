//
// Created by Evald Nexhipi on 20.10.21.
//
#include "formula_generator.h"

/**
 * Recursively generate an expression
 * @param nrOperators the number of operators needed for the expression
 * @return generated expression
 */
std::shared_ptr<Expression> FormulaGenerator::GenerateExpression(const int nrOperators)
{
    double coin;

    /**
     * Formula consists only of a proposition or a predicate
     */
    if(nrOperators == 0)
    {
        coin = rreal(0, m_sumWeightsAtomic + m_sumWeightsPredicate);
        double w = 0.0;
        for(auto & iter : m_atoms)
        {
            w += iter.m_weight;
            if(w >= coin)
                return iter.m_atomic;
        }
        for(auto & iter : m_predicates)
        {
            w += iter.m_weight;
            if(w >= coin)
                return iter.m_predicate;
        }

        return m_predicates[rint(0, m_predicates.size() - 1)].m_predicate;
    }
    /**
     * Operator to be added
     */
    else
    {
        coin = rreal(0, m_sumWeightsOperator);
        int pos = m_operators.size() - 1;
        double w = 0.0;
        for(int i = 0; i < m_operators.size(); ++i)
        {
            w += m_operators[i].m_weight;
            if(w >= coin)
            {
                pos = i;
                break;
            }
        }

        int tstart;
        int tend;
        auto oi = m_operators[pos];
        if(oi.m_hasInterval)
            GenerateInterval(oi, tstart, tend);

        /**
         * If unary operator
         */
        if(oi.m_name == OP_NOT ||
            oi.m_name == OP_FUTURE ||
            oi.m_name == OP_GLOBALLY ||
            oi.m_name == OP_NEXT_STRONG ||
            oi.m_name == OP_NEXT_WEAK ||
            oi.m_name == OP_HISTORICALLY ||
            oi.m_name == OP_ONCE ||
            oi.m_name == OP_PREVIOUSLY
        )
        {
            auto exprLeft = GenerateExpression(nrOperators - 1);

            if(oi.m_name == OP_NOT)
                return std::make_shared<Not>(exprLeft);

            if(oi.m_name == OP_FUTURE)
            {
                if(oi.m_hasInterval)
                    return std::make_shared<Future>(exprLeft, std::make_shared<Interval>(tstart, tend));
                else
                    return std::make_shared<Future>(exprLeft);
            }

            if(oi.m_name == OP_GLOBALLY)
            {
                if(oi.m_hasInterval)
                    return std::make_shared<Globally>(exprLeft, std::make_shared<Interval>(tstart, tend));
                else
                    return std::make_shared<Globally>(exprLeft);
            }

            if(oi.m_name == OP_NEXT_STRONG)
            {
                if(oi.m_hasInterval)
                    return std::make_shared<NextStrong>(exprLeft, std::make_shared<Interval>(tstart, tend));
                else
                    return std::make_shared<NextStrong>(exprLeft);
            }

            if(oi.m_name == OP_NEXT_WEAK)
            {
                if(oi.m_hasInterval)
                    return std::make_shared<NextWeak>(exprLeft, std::make_shared<Interval>(tstart, tend));
                else
                    return std::make_shared<NextWeak>(exprLeft);
            }

            if(oi.m_name == OP_HISTORICALLY)
            {
                if(oi.m_hasInterval)
                    return std::make_shared<Historically>(exprLeft, std::make_shared<Interval>(tstart, tend));
                else
                    return std::make_shared<Historically>(exprLeft);
            }

            if(oi.m_name == OP_ONCE)
            {
                if(oi.m_hasInterval)
                    return std::make_shared<Once>(exprLeft, std::make_shared<Interval>(tstart, tend));
                else
                    return std::make_shared<Once>(exprLeft);
            }

            if(oi.m_name == OP_PREVIOUSLY)
            {
                if(oi.m_hasInterval)
                    return std::make_shared<Previously>(exprLeft, std::make_shared<Interval>(tstart, tend));
                else
                    return std::make_shared<Previously>(exprLeft);
            }
        }
        /**
         * If binary operator
         */
        else if(oi.m_name == OP_AND ||
                 oi.m_name == OP_OR ||
                 oi.m_name == OP_XOR ||
                 oi.m_name == OP_IF ||
                 oi.m_name == OP_IFF ||
                 oi.m_name == OP_SINCE)
        {
            auto nrLeft = rint(0, nrOperators - 1);
            auto exprLeft = GenerateExpression(nrLeft);
            auto exprRight = GenerateExpression(nrOperators - 1 - nrLeft);

            if(oi.m_name == OP_AND)
                return std::make_shared<And>(std::set<std::shared_ptr<Expression>, NaryInterface::SharedPtrExprCompare>{exprLeft, exprRight});
            if(oi.m_name == OP_OR)
                return std::make_shared<Or>(std::set<std::shared_ptr<Expression>, NaryInterface::SharedPtrExprCompare>{exprLeft, exprRight});
            if(oi.m_name == OP_XOR)
                return std::make_shared<Xor>(std::multiset<std::shared_ptr<Expression>, NaryInterface::SharedPtrExprCompare>{exprLeft, exprRight});
            if(oi.m_name == OP_IF)
                return std::make_shared<Implication>(exprRight, exprLeft);
            if(oi.m_name == OP_IFF)
                return std::make_shared<Equivalence>(exprRight, exprLeft);
            if(oi.m_name == OP_SINCE)
                return std::make_shared<Since>(exprRight, exprLeft);
        }

    }

    return std::make_shared<Atomic>("z");
}
