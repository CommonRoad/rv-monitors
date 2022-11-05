
// Generated from Formula.g4 by ANTLR 4.7.2

#pragma once

#include "FormulaParser.h"
#include "antlr4-runtime.h"

/**
 * This class defines an abstract visitor for a parse tree
 * produced by FormulaParser.
 */
class FormulaVisitor : public antlr4::tree::AbstractParseTreeVisitor {
  public:
    /**
     * Visit parse trees produced by FormulaParser.
     */
    virtual antlrcpp::Any visitFormula(FormulaParser::FormulaContext *context) = 0;

    virtual antlrcpp::Any visitEquivalence(FormulaParser::EquivalenceContext *context) = 0;

    virtual antlrcpp::Any visitXorExpr(FormulaParser::XorExprContext *context) = 0;

    virtual antlrcpp::Any visitImplication(FormulaParser::ImplicationContext *context) = 0;

    virtual antlrcpp::Any visitXor(FormulaParser::XorContext *context) = 0;

    virtual antlrcpp::Any visitOr(FormulaParser::OrContext *context) = 0;

    virtual antlrcpp::Any visitAnd(FormulaParser::AndContext *context) = 0;

    virtual antlrcpp::Any visitRelease(FormulaParser::ReleaseContext *context) = 0;

    virtual antlrcpp::Any visitUntil(FormulaParser::UntilContext *context) = 0;

    virtual antlrcpp::Any visitReleaseInterval(FormulaParser::ReleaseIntervalContext *context) = 0;

    virtual antlrcpp::Any visitUntilInterval(FormulaParser::UntilIntervalContext *context) = 0;

    virtual antlrcpp::Any visitSince(FormulaParser::SinceContext *context) = 0;

    virtual antlrcpp::Any visitSinceInterval(FormulaParser::SinceIntervalContext *context) = 0;

    virtual antlrcpp::Any visitEqualityOperator(FormulaParser::EqualityOperatorContext *context) = 0;

    virtual antlrcpp::Any visitUnaryExpr(FormulaParser::UnaryExprContext *context) = 0;

    virtual antlrcpp::Any visitGlobally(FormulaParser::GloballyContext *context) = 0;

    virtual antlrcpp::Any visitFuture(FormulaParser::FutureContext *context) = 0;

    virtual antlrcpp::Any visitOnce(FormulaParser::OnceContext *context) = 0;

    virtual antlrcpp::Any visitPreviously(FormulaParser::PreviouslyContext *context) = 0;

    virtual antlrcpp::Any visitNextWeak(FormulaParser::NextWeakContext *context) = 0;

    virtual antlrcpp::Any visitNextStrong(FormulaParser::NextStrongContext *context) = 0;

    virtual antlrcpp::Any visitGloballyInterval(FormulaParser::GloballyIntervalContext *context) = 0;

    virtual antlrcpp::Any visitFutureInterval(FormulaParser::FutureIntervalContext *context) = 0;

    virtual antlrcpp::Any visitOnceInterval(FormulaParser::OnceIntervalContext *context) = 0;

    virtual antlrcpp::Any visitPreviouslyInterval(FormulaParser::PreviouslyIntervalContext *context) = 0;

    virtual antlrcpp::Any visitNextWeakInterval(FormulaParser::NextWeakIntervalContext *context) = 0;

    virtual antlrcpp::Any visitNextStrongInterval(FormulaParser::NextStrongIntervalContext *context) = 0;

    virtual antlrcpp::Any visitHistorically(FormulaParser::HistoricallyContext *context) = 0;

    virtual antlrcpp::Any visitHistoricallyInterval(FormulaParser::HistoricallyIntervalContext *context) = 0;

    virtual antlrcpp::Any visitNot(FormulaParser::NotContext *context) = 0;

    virtual antlrcpp::Any visitNotExpr(FormulaParser::NotExprContext *context) = 0;

    virtual antlrcpp::Any visitAtomExpr(FormulaParser::AtomExprContext *context) = 0;

    virtual antlrcpp::Any visitPredicate(FormulaParser::PredicateContext *context) = 0;

    virtual antlrcpp::Any visitUniversal(FormulaParser::UniversalContext *context) = 0;

    virtual antlrcpp::Any visitExistential(FormulaParser::ExistentialContext *context) = 0;

    virtual antlrcpp::Any visitQuote(FormulaParser::QuoteContext *context) = 0;

    virtual antlrcpp::Any visitCounting(FormulaParser::CountingContext *context) = 0;

    virtual antlrcpp::Any visitFiniteInterval(FormulaParser::FiniteIntervalContext *context) = 0;

    virtual antlrcpp::Any visitInfiniteInterval(FormulaParser::InfiniteIntervalContext *context) = 0;

    virtual antlrcpp::Any visitFiniteQuoteInterval(FormulaParser::FiniteQuoteIntervalContext *context) = 0;

    virtual antlrcpp::Any visitDomainVariables(FormulaParser::DomainVariablesContext *context) = 0;

    virtual antlrcpp::Any visitBoolean(FormulaParser::BooleanContext *context) = 0;

    virtual antlrcpp::Any visitAtomic(FormulaParser::AtomicContext *context) = 0;

    virtual antlrcpp::Any visitBrackets(FormulaParser::BracketsContext *context) = 0;

    virtual antlrcpp::Any visitDigit(FormulaParser::DigitContext *context) = 0;

    virtual antlrcpp::Any visitQuoteDigit(FormulaParser::QuoteDigitContext *context) = 0;

    virtual antlrcpp::Any visitTrueBoolean(FormulaParser::TrueBooleanContext *context) = 0;

    virtual antlrcpp::Any visitFalseBoolean(FormulaParser::FalseBooleanContext *context) = 0;
};
