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
class RewriteVisitor : public ExpressionVisitor {
  public:
    void rewrite(Formula &formula);

    //     void visitExpression(Expression &expr, Trace &trace, size_t index);

    [[nodiscard]] std::shared_ptr<Expression> visitAtomic(Atomic &opr, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitConstant(Constant &opr, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitAnd(And &opr, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitOr(Or &opr, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitXor(Xor &opr, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitImplication(Implication &opr, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitEquivalence(Equivalence &opr, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitNot(Not &opr, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitFuture(Future &opr, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitGlobally(Globally &opr, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitNextStrong(NextStrong &opr, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitNextWeak(NextWeak &opr, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitOnce(Once &opr, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitPreviously(Previously &opr, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitHistorically(Historically &opr, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitRelease(Release &opr, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitUntil(Until &opr, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitSince(Since &opr, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitEventOperator(EventOperator &opr, Trace &trace,
                                                                 size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitResolvingOperator(ResolvingOperator &opr, Trace &trace,
                                                                     size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitDerivingOperator(DerivingOperator &opr, Trace &trace,
                                                                    size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitPredicate(Predicate &opr, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitUniversalQuantifier(UniversalQuantifier &opr, Trace &trace,
                                                                       size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitExistentialQuantifier(ExistentialQuantifier &opr, Trace &trace,
                                                                         size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitQuoteQuantifier(QuoteQuantifier &opr, Trace &trace,
                                                                   size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitCountingQuantifier(CountingQuantifier &opr, Trace &trace,
                                                                      size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitEqualityOperator(EqualityOperator &opr, Trace &trace,
                                                                    size_t index) override;
};
