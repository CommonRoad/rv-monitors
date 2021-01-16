#pragma once

#include "Formula/Formula.h"

/**
 * Applies rules to transform formula into conjunction of exclusive disjunctions.
 */
class MTLRewriteVisitor : public ExpressionVisitor {
public:
    void rewrite(Formula &formula);

    //void visitExpression(Expression &expr, Trace &trace, int index);

    [[nodiscard]] std::shared_ptr<Expression> visitAtomic(Atomic &op, Trace &trace, int index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitConstant(Constant &op, Trace &trace, int index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitAnd(And &op, Trace &trace, int index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitOr(Or &op, Trace &trace, int index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitXor(Xor &op, Trace &trace, int index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitImplication(Implication &op, Trace &trace, int index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitEquivalence(Equivalence &op, Trace &trace, int index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitNot(Not &op, Trace &trace, int index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitFuture(Future &op, Trace &trace, int index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitGlobally(Globally &op, Trace &trace, int index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitNextStrong(NextStrong &op, Trace &trace, int index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitNextWeak(NextWeak &op, Trace &trace, int index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitOnce(Once &op, Trace &trace, int index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitPreviously(Previously &op, Trace &trace, int index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitHistorically(Historically &op, Trace &trace, int index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitRelease(Release &op, Trace &trace, int index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitUntil(Until &op, Trace &trace, int index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitSince(Since &op, Trace &trace, int index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitEventOperator(EventOperator &op, Trace &trace, int index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitDerivingOperator(DerivingOperator &op, Trace &trace, int index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitResolvingOperator(ResolvingOperator &op, Trace &trace, int index) override;

};



