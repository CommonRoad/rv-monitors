//
// Created by Sebastian Maierhofer and Evald Nexhipi.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include "monitors/formula/formula.h"

/**
 * Applies rules to transform formula into conjunction of exclusive disjunctions.
 */
class MtlRewriteVisitor : public ExpressionVisitor {
  public:
    void rewrite(Formula &formula);

    // void visitExpression(Expression &expr, Trace &trace, size_t index);

    [[nodiscard]] std::shared_ptr<Expression> visitAtomic(Atomic &op, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitConstant(Constant &op, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitAnd(And &op, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitOr(Or &op, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitXor(Xor &op, Trace &trace, size_t index) override;

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

    [[nodiscard]] std::shared_ptr<Expression> visitDerivingOperator(DerivingOperator &op, Trace &trace,
                                                                    size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitResolvingOperator(ResolvingOperator &op, Trace &trace,
                                                                     size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitPredicate(Predicate &op, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitUniversalQuantifier(UniversalQuantifier &op, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitExistentialQuantifier(ExistentialQuantifier &op, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitQuoteQuantifier(QuoteQuantifier &op, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitCountingQuantifier(CountingQuantifier &op, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitEqualityOperator(EqualityOperator &op, Trace &trace, size_t index) override;
};