//
// Created by Evald Nexhipi on 20.10.21.
//
#ifndef RVMONITORS_FORMULA_GENERATOR_H
#define RVMONITORS_FORMULA_GENERATOR_H

#include "monitors/formula/formula.h"

#include "monitors/expression_tree/atomic/atomic.h"
#include "monitors/expression_tree/atomic/predicate.h"
#include "monitors/expression_tree/unary/bool/not.h"
#include "monitors/expression_tree/nary/bool/and.h"
#include "monitors/expression_tree/nary/bool/or.h"
#include "monitors/expression_tree/nary/bool/xor.h"
#include "monitors/expression_tree/binary/bool/implication.h"
#include "monitors/expression_tree/binary/bool/implication.h"
#include "monitors/expression_tree/binary/bool/equivalence.h"

#include "monitors/expression_tree/unary/temporal/ltlf/future.h"
#include "monitors/expression_tree/unary/temporal/ltlf/globally.h"
#include "monitors/expression_tree/unary/temporal/ltlf/next_strong.h"
#include "monitors/expression_tree/unary/temporal/ltlf/next_weak.h"

#include "monitors/expression_tree/binary/temporal/ptlf/since.h"
#include "monitors/expression_tree/unary/temporal/ptlf/historically.h"
#include "monitors/expression_tree/unary/temporal/ptlf/once.h"
#include "monitors/expression_tree/unary/temporal/ptlf/previously.h"

const std::string OP_NOT = "!";
const std::string OP_AND = "&";
const std::string OP_OR = "|";
const std::string OP_XOR = "^";
const std::string OP_IF = "->";
const std::string OP_IFF = "<->";

const std::string OP_FUTURE = "F";
const std::string OP_GLOBALLY = "G";
const std::string OP_NEXT_STRONG = "NS";
const std::string OP_NEXT_WEAK = "NW";

const std::string OP_SINCE = "S";
const std::string OP_HISTORICALLY = "H";
const std::string OP_ONCE = "O";
const std::string OP_PREVIOUSLY = "P";

class FormulaGenerator
{
  public:
    FormulaGenerator(void) :
                             m_sumWeightsAtomic(0.0),
                             m_sumWeightsPredicate(0.0),
                             m_sumWeightsOperator(0.0)
    {

    }

    virtual ~FormulaGenerator(void)
    {
    }

    /**
     * Initializes the sequence of pseudo-random numbers generated when the rand function is called.
     */
    static void rseed(void)
    {
        srand (time(NULL));
    }

    static double rreal(void)
    {
        return ((double) rand()) / ((double) RAND_MAX);
    }

    static double rreal(const double min, const double max)
    {
        return min + (max - min) * rreal();
    }

    static inline long rint(const long min, const long max)
    {
        const long x = min + (long)  rreal(0, max - min + 1);
        return x > max ? max : x;
    }

    /**
     * Adds atoms to the Formula Generator
     * @param a Atom to be added
     * @param w Associated weight
     */
    virtual void AddAtomic(std::shared_ptr<Atomic> a, const double w)
    {
        AtomicInfo ai;
        ai.m_atomic = a;
        ai.m_weight = w;
        m_atoms.push_back(ai);
        m_sumWeightsAtomic += w;
    }

    /**
     * Adds predicates to the Formula Generator
     * @param p Predicate to be added
     * @param w Associated weight
     */
    virtual void AddPredicate(std::shared_ptr<Predicate> p, const double w)
    {
        PredicateInfo pi;
        pi.m_predicate = p;
        pi.m_weight = w;
        m_predicates.push_back(pi);
        m_sumWeightsPredicate += w;
    }

    /**
     * Operators to be added
     * @param name Operator to be added
     * @param w Associated weight
     */
    virtual void AddOperator(const std::string & name, const double w)
    {
        OperatorInfo oi;
        oi.m_name = name;
        oi.m_weight = w;
        oi.m_hasInterval = false;
        m_operators.push_back(oi);
        m_sumWeightsOperator += w;
    }

    /**
     * Operators added on intervals
     * @param name Name of the operator
     * @param w Associated weight of the operator
     * @param minStart
     * @param maxStart
     * @param minDuration
     * @param maxDuration
     */
    virtual void AddOperatorWithInterval(const std::string & name, const double w,  const int minStart, const int maxStart, const int minDuration, const int maxDuration)
    {
        OperatorInfo oi;
        oi.m_name = name;
        oi.m_weight = w;
        oi.m_hasInterval = true;
        oi.m_minStart = minStart;
        oi.m_maxStart = maxStart;
        oi.m_minDuration = minDuration;
        oi.m_maxDuration = maxDuration;
        m_operators.push_back(oi);
        m_sumWeightsOperator += w;
    }

    /**
     * Expression is generated based on the given number of operators
     * @param nrOperators number of operators to be added in the Formula
     * @return returns a recursively generated expression
     */
    virtual std::shared_ptr<Expression> GenerateExpression(const int nrOperators);

    /**
     * Formula to be generated
     * @param nrOperators number of operators to be added in the Formula
     * @return return the pseudo-randomly generated Formula
     */
    virtual Formula GenerateFormula(const int nrOperators)
    {
        Formula f(GenerateExpression(nrOperators));
        return f;
    }

  protected:
    struct AtomicInfo
    {
        std::shared_ptr<Atomic> m_atomic;
        double m_weight;
    };

    struct PredicateInfo
    {
        std::shared_ptr<Predicate> m_predicate;
        double m_weight;
    };

    struct OperatorInfo
    {
        std::string m_name;
        double m_weight;
        bool m_hasInterval;
        int m_minStart;
        int m_maxStart;
        int m_minDuration;
        int m_maxDuration;
    };

    /**
     * Interval to be generated for operators on intervals
     * @param oi Operator info
     * @param tstart contains minStart and maxStart of the interval
     * @param tend contains minDuration and maxDuration of the interval
     */
    virtual void GenerateInterval(const OperatorInfo & oi, int & tstart, int & tend) const
    {
        tstart = rint(oi.m_minStart, oi.m_maxStart);
        tend = tstart + rint(oi.m_minDuration, oi.m_maxDuration);
    }

    int m_nrOperators;
    double m_sumWeightsAtomic;
    double m_sumWeightsPredicate;
    double m_sumWeightsOperator;

    std::vector<AtomicInfo> m_atoms;
    std::vector<PredicateInfo> m_predicates;
    std::vector<OperatorInfo> m_operators;
};

#endif // RVMONITORS_FORMULA_GENERATOR_H