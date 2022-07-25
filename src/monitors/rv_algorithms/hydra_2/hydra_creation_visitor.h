//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include <map>
#include <memory>
#include <vector>

class HydraMonitor;

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

/**
 * Creates Hydra monitors.
 */
class HydraCreationVisitor {
  public:
    /**
     * Constructor for Hydra monitor.
     *
     * @param predicateCosts Cost assigned to predicates consisting of evaluation cost in interval [0, 100] and
     * satisfaction probability in interval [0, 1].
     * @param reordering Boolean indicating whether the formula should be reordered based on the predicate costs.
     * @param shortCircuiting Boolean indicating whether short-circuiting should be used.
     */
    HydraCreationVisitor(std::map<std::string, std::array<double, 2>> predicateCosts, bool reordering = true,
                         bool shortCircuiting = true);

    std::shared_ptr<HydraMonitor> createMonitor(Formula &formula, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitAtomic(Atomic &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitConstant(Constant &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitPreviously(Previously &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitPredicate(Predicate &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitNext(NextStrong &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitSince(Since &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitUntil(Until &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitOr(Or &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitAnd(And &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitNot(Not &tlop, size_t parentId, bool connectedEval);

  private:
    size_t maxId{0}; //**< Maximum ID which is currently assigned to sub-monitor. */
    bool reordering; //**< Boolean indicating whether the formula should be reordered based on the predicate costs. */
    bool shortCircuiting; //**< Boolean indicating whether short-circuiting should be used. */
    std::map<std::string, std::shared_ptr<HydraMonitor>>
        baseline; //**< Stores unique formula sub-sets which can be shared within monitor. Only relevant for connected
                  // evaluation */
    std::map<std::string, std::array<double, 2>>
        predicateCosts; //**< Cost assigned to predicates consisting of evaluation cost in interval [0, 100] and
                        // satisfaction probability in interval [0, 1]. */

    /**
     * Resorts the formula based on predicate costs.
     *
     * @param monitors List of monitors.
     * @param increasing Boolean indicating whether predicates should be ordered in increasing or decreasing order based
     * on cost value (and-> increasing, or->decreasing).
     */
    static void resort(std::vector<std::shared_ptr<HydraMonitor>> &monitors, bool increasing);
};
