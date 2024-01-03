#pragma once
#include "monitors/formula/formula.h"
#include <utility>

#include "monitors/expression_tree/atomic/atomic.h"
#include "monitors/expression_tree/binary/bool/equivalence.h"
#include "monitors/expression_tree/binary/bool/implication.h"
#include "monitors/expression_tree/nary/bool/and.h"
#include "monitors/expression_tree/nary/bool/or.h"

#include "monitors/expression_tree/nary/bool/xor.h"
#include "monitors/expression_tree/unary/bool/not.h"

#include "monitors/expression_tree/unary/temporal/future.h"
#include "monitors/expression_tree/unary/temporal/globally.h"
#include "monitors/expression_tree/unary/temporal/historically.h"
#include "monitors/expression_tree/unary/temporal/next_strong.h"
#include "monitors/expression_tree/unary/temporal/next_weak.h"
#include "monitors/expression_tree/unary/temporal/once.h"
#include "monitors/expression_tree/unary/temporal/previously.h"

#include "monitors/expression_tree/binary/temporal/release.h"
#include "monitors/expression_tree/binary/temporal/since.h"
#include "monitors/expression_tree/binary/temporal/until.h"

#include "monitors/symbols/symbols.h"

/**
 * Class to pseudo-randomly generate a formula
 */
class FormulaGenerator {
  protected:
    /**
     * Structure to store the association of a weight to an Atom
     * @var m_atomic atoms to be stored in the structure
     * @var m_weight respective weights for the atoms
     */
    struct AtomicInfo {
        std::shared_ptr<Atomic> m_atomic;
        double m_weight;
    };

    /**
     * Structure to store the association of a weight to a Predicate
     * @var m_atomic atoms to be stored in the structure
     * @var m_weight respective weights for the predicates
     */
    struct PredicateInfo {
        std::shared_ptr<Predicate> m_predicate;
        double m_weight;
    };

    /**
     * Structure to store information regarding operators
     * @var m_name name of the operator
     * @var m_weight respective weight for the operator
     * @var m_hasInterval operators within an interval
     * @var m_minStart minimum Start
     * @var m_maxStart maximum Start
     * @var m_minDuration minimum Duration
     * @var m_maxDuration maximum Duration
     */
    struct OperatorInfo {
        std::string m_name;
        double m_weight;
        bool m_hasInterval;
        size_t m_minStart;
        size_t m_maxStart;
        size_t m_minDuration;
        size_t m_maxDuration;
    };

    /**
     * Interval to be generated for operators on intervals
     * @param oi Operator info
     * @param tStart randomly generates a value between m_minStart and m_maxStart
     * @param tEnd randomly generates a value between m_minDuration and m_maxDuration
     */
    static void generateInterval(const OperatorInfo &oi, size_t &tStart, size_t &tEnd) {
        tStart = rInt(oi.m_minStart, oi.m_maxStart);
        tEnd = tStart + rInt(oi.m_minDuration, oi.m_maxDuration);
    }

    /**
     * @var m_sumWeightsAtomic total sum of atoms weights
     */
    double m_sumWeightsAtomic{0.0};
    /**
     * @var m_sumWeightsPredicate total sum of predicate weights
     */
    double m_sumWeightsPredicate{0.0};
    /**
     * @var m_sumWeightsOperator total sum of operators weights
     */
    double m_sumWeightsOperator{0.0};

    /**
     * @var m_atoms vector containing atoms info
     */
    std::vector<AtomicInfo> m_atoms;
    /**
     * @var m_predicates vector containing predicates info
     */
    std::vector<PredicateInfo> m_predicates;
    /**
     * @var m_operators vector containing operators info
     */
    std::vector<OperatorInfo> m_operators;

  public:
    /**
     * Initializes the sequence of pseudo-random numbers generated when the rand function is called.
     */
    static void rSeed() { srand(static_cast<unsigned int>(time(nullptr))); }

    /**
     * Generates a real number
     * @return returns a pseudo-random real number
     */
    static double rReal() { return ((double)rand()) / ((double)RAND_MAX); }

    /**
     * Generates a real number within an interval
     * @param min minimum number to be generated
     * @param max maximum number to be generated
     * @return a real number within (min, max) interval
     */
    static double rReal(const double min, const double max) { return min + (max - min) * rReal(); }

    /**
     * Generates an integer within an interval
     * @param min minimum number to be generated
     * @param max maximum number to be generated
     * @return an integer within (min, max) interval
     */
    static inline size_t rInt(const size_t min, const size_t max) {
        const size_t x = min + (size_t)rReal(0, static_cast<double>(max - min + 1));
        return x > max ? max : x;
    }

