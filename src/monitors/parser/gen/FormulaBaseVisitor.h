
// Generated from Formula.g4 by ANTLR 4.7.2

#pragma once

#include "FormulaVisitor.h"
#include "antlr4-runtime.h"

/**
 * This class provides an empty implementation of FormulaVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class FormulaBaseVisitor : public FormulaVisitor {
  public:
    virtual antlrcpp::Any visitFormula(FormulaParser::FormulaContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitEquivalence(FormulaParser::EquivalenceContext *ctx) override {
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitXorExpr(FormulaParser::XorExprContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitImplication(FormulaParser::ImplicationContext *ctx) override {
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitXor(FormulaParser::XorContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitOr(FormulaParser::OrContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitAnd(FormulaParser::AndContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitRelease(FormulaParser::ReleaseContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitUntil(FormulaParser::UntilContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitReleaseInterval(FormulaParser::ReleaseIntervalContext *ctx) override {
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitUntilInterval(FormulaParser::UntilIntervalContext *ctx) override {
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitSince(FormulaParser::SinceContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitSinceInterval(FormulaParser::SinceIntervalContext *ctx) override {
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitEqualityOperator(FormulaParser::EqualityOperatorContext *ctx) override {
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitUnaryExpr(FormulaParser::UnaryExprContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitGlobally(FormulaParser::GloballyContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitFuture(FormulaParser::FutureContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitOnce(FormulaParser::OnceContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitPreviously(FormulaParser::PreviouslyContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitNextWeak(FormulaParser::NextWeakContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitNextStrong(FormulaParser::NextStrongContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitGloballyInterval(FormulaParser::GloballyIntervalContext *ctx) override {
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitFutureInterval(FormulaParser::FutureIntervalContext *ctx) override {
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitOnceInterval(FormulaParser::OnceIntervalContext *ctx) override {
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitPreviouslyInterval(FormulaParser::PreviouslyIntervalContext *ctx) override {
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitNextWeakInterval(FormulaParser::NextWeakIntervalContext *ctx) override {
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitNextStrongInterval(FormulaParser::NextStrongIntervalContext *ctx) override {
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitHistorically(FormulaParser::HistoricallyContext *ctx) override {
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitHistoricallyInterval(FormulaParser::HistoricallyIntervalContext *ctx) override {
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitNot(FormulaParser::NotContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitNotExpr(FormulaParser::NotExprContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitAtomExpr(FormulaParser::AtomExprContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitPredicate(FormulaParser::PredicateContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitUniversal(FormulaParser::UniversalContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitExistential(FormulaParser::ExistentialContext *ctx) override {
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitQuote(FormulaParser::QuoteContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitCounting(FormulaParser::CountingContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitFiniteInterval(FormulaParser::FiniteIntervalContext *ctx) override {
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitInfiniteInterval(FormulaParser::InfiniteIntervalContext *ctx) override {
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitFiniteQuoteInterval(FormulaParser::FiniteQuoteIntervalContext *ctx) override {
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitDomainVariables(FormulaParser::DomainVariablesContext *ctx) override {
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitBoolean(FormulaParser::BooleanContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitAtomic(FormulaParser::AtomicContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitBrackets(FormulaParser::BracketsContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitDigit(FormulaParser::DigitContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitQuoteDigit(FormulaParser::QuoteDigitContext *ctx) override { return visitChildren(ctx); }

    virtual antlrcpp::Any visitTrueBoolean(FormulaParser::TrueBooleanContext *ctx) override {
        return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitFalseBoolean(FormulaParser::FalseBooleanContext *ctx) override {
        return visitChildren(ctx);
    }
};
