//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include "monitors/collections/domain_collection.h"
#include "monitors/collections/function_collection.h"
#include <any>
#include <functional>
#include <map>
#include <memory>
#include <vector>

class Hydra2Monitor;

class Formula;

class Expression;

class Unary;

class Binary;

class nary_interface;

class Atomic;

class Constant;

class And;

class Or;

class Xor;

class Implication;

class Not;

class Future;

class Globally;

class NextStrong;

class NextWeak;

class Once;

class Previously;

class Release;

class Until;

class Since;

class Historically;

class Trace;

class EventOperator;

class ResolvingOperator;

class DerivingOperator;

class Equivalence;

class Predicate;

class UniversalQuantifier;

class ExistentialQuantifier;

class QuoteQuantifier;

class CountingQuantifier;

class EqualityOperator;

/**
 * Creates Hydra2 monitors.
 */
class HydraCreationVisitor2 {
  public:
    /**
     * Constructor for Hydra2 monitor.
     *
     * @param predicateCosts Cost assigned to predicates consisting of evaluation cost in interval [0, 100] and
     * satisfaction probability in interval [0, 1].
     * @param reordering Boolean indicating whether the formula should be reordered based on the predicate costs.
     * @param shortCircuiting Boolean indicating whether short-circuiting should be used.
     */
    HydraCreationVisitor2(std::map<std::string, std::array<double, 2>> predicate_costs,
                         DomainCollection &domains,
                         FunctionCollection &functions,
                         bool reordering = true, bool shortCircuiting = true);

    std::shared_ptr<Hydra2Monitor> createMonitor(Formula &formula, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<Hydra2Monitor> visitAtomic(Atomic &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<Hydra2Monitor> visitConstant(Constant &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<Hydra2Monitor> visitPreviously(Previously &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<Hydra2Monitor> visitPredicate(Predicate &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<Hydra2Monitor> visitNext(NextStrong &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<Hydra2Monitor> visitSince(Since &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<Hydra2Monitor> visitUntil(Until &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<Hydra2Monitor> visitOr(Or &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<Hydra2Monitor> visitAnd(And &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<Hydra2Monitor> visitNot(Not &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<Hydra2Monitor> visitUniversalQuantifier(UniversalQuantifier &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<Hydra2Monitor> visitExistentialQuantifier(ExistentialQuantifier &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<Hydra2Monitor> visitQuoteQuantifier(QuoteQuantifier &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<Hydra2Monitor> visitCountingQuantifier(CountingQuantifier &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<Hydra2Monitor> visitEqualityOperator(EqualityOperator &tlop, size_t parentId, bool connectedEval);

  private:
    size_t maxId{0}; //**< Maximum ID which is currently assigned to sub-monitor. */
    bool reordering; //**< Boolean indicating whether the formula should be reordered based on the predicate costs. */
    bool shortCircuiting; //**< Boolean indicating whether short-circuiting should be used. */
    std::map<std::string, std::shared_ptr<Hydra2Monitor>> baseline; //**< Stores unique formula sub-sets which can be shared within monitor. Only relevant for connected evaluation */
    std::map<std::string, std::array<double, 2>> predicateCosts; //**< Cost assigned to predicates consisting of evaluation cost in interval [0, 100] and satisfaction probability in interval [0, 1]. */
    DomainCollection domains; //**< Domains of quantified variables. */
    FunctionCollection functions; //**< Functions used for the evaluation of predicates. */

    /**
     * Resorts the formula based on predicate costs.
     *
     * @param monitors List of monitors.
     * @param increasing Boolean indicating whether predicates should be ordered in increasing or decreasing order based
     * on cost value (and-> increasing, or->decreasing).
     */
    static void resort(std::vector<std::shared_ptr<Hydra2Monitor>> &monitors, bool increasing);
};
