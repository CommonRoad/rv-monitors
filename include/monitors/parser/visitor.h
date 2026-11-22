#pragma once

#include <monitors/parser/generated/FormulaGrammarParser/FormulaBaseVisitor.h>

/**
 * Visitor for parsing.
 */
class Visitor : public FormulaBaseVisitor {
  public:
    /**
     * Returns type of an expression.
     *
     * @param expr Parsed expression.
     * @return The expression's type.
     */
    static std::shared_ptr<Expression> getType(const std::any &expr);

    /**
     * Visits a formula's expression.
     *
     * @param ctx Context.
     * @return Formula object with expression built by visitor.
     */
    std::any visitFormula(FormulaParser::FormulaContext *ctx) override;

    /**
     * Recursively visit the children and build equivalence with them.
     *
     * @param ctx Context.
     * @return Equivalence.
     */
    std::any visitEquivalence(FormulaParser::EquivalenceContext *ctx) override;

    /**
     * Recursively visits the children and builds an Implication with them.
     *
     * @param ctx Context.
     * @return Implication.
     */
    std::any visitImplication(FormulaParser::ImplicationContext *ctx) override;

    /**
     * Visit the expression encoded by xorExpr.
     *
     * @param ctx Context.
     * @return Result of visiting expression behind xorExpr.
     */
    std::any visitXorExpr(FormulaParser::XorExprContext *ctx) override;

    /**
     * Builds an Xor by recursively visiting its children or goes deeper into the expression tree if expression is not
     * an Xor.
     *
     * @param ctx Context.
     * @return Xor if actual xorExpr visited, result of visitChildren() otherwise.
     */
    std::any visitXor(FormulaParser::XorContext *ctx) override;

    /**
     * Builds an Or by recursively visiting its children or goes deeper into the expression tree if expression is not an
     * Or.
     *
     * @param ctx Context.
     * @return Or if actual orExpr visited, result of visitChildren() otherwise.
     */
    std::any visitOr(FormulaParser::OrContext *ctx) override;

    /**
     * Builds an And by recursively visiting its children or goes deeper into the expression tree if expression is not
     * an And.
     *
     * @param ctx Context.
     * @return And if actual orExpr visited, result of visitChildren() otherwise.
     */
    std::any visitAnd(FormulaParser::AndContext *ctx) override;

    /**
     * Recursively visits the children and builds a Release with them.
     *
     * @param ctx Context.
     * @return Release.
     */
    std::any visitRelease(FormulaParser::ReleaseContext *ctx) override;

    /**
     * Recursively visits the children and builds an Until with them.
     *
     * @param ctx Context.
     * @return Until.
     */
    std::any visitUntil(FormulaParser::UntilContext *ctx) override;

    /**
     * Recursively visits the children and builds an interval-based Release with them.
     *
     * @param ctx Context.
     * @return Release.
     */
    std::any visitReleaseInterval(FormulaParser::ReleaseIntervalContext *ctx) override;

    /**
     * Recursively visits the children and builds an interval-based Until with them.
     *
     * @param ctx Context.
     * @return Until.
     */
    std::any visitUntilInterval(FormulaParser::UntilIntervalContext *ctx) override;

    /**
     * Recursively visits the children and builds a Since with them.
     *
     * @param ctx Context.
     * @return Until.
     */
    std::any visitSince(FormulaParser::SinceContext *ctx) override;

    /**
     * Recursively visits the children and builds an interval-based Since with them.
     *
     * @param ctx Context.
     * @return Until.
     */
    std::any visitSinceInterval(FormulaParser::SinceIntervalContext *ctx) override;

    /**
     * Visits a unary expression.
     *
     * @param ctx Context
     * @return Result of visiting unary.
     */
    std::any visitUnaryExpr(FormulaParser::UnaryExprContext *ctx) override;

