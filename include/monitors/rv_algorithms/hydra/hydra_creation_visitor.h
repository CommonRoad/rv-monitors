#pragma once

#include <memory>

class HydraMonitor;

class Formula;

class Expression;

class Unary;

class Binary;

class nary_interface;

class Atomic;

class Constant;

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

class HydraCreationVisitor {
  public:
    /**
     * Base function for creating Hydra monitor.
     *
     * @param formula Formula for which monitor should be created.
     * @return Created monitor.
     */
    std::shared_ptr<HydraMonitor> createMonitor(Formula &formula);

    /**
     * Visitor for creating Hydra Atomic Monitor.
     *
     * @param top Temporal operator expression.
     * @return Created monitor.
     */
    [[nodiscard]] static std::shared_ptr<HydraMonitor> visitAtomic(Atomic &top);

    /**
     * Visitor for creating Hydra Constant Monitor.
     *
     * @param top Temporal operator expression.
     * @return Created monitor.
     */
    [[nodiscard]] static std::shared_ptr<HydraMonitor> visitConstant(Constant &top);

    /**
     * Visitor for creating Hydra Previously Monitor.
     *
     * @param top Temporal operator expression.
     * @return Created monitor.
     */
    [[nodiscard]] std::shared_ptr<HydraMonitor> visitPreviously(Previously &top);

    /**
     * Visitor for creating Hydra Predicate Monitor.
     *
     * @param top Temporal operator expression.
     * @return Created monitor.
     */
    [[nodiscard]] static std::shared_ptr<HydraMonitor> visitPredicate(Predicate &top);

    /**
     * Visitor for creating Hydra Next Monitor.
     *
     * @param top Temporal operator expression.
     * @return Created monitor.
     */
    [[nodiscard]] std::shared_ptr<HydraMonitor> visitNext(NextStrong &top);

    /**
     * Visitor for creating Hydra Since Monitor.
     *
     * @param top Temporal operator expression.
     * @return Created monitor.
     */
    [[nodiscard]] std::shared_ptr<HydraMonitor> visitSince(Since &top);

    /**
     * Visitor for creating Hydra Until Monitor.
     *
     * @param top Temporal operator expression.
     * @return Created monitor.
     */
    [[nodiscard]] std::shared_ptr<HydraMonitor> visitUntil(Until &top);

    /**
     * Visitor for creating Hydra Or Monitor.
     *
     * @param top Temporal operator expression.
     * @return Created monitor.
     */
    [[nodiscard]] std::shared_ptr<HydraMonitor> visitOr(Or &top);

    /**
     * Visitor for creating Hydra And Monitor.
     *
     * @param top Temporal operator expression.
     * @return Created monitor.
     */
    [[nodiscard]] std::shared_ptr<HydraMonitor> visitAnd(And &top);

    /**
     * Visitor for creating Hydra Negation Monitor.
     *
     * @param top Temporal operator expression.
     * @return Created monitor.
     */
    [[nodiscard]] std::shared_ptr<HydraMonitor> visitNot(Not &top);
};
