#pragma once

#include "monitors/formula/formula.h"

/**
 * Applies rules to transform formula into conjunction of exclusive disjunctions.
 */
class RewriteVisitor : public ExpressionVisitor {
  public:
    void rewrite(Formula &formula);

    //     void visitExpression(Expression &expr, const std::shared_ptr<Trace> &trace, size_t index);

    [[nodiscard]] std::shared_ptr<Expression> visitAtomic(Atomic &opr, const std::shared_ptr<Trace> &trace,
                                                          size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitConstant(Constant &opr, const std::shared_ptr<Trace> &trace,
                                                            size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitAnd(And &opr, const std::shared_ptr<Trace> &trace,
                                                       size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitOr(Or &opr, const std::shared_ptr<Trace> &trace,
                                                      size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitXor(Xor &opr, const std::shared_ptr<Trace> &trace,
                                                       size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitImplication(Implication &opr, const std::shared_ptr<Trace> &trace,
                                                               size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitEquivalence(Equivalence &opr, const std::shared_ptr<Trace> &trace,
                                                               size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitNot(Not &opr, const std::shared_ptr<Trace> &trace,
                                                       size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitFuture(Future &opr, const std::shared_ptr<Trace> &trace,
                                                          size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitGlobally(Globally &opr, const std::shared_ptr<Trace> &trace,
                                                            size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitNextStrong(NextStrong &opr, const std::shared_ptr<Trace> &trace,
                                                              size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitNextWeak(NextWeak &opr, const std::shared_ptr<Trace> &trace,
                                                            size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitOnce(Once &opr, const std::shared_ptr<Trace> &trace,
                                                        size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitPreviously(Previously &opr, const std::shared_ptr<Trace> &trace,
                                                              size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitHistorically(Historically &opr, const std::shared_ptr<Trace> &trace,
                                                                size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitRelease(Release &opr, const std::shared_ptr<Trace> &trace,
                                                           size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitUntil(Until &opr, const std::shared_ptr<Trace> &trace,
                                                         size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitSince(Since &opr, const std::shared_ptr<Trace> &trace,
                                                         size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression>
    visitEventOperator(EventOperator &opr, const std::shared_ptr<Trace> &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression>
    visitResolvingOperator(ResolvingOperator &opr, const std::shared_ptr<Trace> &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression>
    visitDerivingOperator(DerivingOperator &opr, const std::shared_ptr<Trace> &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitPredicate(Predicate &opr, const std::shared_ptr<Trace> &trace,
                                                             size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression>
    visitUniversalQuantifier(UniversalQuantifier &opr, const std::shared_ptr<Trace> &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression>
    visitExistentialQuantifier(ExistentialQuantifier &opr, const std::shared_ptr<Trace> &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression>
    visitQuoteQuantifier(QuoteQuantifier &opr, const std::shared_ptr<Trace> &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression>
    visitCountingQuantifier(CountingQuantifier &opr, const std::shared_ptr<Trace> &trace, size_t index) override;
};
