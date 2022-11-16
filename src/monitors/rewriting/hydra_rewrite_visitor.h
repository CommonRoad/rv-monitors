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

    [[nodiscard]] std::shared_ptr<Expression> visitDerivingOperator(DerivingOperator &opr, Trace &trace,
                                                                    size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression> visitResolvingOperator(ResolvingOperator &opr, Trace &trace,
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
