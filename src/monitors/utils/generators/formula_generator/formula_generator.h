//
// Created by Evald Nexhipi.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//

#pragma once

#include "monitors/formula/formula.h"

#include "monitors/expression_tree/atomic/atomic.h"
#include "monitors/expression_tree/atomic/predicate.h"
#include "monitors/expression_tree/binary/bool/equivalence.h"
#include "monitors/expression_tree/binary/bool/implication.h"
#include "monitors/expression_tree/nary/bool/and.h"
#include "monitors/expression_tree/nary/bool/or.h"

#include "monitors/expression_tree/nary/bool/xor.h"
#include "monitors/expression_tree/unary/bool/not.h"
#include <utility>

#include "monitors/expression_tree/unary/temporal/ltlf/future.h"
#include "monitors/expression_tree/unary/temporal/ltlf/globally.h"
#include "monitors/expression_tree/unary/temporal/ltlf/next_strong.h"
#include "monitors/expression_tree/unary/temporal/ltlf/next_weak.h"

#include "monitors/expression_tree/binary/temporal/ptlf/since.h"
#include "monitors/expression_tree/unary/temporal/ptlf/historically.h"
#include "monitors/expression_tree/unary/temporal/ptlf/once.h"
#include "monitors/expression_tree/unary/temporal/ptlf/previously.h"

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
     * @param tstart randomly generates a value between m_minStart and m_maxStart
     * @param tend randomly generates a value between m_minDuration and m_maxDuration
     */
    void generateInterval(const OperatorInfo &oi, size_t &tstart, size_t &tend) const {
        tstart = rint(oi.m_minStart, oi.m_maxStart);
        tend = tstart + rint(oi.m_minDuration, oi.m_maxDuration);
    }

    /**
     * @var m_nrOperators total number of operators to be in the formula
     */
    size_t m_nrOperators;
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
    static void rseed() { srand(static_cast<unsigned int>(time(nullptr))); }

    /**
     * Generates a real number
     * @return returns a pseudo-random real number
     */
    static double rreal() { return ((double)rand()) / ((double)RAND_MAX); }

    /**
     * Generates a real number within an interval
     * @param min minimum number to be generated
     * @param max maximum number to be generated
     * @return a real number within (min, max) interval
     */
    static double rreal(const double min, const double max) { return min + (max - min) * rreal(); }

    /**
     * Generates an integer within an interval
     * @param min minimum number to be generated
     * @param max maximum number to be generated
     * @return an integer within (min, max) interval
     */
    static inline size_t rint(const size_t min, const size_t max) {
        const size_t x = min + (size_t)rreal(0, static_cast<double>(max - min + 1));
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
     * @param minStart
     * @param maxStart
     * @param minDuration
     * @param maxDuration
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
     * @param hasInterval check if the operator has an interval
     * @param tstart start time step of the operator
     * @param tend end time step of the operator
     * @return a unary expression
     */
    std::shared_ptr<Expression> generateUnaryExpression(std::string op, const std::shared_ptr<Expression> &exprLeft,
                                                        bool hasInterval, size_t tstart, size_t tend);

    /**
     * Helper Function to generate Binary Expression
     * @param op operator name
     * @param exprLeft expression on the left
     * @param exprRight expression on the right
     * @return a binary expression
     */
    std::shared_ptr<Expression> generateBinaryExpression(std::string op, const std::shared_ptr<Expression> &exprLeft,
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