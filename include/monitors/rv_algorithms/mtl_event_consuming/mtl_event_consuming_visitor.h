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
    void consumeEvent(Formula &formula, const std::shared_ptr<Trace> &trace, size_t index);

    /**
     * Logic for traversing entire expression tree.
     * @param expr Expression.
     * @param event Event.
     */
    // void visitExpression(Expression &expr, const std::shared_ptr<Trace> &trace, size_t index);

    /**
     * Converts op to a shared_ptr.
     * @param opr Atomic.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitAtomic(Atomic &opr, const std::shared_ptr<Trace> &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param opr Constant.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitConstant(Constant &opr, const std::shared_ptr<Trace> &trace,
                                              size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param opr And.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitAnd(And &opr, const std::shared_ptr<Trace> &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param opr Or.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitOr(Or &opr, const std::shared_ptr<Trace> &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param opr Xor.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitXor(Xor &opr, const std::shared_ptr<Trace> &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param opr Implication.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitImplication(Implication &opr, const std::shared_ptr<Trace> &trace,
                                                 size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param opr Equivalence.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitEquivalence(Equivalence &opr, const std::shared_ptr<Trace> &trace,
                                                 size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param opr Not.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitNot(Not &opr, const std::shared_ptr<Trace> &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param opr Future.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitFuture(Future &opr, const std::shared_ptr<Trace> &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param opr Globally.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitGlobally(Globally &opr, const std::shared_ptr<Trace> &trace,
                                              size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param opr Globally.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitHistorically(Historically &opr, const std::shared_ptr<Trace> &trace,
                                                  size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param opr NextStrong.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitNextStrong(NextStrong &opr, const std::shared_ptr<Trace> &trace,
                                                size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param opr NextWeak.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitNextWeak(NextWeak &opr, const std::shared_ptr<Trace> &trace,
                                              size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param opr Once.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitOnce(Once &opr, const std::shared_ptr<Trace> &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param opr Previously.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitPreviously(Previously &opr, const std::shared_ptr<Trace> &trace,
                                                size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param opr Release.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitRelease(Release &opr, const std::shared_ptr<Trace> &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param opr Until.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitUntil(Until &opr, const std::shared_ptr<Trace> &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param opr Since.
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitSince(Since &opr, const std::shared_ptr<Trace> &trace, size_t index) override;

    /**
     * Applies event to its child and applies rewriting rules from resolving past algorithm.
     * @param opr ResolvingOperator.
     * @param event Event.
     * @return Expression transformed according to rewriting rules.
     */
    std::shared_ptr<Expression> visitResolvingOperator(ResolvingOperator &opr, const std::shared_ptr<Trace> &trace,
                                                       size_t index) override;

    /**
     * Applies event to its child and applies rewriting rules from deriving algorithm.
     * @param opr EventOperator.
     * @param event Event.
     * @return Expression transformed according to rewriting rules.
     */
    std::shared_ptr<Expression> visitDerivingOperator(DerivingOperator &opr, const std::shared_ptr<Trace> &trace,
                                                      size_t index) override;

    /**
     * Applies event to its child and applies rewriting rules from event consuming algorithm.
     * @param opr EventOperator.
     * @param event Event.
     * @return Expression transformed according to rewriting rules.
     */
    std::shared_ptr<Expression> visitEventOperator(EventOperator &opr, const std::shared_ptr<Trace> &trace,
                                                   size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param opr Predicate
     * @param event Event.
     * @return op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitPredicate(Predicate &opr, const std::shared_ptr<Trace> &trace,
                                               size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Universal quantifier.
     * @param trace Trace.
     * @return Op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitUniversalQuantifier(UniversalQuantifier &op, const std::shared_ptr<Trace> &trace,
                                                         size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Existential quantifier.
     * @param trace Trace.
     * @return Op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitExistentialQuantifier(ExistentialQuantifier &op,
                                                           const std::shared_ptr<Trace> &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Quote quantifier.
     * @param trace Trace.
     * @return Op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitQuoteQuantifier(QuoteQuantifier &op, const std::shared_ptr<Trace> &trace,
                                                     size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Counting quantifier.
     * @param trace Trace.
     * @return Op as a shared_ptr.
     */
    std::shared_ptr<Expression> visitCountingQuantifier(CountingQuantifier &op, const std::shared_ptr<Trace> &trace,
                                                        size_t index) override;
};