    /**
     * Recursively visits the child and builds a Globally with it.
     *
     * @param ctx Context.
     * @return Globally.
     */
    std::any visitGlobally(FormulaParser::GloballyContext *ctx) override;

    /**
     * Recursively visits the child and builds a Historically with it.
     *
     * @param ctx Context.
     * @return Historically.
     */
    std::any visitHistorically(FormulaParser::HistoricallyContext *ctx) override;

    /**
     * Recursively visits the child and builds a Future with it.
     *
     * @param ctx Context.
     * @return Future.
     */
    std::any visitFuture(FormulaParser::FutureContext *ctx) override;

    /**
     * Recursively visits the child and builds a Once with it.
     *
     * @param ctx Context.
     * @return Once.
     */
    std::any visitOnce(FormulaParser::OnceContext *ctx) override;

    /**
     * Recursively visits the child and builds a Previously with it.
     *
     * @param ctx Context.
     * @return Previously.
     */
    std::any visitPreviously(FormulaParser::PreviouslyContext *ctx) override;

    /**
     * Recursively visits the child and builds a NextWeak with it.
     *
     * @param ctx Context.
     * @return NextWeak.
     */
    std::any visitNextWeak(FormulaParser::NextWeakContext *ctx) override;
    /**
     * Recursively visits the child and builds a NextStrong with it.
     *
     * @param ctx Context.
     * @return NextStrong.
     */
    std::any visitNextStrong(FormulaParser::NextStrongContext *ctx) override;

    /**
     * Recursively visits the child and builds an interval-based Globally with it.
     *
     * @param ctx Context.
     * @return Globally.
     */
    std::any visitGloballyInterval(FormulaParser::GloballyIntervalContext *ctx) override;

    /**
     * Recursively visits the child and builds an interval-based Historically with it.
     *
     * @param ctx Context.
     * @return Historically.
     */
    std::any visitHistoricallyInterval(FormulaParser::HistoricallyIntervalContext *ctx) override;

    /**
     * Recursively visits the child and builds an interval-based Future with it.
     *
     * @param ctx Context.
     * @return Future.
     */
    std::any visitFutureInterval(FormulaParser::FutureIntervalContext *ctx) override;

    /**
     * Recursively visits the child and builds an interval-based Once with it.
     *
     * @param ctx Context.
     * @return Once.
     */
    std::any visitOnceInterval(FormulaParser::OnceIntervalContext *ctx) override;

    /**
     * Recursively visits the child and builds an interval-based Previously with it.
     *
     * @param ctx Context.
     * @return Previously.
     */
    std::any visitPreviouslyInterval(FormulaParser::PreviouslyIntervalContext *ctx) override;

    /**
     * Recursively visits the child and builds an interval-based NextWeak with it.
     *
     * @param ctx Context.
     * @return NextWeak.
     */
    std::any visitNextWeakInterval(FormulaParser::NextWeakIntervalContext *ctx) override;

    /**
     * Recursively visits the child and builds an interval-based NextStrong with it.
     *
     * @param ctx Context.
     * @return NextStrong.
     */
    std::any visitNextStrongInterval(FormulaParser::NextStrongIntervalContext *ctx) override;

    /**
     * Recursively visits the child and builds a Not with it.
     *
     * @param ctx Context.
     * @return Not.
     */
    std::any visitNot(FormulaParser::NotContext *ctx) override;

    /**
     * Visits the atom in atomExpr.
     *
     * @param ctx Context.
     * @return Result if visiting the atom in atomExpr.
     */
    std::any visitAtomExpr(FormulaParser::AtomExprContext *ctx) override;

    /**
     * Visits a constant and builds a Constant with it.
     *
     * @param ctx Content.
     * @return Constant according to given value.
     */
    std::any visitBoolean(FormulaParser::BooleanContext *ctx) override;

    /**
     * Visits a true constant and builds a Constant with it.
     *
     * @param ctx Content.
     * @return True constant.
     */
    std::any visitTrueBoolean(FormulaParser::TrueBooleanContext *context) override;

