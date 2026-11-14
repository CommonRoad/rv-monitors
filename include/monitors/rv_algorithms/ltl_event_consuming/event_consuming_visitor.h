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
    void consumeEvent(Formula &formula, const std::shared_ptr<Trace> &trace, size_t index);

    /**
     * Converts op to a shared_ptr.
     * @param op Atomic.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitAtomic(Atomic &opr, const std::shared_ptr<Trace> &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Constant.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitConstant(Constant &opr, const std::shared_ptr<Trace> &trace,
                                              size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op And.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitAnd(And &opr, const std::shared_ptr<Trace> &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Or.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitOr(Or &opr, const std::shared_ptr<Trace> &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Xor.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitXor(Xor &opr, const std::shared_ptr<Trace> &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Implication.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitImplication(Implication &opr, const std::shared_ptr<Trace> &trace,
                                                 size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Equivalence.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitEquivalence(Equivalence &opr, const std::shared_ptr<Trace> &trace,
                                                 size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Not.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitNot(Not &opr, const std::shared_ptr<Trace> &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Future.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitFuture(Future &opr, const std::shared_ptr<Trace> &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Globally.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitGlobally(Globally &opr, const std::shared_ptr<Trace> &trace,
                                              size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op NextStrong.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitNextStrong(NextStrong &opr, const std::shared_ptr<Trace> &trace,
                                                size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op NextWeak.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitNextWeak(NextWeak &opr, const std::shared_ptr<Trace> &trace,
                                              size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Once.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitOnce(Once &opr, const std::shared_ptr<Trace> &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Previously.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitPreviously(Previously &opr, const std::shared_ptr<Trace> &trace,
                                                size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Release.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitRelease(Release &opr, const std::shared_ptr<Trace> &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Until.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitUntil(Until &opr, const std::shared_ptr<Trace> &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Since.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitSince(Since &opr, const std::shared_ptr<Trace> &trace, size_t index) override;

    /**
     * Converts op to a shared_ptr.
     * @param op Historically.
     * @param event Event.
     * @return opr as a shared_ptr.
     */
    std::shared_ptr<Expression> visitHistorically(Historically &opr, const std::shared_ptr<Trace> &trace,
                                                  size_t index) override;

    /**
     * Applies event to its child and applies rewriting rules from event consuming algorithm.
     * @param op EventOperator.
     * @param event Event.
     * @return Expression transformed according to rewriting rules.
     */
    std::shared_ptr<Expression> visitEventOperator(EventOperator &opr, const std::shared_ptr<Trace> &trace,
                                                   size_t index) override;

    std::shared_ptr<Expression> visitResolvingOperator(ResolvingOperator &opr, const std::shared_ptr<Trace> &trace,
                                                       size_t index) override;

    std::shared_ptr<Expression> visitDerivingOperator(DerivingOperator &opr, const std::shared_ptr<Trace> &trace,
                                                      size_t index) override;

    std::shared_ptr<Expression> visitPredicate(Predicate &opr, const std::shared_ptr<Trace> &trace,
                                               size_t index) override;

    std::shared_ptr<Expression> visitUniversalQuantifier(UniversalQuantifier &opr, const std::shared_ptr<Trace> &trace,
                                                         size_t index) override;

    std::shared_ptr<Expression> visitExistentialQuantifier(ExistentialQuantifier &opr,
                                                           const std::shared_ptr<Trace> &trace, size_t index) override;

    std::shared_ptr<Expression> visitQuoteQuantifier(QuoteQuantifier &opr, const std::shared_ptr<Trace> &trace,
                                                     size_t index) override;

    std::shared_ptr<Expression> visitCountingQuantifier(CountingQuantifier &opr, const std::shared_ptr<Trace> &trace,
                                                        size_t index) override;
};
