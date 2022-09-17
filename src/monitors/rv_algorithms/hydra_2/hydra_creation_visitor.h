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
    HydraCreationVisitor2(std::map<std::string, std::array<double, 2>> predicateCosts, bool reordering = true,
                          bool shortCircuiting = true);

    /**
     * Base function for creating Hydra2 monitor.
     *
     * @param formula Formula for which monitor should be created.
     * @param parentId ID of parent monitor.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     * @return Created monitor.
     */
    std::shared_ptr<Hydra2Monitor> createMonitor(Formula &formula, size_t parentId, bool connectedEval);

    /**
     * Visitor for creating Hydra2 Atomic Monitor.
     *
     * @param top Temporal operator expression.
     * @param parentId ID of parent monitor.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     * @return Created monitor.
     */
    [[nodiscard]] std::shared_ptr<Hydra2Monitor> visitAtomic(Atomic &top, size_t parentId, bool connectedEval);

    /**
     * Visitor for creating Hydra2 Constant Monitor.
     *
     * @param top Temporal operator expression.
     * @param parentId ID of parent monitor.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     * @return Created monitor.
     */
    [[nodiscard]] std::shared_ptr<Hydra2Monitor> visitConstant(Constant &top, size_t parentId, bool connectedEval);

    /**
     * Visitor for creating Hydra2 Previous Monitor.
     *
     * @param top Temporal operator expression.
     * @param parentId ID of parent monitor.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     * @return Created monitor.
     */
    [[nodiscard]] std::shared_ptr<Hydra2Monitor> visitPreviously(Previously &top, size_t parentId, bool connectedEval);

    /**
     * Visitor for creating Hydra2 Predicate Monitor.
     *
     * @param top Temporal operator expression.
     * @param parentId ID of parent monitor.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     * @return Created monitor.
     */
    [[nodiscard]] std::shared_ptr<Hydra2Monitor> visitPredicate(Predicate &top, size_t parentId, bool connectedEval);

    /**
     * Visitor for creating Hydra2 Next Monitor.
     *
     * @param top Temporal operator expression.
     * @param parentId ID of parent monitor.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     * @return Created monitor.
     */
    [[nodiscard]] std::shared_ptr<Hydra2Monitor> visitNext(NextStrong &top, size_t parentId, bool connectedEval);

    /**
     * Visitor for creating Hydra2 Since Monitor.
     *
     * @param top Temporal operator expression.
     * @param parentId ID of parent monitor.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     * @return Created monitor.
     */
    [[nodiscard]] std::shared_ptr<Hydra2Monitor> visitSince(Since &top, size_t parentId, bool connectedEval);

    /**
     * Visitor for creating Hydra2 Until Monitor.
     *
     * @param top Temporal operator expression.
     * @param parentId ID of parent monitor.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     * @return Created monitor.
     */
    [[nodiscard]] std::shared_ptr<Hydra2Monitor> visitUntil(Until &top, size_t parentId, bool connectedEval);

    /**
     * Visitor for creating Hydra2 Or Monitor.
     *
     * @param top Temporal operator expression.
     * @param parentId ID of parent monitor.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     * @return Created monitor.
     */
    [[nodiscard]] std::shared_ptr<Hydra2Monitor> visitOr(Or &top, size_t parentId, bool connectedEval);

    /**
     * Visitor for creating Hydra2 And Monitor.
     *
     * @param top Temporal operator expression.
     * @param parentId ID of parent monitor.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     * @return Created monitor.
     */
    [[nodiscard]] std::shared_ptr<Hydra2Monitor> visitAnd(And &top, size_t parentId, bool connectedEval);

    /**
     * Visitor for creating Hydra2 Not Monitor.
     *
     * @param top Temporal operator expression.
     * @param parentId ID of parent monitor.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     * @return Created monitor.
     */
    [[nodiscard]] std::shared_ptr<Hydra2Monitor> visitNot(Not &top, size_t parentId, bool connectedEval);

  private:
    size_t maxId{0}; //**< Maximum ID which is currently assigned to sub-monitor. */
    bool reordering; //**< Boolean indicating whether the formula should be reordered based on the predicate costs. */
    bool shortCircuiting; //**< Boolean indicating whether short-circuiting should be used. */
    std::map<std::string, std::shared_ptr<Hydra2Monitor>>
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
    static void reorder(std::vector<std::shared_ptr<Hydra2Monitor>> &monitors, bool increasing);
};