    /**
     * Adds atoms to the Formula Generator
     * @param a Atom to be added
     * @param w Associated weight (larger than 0)
     */
    virtual void addAtomic(std::shared_ptr<Atomic> a, const double w) {
        AtomicInfo ai;
        ai.m_atomic = std::move(a);
        ai.m_weight = w;
        m_atoms.push_back(ai);
        m_sumWeightsAtomic += w;
    }

    /**
     * Adds predicates to the Formula Generator
     * @param p Predicate to be added
     * @param w Associated weight (larger than 0)
     */
    virtual void addPredicate(std::shared_ptr<Predicate> p, const double w) {
        PredicateInfo pi;
        pi.m_predicate = std::move(p);
        pi.m_weight = w;
        m_predicates.push_back(pi);
        m_sumWeightsPredicate += w;
    }

    /**
     * Operators to be added
     * @param name Operator to be added
     * @param w Associated weight (larger than 0)
     */
    virtual void addOperator(const std::string &name, const double w) {
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
     * @param minStart Minimum value for the interval start
     * @param maxStart Maximum value for the interval end
     * @param minDuration Minimum value for the duration of the interval
     * @param maxDuration Maximum value for the duration of the interval
     */
    virtual void addOperatorWithInterval(const std::string &name, const double w, const size_t minStart,
                                         const size_t maxStart, const size_t minDuration, const size_t maxDuration) {
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
     * Helper Function to generate Unary Expression
     * @param op operator name
     * @param exprLeft expression on the left
     * @param hasInterval checks if the operator has an interval
     * @param tStart start time step of the operator
     * @param tEnd end time step of the operator
     * @return a unary expression
     */
    static std::shared_ptr<Expression> generateUnaryExpression(const std::string &op,
                                                               const std::shared_ptr<Expression> &exprLeft,
                                                               bool hasInterval, size_t tStart, size_t tEnd);

    /**
     * Helper Function to generate Unary Expression with interval
     * @param op operator name
     * @param exprLeft expression on the left
     * @param tStart start time step of the operator
     * @param tEnd end time step of the operator
     * @return a unary expression
     */
    static std::shared_ptr<Expression> generateUnaryExpressionWithInterval(const std::string &op,
                                                                           const std::shared_ptr<Expression> &exprLeft,
                                                                           size_t tStart, size_t tEnd);

    /**
     * Helper Function to generate Unary Expression with interval
     * @param op operator name
     * @param exprLeft expression on the left
     * @return a unary expression
     */
    static std::shared_ptr<Expression>
    generateUnaryExpressionWithoutInterval(const std::string &op, const std::shared_ptr<Expression> &exprLeft);

    /**
     * Helper Function to generate Binary Expression
     * @param op operator name
     * @param exprLeft expression on the left
     * @param exprRight expression on the right
     * @param hasInterval checks if the operator has an interval
     * @param tStart start time of the operator
     * @param tEnd end time of the operator
     * @return a binary expression
     */
    static std::shared_ptr<Expression> generateBinaryExpression(const std::string &op,
                                                                const std::shared_ptr<Expression> &exprLeft,
                                                                const std::shared_ptr<Expression> &exprRight,
                                                                bool hasInterval, size_t tStart, size_t tEnd);

    /**
     * Helper function to generate Binary Expression with Interval
     * @param op operator name
     * @param exprLeft expression on the left
     * @param exprRight expression on the right
     * @param tStart start time of the operator
     * @param tEnd end time of the operator
     * @return a binary expression with interval
     */
    static std::shared_ptr<Expression>
    generateBinaryExpressionWithInterval(const std::string &op, const std::shared_ptr<Expression> &exprLeft,
                                         const std::shared_ptr<Expression> &exprRight, size_t tStart, size_t tEnd);

    /**
     * Helper function to generate Binary Expression without Interval
     * @param op operator name
     * @param exprLeft expression on the left
     * @param exprRight expression on the right
     * @return a binary expression without interval
     */
    static std::shared_ptr<Expression>
    generateBinaryExpressionWithoutInterval(const std::string &op, const std::shared_ptr<Expression> &exprLeft,
                                            const std::shared_ptr<Expression> &exprRight);
    /**
     * Generates either an atom or predicate
     * @return an atom or predicate
     */
    std::shared_ptr<Expression> generateAtomOrPredicate();

    /**
     * Chooses an index in operators vector randomly
     * @param pos index in the operators vector
     */
    void pickOperator(size_t &pos);

    /**
     * Expression is generated based on the given number of operators
     * @param nrOperators number of operators to be added in the Formula
     * @return returns a recursively generated expression
     */
    std::shared_ptr<Expression> generateExpression(size_t nrOperators);

    /**
     * Formula to be generated
     * @param nrOperators number of operators to be added in the Formula
     * @return return the pseudo-randomly generated Formula
     */
    Formula generateFormula(const size_t nrOperators) {
        Formula f(generateExpression(nrOperators));
        return f;
    }
};