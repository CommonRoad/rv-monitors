#pragma once

#include "monitors/formula/formula.h"

/**
 * Reduces formulas.
 */
class BasicRewriteVisitor : public ExpressionVisitor {
  public:
    void rewrite(Formula &formula);

    // void visitExpression(Expression &expr, Trace &trace, size_t index);

    [[nodiscard]] std::shared_ptr<Expression> visitAtomic(Atomic &op, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitConstant(Constant &op, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitAnd(And &op, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitOr(Or &itrOuter, Trace &itrInner, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitXor(Xor &itrOuter, Trace &itrInner, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitImplication(Implication &op, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitEquivalence(Equivalence &op, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitNot(Not &op, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitFuture(Future &op, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitGlobally(Globally &op, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitNextStrong(NextStrong &op, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitNextWeak(NextWeak &op, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitOnce(Once &op, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitPreviously(Previously &op, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitHistorically(Historically &op, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitRelease(Release &op, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitUntil(Until &op, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitSince(Since &op, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitEventOperator(EventOperator &op, Trace &trace,
                                                                 size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitResolvingOperator(ResolvingOperator &op, Trace &trace,
                                                                     size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitDerivingOperator(DerivingOperator &op, Trace &trace,
                                                                    size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitPredicate(Predicate &op, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitUniversalQuantifier(UniversalQuantifier &op, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitExistentialQuantifier(ExistentialQuantifier &op, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitCountingQuantifier(CountingQuantifier &op, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitSameFunc(SameFunc &op, Trace &trace, size_t index) override;
};
