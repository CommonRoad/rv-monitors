
// Generated from Formula.g4 by ANTLR 4.7.2

#pragma once

#include "FormulaParser.h"
#include "antlr4-runtime.h"

/**
 * This interface defines an abstract listener for a parse tree produced by FormulaParser.
 */
class FormulaListener : public antlr4::tree::ParseTreeListener {
  public:
    virtual void enterFormula(FormulaParser::FormulaContext *ctx) = 0;
    virtual void exitFormula(FormulaParser::FormulaContext *ctx) = 0;

    virtual void enterEquivalence(FormulaParser::EquivalenceContext *ctx) = 0;
    virtual void exitEquivalence(FormulaParser::EquivalenceContext *ctx) = 0;

    virtual void enterXorExpr(FormulaParser::XorExprContext *ctx) = 0;
    virtual void exitXorExpr(FormulaParser::XorExprContext *ctx) = 0;

    virtual void enterImplication(FormulaParser::ImplicationContext *ctx) = 0;
    virtual void exitImplication(FormulaParser::ImplicationContext *ctx) = 0;

    virtual void enterXor(FormulaParser::XorContext *ctx) = 0;
    virtual void exitXor(FormulaParser::XorContext *ctx) = 0;

    virtual void enterOr(FormulaParser::OrContext *ctx) = 0;
    virtual void exitOr(FormulaParser::OrContext *ctx) = 0;

    virtual void enterAnd(FormulaParser::AndContext *ctx) = 0;
    virtual void exitAnd(FormulaParser::AndContext *ctx) = 0;

    virtual void enterRelease(FormulaParser::ReleaseContext *ctx) = 0;
    virtual void exitRelease(FormulaParser::ReleaseContext *ctx) = 0;

    virtual void enterUntil(FormulaParser::UntilContext *ctx) = 0;
    virtual void exitUntil(FormulaParser::UntilContext *ctx) = 0;

    virtual void enterReleaseInterval(FormulaParser::ReleaseIntervalContext *ctx) = 0;
    virtual void exitReleaseInterval(FormulaParser::ReleaseIntervalContext *ctx) = 0;

    virtual void enterUntilInterval(FormulaParser::UntilIntervalContext *ctx) = 0;
    virtual void exitUntilInterval(FormulaParser::UntilIntervalContext *ctx) = 0;

    virtual void enterSince(FormulaParser::SinceContext *ctx) = 0;
    virtual void exitSince(FormulaParser::SinceContext *ctx) = 0;

    virtual void enterSinceInterval(FormulaParser::SinceIntervalContext *ctx) = 0;
    virtual void exitSinceInterval(FormulaParser::SinceIntervalContext *ctx) = 0;

    virtual void enterUnaryExpr(FormulaParser::UnaryExprContext *ctx) = 0;
    virtual void exitUnaryExpr(FormulaParser::UnaryExprContext *ctx) = 0;

    virtual void enterGlobally(FormulaParser::GloballyContext *ctx) = 0;
    virtual void exitGlobally(FormulaParser::GloballyContext *ctx) = 0;

    virtual void enterFuture(FormulaParser::FutureContext *ctx) = 0;
    virtual void exitFuture(FormulaParser::FutureContext *ctx) = 0;

    virtual void enterOnce(FormulaParser::OnceContext *ctx) = 0;
    virtual void exitOnce(FormulaParser::OnceContext *ctx) = 0;

    virtual void enterPreviously(FormulaParser::PreviouslyContext *ctx) = 0;
    virtual void exitPreviously(FormulaParser::PreviouslyContext *ctx) = 0;

    virtual void enterNextWeak(FormulaParser::NextWeakContext *ctx) = 0;
    virtual void exitNextWeak(FormulaParser::NextWeakContext *ctx) = 0;

    virtual void enterNextStrong(FormulaParser::NextStrongContext *ctx) = 0;
    virtual void exitNextStrong(FormulaParser::NextStrongContext *ctx) = 0;

    virtual void enterGloballyInterval(FormulaParser::GloballyIntervalContext *ctx) = 0;
    virtual void exitGloballyInterval(FormulaParser::GloballyIntervalContext *ctx) = 0;

    virtual void enterFutureInterval(FormulaParser::FutureIntervalContext *ctx) = 0;
    virtual void exitFutureInterval(FormulaParser::FutureIntervalContext *ctx) = 0;

    virtual void enterOnceInterval(FormulaParser::OnceIntervalContext *ctx) = 0;
    virtual void exitOnceInterval(FormulaParser::OnceIntervalContext *ctx) = 0;

    virtual void enterPreviouslyInterval(FormulaParser::PreviouslyIntervalContext *ctx) = 0;
    virtual void exitPreviouslyInterval(FormulaParser::PreviouslyIntervalContext *ctx) = 0;

    virtual void enterNextWeakInterval(FormulaParser::NextWeakIntervalContext *ctx) = 0;
    virtual void exitNextWeakInterval(FormulaParser::NextWeakIntervalContext *ctx) = 0;

    virtual void enterNextStrongInterval(FormulaParser::NextStrongIntervalContext *ctx) = 0;
    virtual void exitNextStrongInterval(FormulaParser::NextStrongIntervalContext *ctx) = 0;

    virtual void enterHistorically(FormulaParser::HistoricallyContext *ctx) = 0;
    virtual void exitHistorically(FormulaParser::HistoricallyContext *ctx) = 0;

    virtual void enterHistoricallyInterval(FormulaParser::HistoricallyIntervalContext *ctx) = 0;
    virtual void exitHistoricallyInterval(FormulaParser::HistoricallyIntervalContext *ctx) = 0;

    virtual void enterNot(FormulaParser::NotContext *ctx) = 0;
    virtual void exitNot(FormulaParser::NotContext *ctx) = 0;

    virtual void enterNotExpr(FormulaParser::NotExprContext *ctx) = 0;
    virtual void exitNotExpr(FormulaParser::NotExprContext *ctx) = 0;

    virtual void enterAtomExpr(FormulaParser::AtomExprContext *ctx) = 0;
    virtual void exitAtomExpr(FormulaParser::AtomExprContext *ctx) = 0;

    virtual void enterFiniteInterval(FormulaParser::FiniteIntervalContext *ctx) = 0;
    virtual void exitFiniteInterval(FormulaParser::FiniteIntervalContext *ctx) = 0;

    virtual void enterInfiniteInterval(FormulaParser::InfiniteIntervalContext *ctx) = 0;
    virtual void exitInfiniteInterval(FormulaParser::InfiniteIntervalContext *ctx) = 0;

    virtual void enterBoolean(FormulaParser::BooleanContext *ctx) = 0;
    virtual void exitBoolean(FormulaParser::BooleanContext *ctx) = 0;

    virtual void enterAtomic(FormulaParser::AtomicContext *ctx) = 0;
    virtual void exitAtomic(FormulaParser::AtomicContext *ctx) = 0;

    virtual void enterBrackets(FormulaParser::BracketsContext *ctx) = 0;
    virtual void exitBrackets(FormulaParser::BracketsContext *ctx) = 0;

    virtual void enterDigit(FormulaParser::DigitContext *ctx) = 0;
    virtual void exitDigit(FormulaParser::DigitContext *ctx) = 0;
};
