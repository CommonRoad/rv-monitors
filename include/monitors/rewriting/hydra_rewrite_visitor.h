#pragma once

#include "monitors/collections/domain.h"
#include "monitors/expression_tree/first_order_operator.h"
#include "monitors/formula/formula.h"

/**
 * Applies rules to transform formula into conjunction of exclusive disjunctions.
 */
class HydraRewriteVisitor : public ExpressionVisitor {
  public:
    HydraRewriteVisitor() = default;

    /**
     * Constructor.
     *
     * @param domains Domains known when the formula is rewritten. Quantifiers over domains which are not contained
     * here, or which are dynamic or time-dependent, are never swapped with temporal operators.
     */
    explicit HydraRewriteVisitor(std::vector<std::shared_ptr<DomainStorage>> domains);

    void rewrite(Formula &formula);

    // void visitExpression(Expression &expr, const std::shared_ptr<Trace> &trace, size_t index);

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
    [[nodiscard]] std::shared_ptr<Expression> visitPredicate(Predicate &opr, const std::shared_ptr<Trace> &trace,
                                                             size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression>
    visitUniversalQuantifier(UniversalQuantifier &opr, const std::shared_ptr<Trace> &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression>
    visitDerivingOperator(DerivingOperator &opr, const std::shared_ptr<Trace> &trace, size_t index) override;
    [[nodiscard]] std::shared_ptr<Expression>
    visitExistentialQuantifier(ExistentialQuantifier &opr, const std::shared_ptr<Trace> &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression>
    visitResolvingOperator(ResolvingOperator &opr, const std::shared_ptr<Trace> &trace, size_t index) override;
    [[nodiscard]] std::shared_ptr<Expression>
    visitQuoteQuantifier(QuoteQuantifier &opr, const std::shared_ptr<Trace> &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Expression>
    visitCountingQuantifier(CountingQuantifier &opr, const std::shared_ptr<Trace> &trace, size_t index) override;

  private:
    std::vector<std::shared_ptr<DomainStorage>> domains; //**< Domains known at rewriting time. */

    /**
     * Returns the domain of a variable if it is a constant domain known at rewriting time.
     *
     * @param var Variable.
     * @return Domain, or nullptr if the domain is unknown, dynamic, or a function domain.
     */
    [[nodiscard]] const DomainStorage *findConstantDomain(const Variable &var) const;

    /**
     * Checks whether all variables of a quantifier range over constant domains known at rewriting time. Domains which
     * are unknown, dynamic (DomainStorage::isConstDomain() is false), or function domains with parameters depending on
     * other variables are not constant.
     *
     * @param oper First-order quantifier.
     * @return Boolean indicating whether all domains are constant.
     */
    [[nodiscard]] bool hasConstantDomains(const FirstOrderOperator &oper) const;

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
                                                           const std::shared_ptr<Trace> &trace, size_t index);

    /**
     * Removes quantified variables which are unused. If a quantifier has no variable anymore, the complete quantifier
     * will be removed. A shadowed variable is also be seen as a unused one.
     *
     * @param oper First-order quantifier.
     * @param keepLastVariable Boolean indicating whether at least one variable should be kept.
     * @return Expression with removed shadowed quantified variables.
     */
    std::shared_ptr<Expression> removeUnusedVariables(const std::shared_ptr<FirstOrderOperator> &oper,
                                                      bool keepLastVariable = false);

    /**
     * Searches an expression to check if a certain quantified variable is used.
     *
     * @param expr Expression.
     * @param varName Name of quantified variable.
     * @return Boolean indicating whether a quantified variable is used.
     */
    static bool hasUsedQuantifiedVariable(Expression &expr, std::string &varName);
};