    /**
     * Visits a false constant and builds a Constant with it.
     *
     * @param ctx Content.
     * @return False constant.
     */
    std::any visitFalseBoolean(FormulaParser::FalseBooleanContext *context) override;

    /**
     * Ignores the brackets and visits the content inside them.
     *
     * @param ctx Context.
     * @return Result of visiting content inside the bracktes.
     */
    std::any visitBrackets(FormulaParser::BracketsContext *ctx) override;

    /**
     * Builds an Atomic from a string.
     *
     * @param ctx Context.
     * @return Atomic.
     */
    std::any visitAtomic(FormulaParser::AtomicContext *ctx) override;

    /**
     * Builds an finite interval from a string.
     *
     * @param ctx Context.
     * @return finite interval.
     */
    std::any visitFiniteInterval(FormulaParser::FiniteIntervalContext *ctx) override;

    /**
     * Builds an infinite interval from a string.
     *
     * @param ctx Context.
     * @return infinite interval.
     */
    std::any visitInfiniteInterval(FormulaParser::InfiniteIntervalContext *ctx) override;

    /**
     * Builds an digit from a string.
     *
     * @param ctx Context.
     * @return Atomic.
     */
    std::any visitDigit(FormulaParser::DigitContext *ctx) override;

    /**
     * Builds a predicate from a string.
     *
     * @param ctx Context.
     * @return Predicate.
     */
    std::any visitPredicate(FormulaParser::PredicateContext *ctx) override;

    /**
     * Helper function to extract function parameters from a corresponding antrl4 context.
     *
     * @param ctx Context.
     * @return Tuple consisting list of all parameter values/names, list of all variables (no constants),
     * concatenated name of parameters separated by underscore
     */
    static std::tuple<std::vector<std::string>, std::vector<std::string>, std::string>
    extractFunctionParameters(FormulaParser::UnaryContext *ctx);

    /**
     * Builds a universal quantifier from a string.
     *
     * @param ctx Context.
     * @return Universal quantifier.
     */
    std::any visitUniversal(FormulaParser::UniversalContext *ctx) override;

    /**
     * Builds a existential quantifier from a string.
     *
     * @param ctx Context.
     * @return Existential quantifier.
     */
    std::any visitExistential(FormulaParser::ExistentialContext *ctx) override;

    /**
     * Builds an quote quantifier from a string.
     *
     * @param ctx Context.
     * @return Quote quantifier.
     */
    std::any visitQuote(FormulaParser::QuoteContext *ctx) override;

    /**
     * Builds a counting quantifier from a string.
     *
     * @param ctx Context.
     * @return Counting quantifier.
     */
    std::any visitCounting(FormulaParser::CountingContext *ctx) override;

    /**
     * Builds a finite quote quantifier from a string.
     *
     * @param ctx Context.
     * @return Finite quote quantifier.
     */
    std::any visitFiniteQuoteInterval(FormulaParser::FiniteQuoteIntervalContext *ctx) override;

    /**
     * Builds a domain variable from a string.
     *
     * @param ctx Context.
     * @return Domain variable.
     */
    std::any visitDomainVariables(FormulaParser::DomainVariablesContext *ctx) override;

    /**
     * Builds a fixed domain from a string.
     *
     * @param ctx Context.
     * @return Domain.
     */
    std::any visitStringDomain(FormulaParser::StringDomainContext *ctx) override;

    /**
     * Builds a dynamic function domain from a string.
     *
     * @param ctx Context.
     * @return Domain.
     */
    std::any visitFunctionDomain(FormulaParser::FunctionDomainContext *ctx) override;

    /**
     * Builds a quote quantifier digit from a string.
     *
     * @param ctx Context.
     * @return Digit.
     */
    std::any visitQuoteDigit(FormulaParser::QuoteDigitContext *ctx) override;
};
