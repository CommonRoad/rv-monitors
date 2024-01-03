#pragma once

#include "monitors/formula/formula.h"
#include "monitors/rewriting/expression_visitor.h"

/**
 * Visitor applying the event consuming rules to a formula and propagating the EventOperator down the expression tree.
 */
class EventConsumingVisitor : public ExpressionVisitor {

  public:
    /**
     * Binds a formula and an event to an EventOperator.
     * @param formula Formula.
     * @param trace Trace.
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
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitAtomic(Atomic &opr, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Constant.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitConstant(Constant &opr, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op And.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitAnd(And &opr, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Or.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitOr(Or &opr, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Xor.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitXor(Xor &opr, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Implication.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitImplication(Implication &opr, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Equivalence.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitEquivalence(Equivalence &opr, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Not.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitNot(Not &opr, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Future.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitFuture(Future &opr, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Globally.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitGlobally(Globally &opr, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op NextStrong.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitNextStrong(NextStrong &opr, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op NextWeak.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitNextWeak(NextWeak &opr, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Once.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitOnce(Once &opr, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Previously.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitPreviously(Previously &opr, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Release.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitRelease(Release &opr, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Until.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitUntil(Until &opr, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Since.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitSince(Since &opr, Trace &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Historically.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitHistorically(Historically &opr, Trace &trace, size_t index) override;

    /**
     * Applies event to its child and applies rewriting rules from event consuming algorithm.
     * @param op EventOperator.
     * @param event Event.
     * @return Expression transformed according to rewriting rules.
     */
    std::shared_ptr<Expression> visitEventOperator(EventOperator &opr, Trace &trace, size_t index) override;

    std::shared_ptr<Expression> visitResolvingOperator(ResolvingOperator &opr, Trace &trace, size_t index) override;

    std::shared_ptr<Expression> visitDerivingOperator(DerivingOperator &opr, Trace &trace, size_t index) override;

    std::shared_ptr<Expression> visitPredicate(Predicate &opr, Trace &trace, size_t index) override;

    std::shared_ptr<Expression> visitUniversalQuantifier(UniversalQuantifier &opr, Trace &trace, size_t index) override;

    std::shared_ptr<Expression> visitExistentialQuantifier(ExistentialQuantifier &opr, Trace &trace,
                                                           size_t index) override;

    std::shared_ptr<Expression> visitQuoteQuantifier(QuoteQuantifier &opr, Trace &trace, size_t index) override;

    std::shared_ptr<Expression> visitCountingQuantifier(CountingQuantifier &opr, Trace &trace, size_t index) override;
};
