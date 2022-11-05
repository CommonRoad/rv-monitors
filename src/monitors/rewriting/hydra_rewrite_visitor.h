#pragma once

#include "monitors/expression_tree/first_order_operator.h"
#include "monitors/formula/formula.h"

/**
 * Applies rules to transform formula into conjunction of exclusive disjunctions.
 */
class HydraRewriteVisitor : public ExpressionVisitor {
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

    [[nodiscard]] std::shared_ptr<Expression> visitUniversalQuantifier(UniversalQuantifier &op, Trace &trace,
                                                                       size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitExistentialQuantifier(ExistentialQuantifier &op, Trace &trace,
                                                                         size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitQuoteQuantifier(QuoteQuantifier &op, Trace &trace,
                                                                   size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitCountingQuantifier(CountingQuantifier &op, Trace &trace,
                                                                      size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitEqualityOperator(EqualityOperator &op, Trace &trace,
                                                                    size_t index) override;

  private:
    /**
     * Swaps a first-order quantifier and a following unary temporal operator. If the child expression of the
     * first-order quantifier is not an unary temporal operator, a nullptr will be returned.
     *
     * @param oper First-order quantifier.
     * @param trace Trace.
     * @param index Index.
     * @return Expression with swapped quantifier and unary operator.
     */
    std::shared_ptr<Expression> swapQuantifierAndTempUnary(const std::shared_ptr<FirstOrderOperator> &oper,
                                                           Trace &trace, size_t index);

    /**
     * Removes quantified variables which are unused. If a quantifier has no variable anymore, the complete quantifier
     * will be removed. A shadowed variable is also be seen as a unused one.
     *
     * @param oper First-order quantifier.
     * @param left Child expression.
     * @return Expression with removed shadowed quantified variables.
     */
    static std::shared_ptr<Expression> removeUnusedVariables(const std::shared_ptr<FirstOrderOperator> &oper);

    /**
     * Searches an expression to check if a certain quantified variable is used.
     *
     * @param expr Expression.
     * @param varName Name of quantified variable.
     * @return Boolean indicating whether a quantified variable is used.
     */
    static bool hasUsedQuantifiedVariable(Expression &expr, std::string &varName);
};
