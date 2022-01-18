//
// Created by Sebastian Maierhofer and Evald Nexhipi.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include "monitors/formula/formula.h"
#include "monitors/rewriting/expression_visitor.h"

/**
 * Visitor applying the event consuming rules to a formula and propagating the EventOperator down the expression tree.
 */
class MTLEventConsumingVisitor : public ExpressionVisitor {

  public:
    /**
     * Binds a formula and an event to an EventOperator.
     * @param formula Formula.
     * @param event Event.
     */
    void consumeEvent(Formula &formula, Trace &trace, size_t index);

    /**
     * Logic for traversing entire expression tree.
     * @param expr Expression.
     * @param event Event.
     */
    // void visitExpression(Expression &expr, Trace &trace, size_t index);

    /**
     * Converts op to a shared_ptr.
     * @param op Atomic.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitAtomic(Atomic &op, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Constant.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitConstant(Constant &op, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op And.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitAnd(And &op, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Or.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitOr(Or &op, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Xor.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitXor(Xor &op, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Implication.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitImplication(Implication &op, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Equivalence.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitEquivalence(Equivalence &op, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Not.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitNot(Not &op, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Future.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitFuture(Future &op, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Globally.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitGlobally(Globally &op, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Globally.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitHistorically(Historically &op, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op NextStrong.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitNextStrong(NextStrong &op, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op NextWeak.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitNextWeak(NextWeak &op, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Once.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitOnce(Once &op, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Previously.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitPreviously(Previously &op, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Release.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitRelease(Release &op, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Until.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitUntil(Until &op, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Since.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitSince(Since &op, Trace &trace, size_t index) override;

    /**
     * Applies event to its child and applies rewriting rules from resolving past algorithm.
     * @param op ResolvingOperator.
     * @param event Event.
     * @return Expression transformed according to rewriting rules.
     */
    std::shared_ptr<Expression> visitResolvingOperator(ResolvingOperator &op, Trace &trace, size_t index) override;

    /**
     * Applies event to its child and applies rewriting rules from deriving algorithm.
     * @param op EventOperator.
     * @param event Event.
     * @return Expression transformed according to rewriting rules.
     */
    std::shared_ptr<Expression> visitDerivingOperator(DerivingOperator &op, Trace &trace, size_t index) override;

    /**
     * Applies event to its child and applies rewriting rules from event consuming algorithm.
     * @param op EventOperator.
     * @param event Event.
     * @return Expression transformed according to rewriting rules.
     */
    std::shared_ptr<Expression> visitEventOperator(EventOperator &op, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Predicate
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitPredicate(Predicate &op, Trace &trace, size_t index) override;
};
