
// Generated from Formula.g4 by ANTLR 4.9.3

#include "FormulaVisitor.h"

#include "FormulaParser.h"

using namespace antlrcpp;
using namespace antlr4;

FormulaParser::FormulaParser(TokenStream *input) : Parser(input) {
    _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

FormulaParser::~FormulaParser() { delete _interpreter; }

std::string FormulaParser::getGrammarFileName() const { return "Formula.g4"; }

const std::vector<std::string> &FormulaParser::getRuleNames() const { return _ruleNames; }

dfa::Vocabulary &FormulaParser::getVocabulary() const { return _vocabulary; }

//----------------- FormulaContext ------------------------------------------------------------------

FormulaParser::FormulaContext::FormulaContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

FormulaParser::ExprContext *FormulaParser::FormulaContext::expr() {
    return getRuleContext<FormulaParser::ExprContext>(0);
}

size_t FormulaParser::FormulaContext::getRuleIndex() const { return FormulaParser::RuleFormula; }

antlrcpp::Any FormulaParser::FormulaContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitFormula(this);
    else
        return visitor->visitChildren(this);
}

FormulaParser::FormulaContext *FormulaParser::formula() {
    FormulaContext *_localctx = _tracker.createInstance<FormulaContext>(_ctx, getState());
    enterRule(_localctx, 0, FormulaParser::RuleFormula);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        enterOuterAlt(_localctx, 1);
        setState(22);
        expr(0);

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- ExprContext ------------------------------------------------------------------

FormulaParser::ExprContext::ExprContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

size_t FormulaParser::ExprContext::getRuleIndex() const { return FormulaParser::RuleExpr; }

void FormulaParser::ExprContext::copyFrom(ExprContext *ctx) { ParserRuleContext::copyFrom(ctx); }

//----------------- EquivalenceContext ------------------------------------------------------------------

std::vector<FormulaParser::ExprContext *> FormulaParser::EquivalenceContext::expr() {
    return getRuleContexts<FormulaParser::ExprContext>();
}

FormulaParser::ExprContext *FormulaParser::EquivalenceContext::expr(size_t i) {
    return getRuleContext<FormulaParser::ExprContext>(i);
}

tree::TerminalNode *FormulaParser::EquivalenceContext::EQ() { return getToken(FormulaParser::EQ, 0); }

FormulaParser::EquivalenceContext::EquivalenceContext(ExprContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::EquivalenceContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitEquivalence(this);
    else
        return visitor->visitChildren(this);
}
//----------------- XorExprContext ------------------------------------------------------------------

FormulaParser::Xor_exprContext *FormulaParser::XorExprContext::xor_expr() {
    return getRuleContext<FormulaParser::Xor_exprContext>(0);
}

FormulaParser::XorExprContext::XorExprContext(ExprContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::XorExprContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitXorExpr(this);
    else
        return visitor->visitChildren(this);
}
//----------------- ImplicationContext ------------------------------------------------------------------

std::vector<FormulaParser::ExprContext *> FormulaParser::ImplicationContext::expr() {
    return getRuleContexts<FormulaParser::ExprContext>();
}

FormulaParser::ExprContext *FormulaParser::ImplicationContext::expr(size_t i) {
    return getRuleContext<FormulaParser::ExprContext>(i);
}

tree::TerminalNode *FormulaParser::ImplicationContext::IMPL() { return getToken(FormulaParser::IMPL, 0); }

FormulaParser::ImplicationContext::ImplicationContext(ExprContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::ImplicationContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitImplication(this);
    else
        return visitor->visitChildren(this);
}

FormulaParser::ExprContext *FormulaParser::expr() { return expr(0); }

FormulaParser::ExprContext *FormulaParser::expr(int precedence) {
    ParserRuleContext *parentContext = _ctx;
    size_t parentState = getState();
    FormulaParser::ExprContext *_localctx = _tracker.createInstance<ExprContext>(_ctx, parentState);
    FormulaParser::ExprContext *previousContext = _localctx;
    (void)previousContext; // Silence compiler, in case the context is not used by generated code.
    size_t startState = 2;
    enterRecursionRule(_localctx, 2, FormulaParser::RuleExpr, precedence);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        unrollRecursionContexts(parentContext);
    });
    try {
        size_t alt;
        enterOuterAlt(_localctx, 1);
        _localctx = _tracker.createInstance<XorExprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(25);
        xor_expr();
        _ctx->stop = _input->LT(-1);
        setState(35);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
            if (alt == 1) {
                if (!_parseListeners.empty())
                    triggerExitRuleEvent();
                previousContext = _localctx;
                setState(33);
                _errHandler->sync(this);
                switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx)) {
                case 1: {
                    auto newContext = _tracker.createInstance<ImplicationContext>(
                        _tracker.createInstance<ExprContext>(parentContext, parentState));
                    _localctx = newContext;
                    newContext->left = previousContext;
                    pushNewRecursionContext(newContext, startState, RuleExpr);
                    setState(27);

                    if (!(precpred(_ctx, 3)))
                        throw FailedPredicateException(this, "precpred(_ctx, 3)");
                    setState(28);
                    antlrcpp::downCast<ImplicationContext *>(_localctx)->op = match(FormulaParser::IMPL);
                    setState(29);
                    antlrcpp::downCast<ImplicationContext *>(_localctx)->right = expr(3);
                    break;
                }

                case 2: {
                    auto newContext = _tracker.createInstance<EquivalenceContext>(
                        _tracker.createInstance<ExprContext>(parentContext, parentState));
                    _localctx = newContext;
                    newContext->left = previousContext;
                    pushNewRecursionContext(newContext, startState, RuleExpr);
                    setState(30);

                    if (!(precpred(_ctx, 2)))
                        throw FailedPredicateException(this, "precpred(_ctx, 2)");
                    setState(31);
                    antlrcpp::downCast<EquivalenceContext *>(_localctx)->op = match(FormulaParser::EQ);
                    setState(32);
                    antlrcpp::downCast<EquivalenceContext *>(_localctx)->right = expr(2);
                    break;
                }

                default:
                    break;
                }
            }
            setState(37);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx);
        }
    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }
    return _localctx;
}

//----------------- Xor_exprContext ------------------------------------------------------------------

FormulaParser::Xor_exprContext::Xor_exprContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

size_t FormulaParser::Xor_exprContext::getRuleIndex() const { return FormulaParser::RuleXor_expr; }

void FormulaParser::Xor_exprContext::copyFrom(Xor_exprContext *ctx) { ParserRuleContext::copyFrom(ctx); }

//----------------- XorContext ------------------------------------------------------------------

std::vector<FormulaParser::Or_exprContext *> FormulaParser::XorContext::or_expr() {
    return getRuleContexts<FormulaParser::Or_exprContext>();
}

FormulaParser::Or_exprContext *FormulaParser::XorContext::or_expr(size_t i) {
    return getRuleContext<FormulaParser::Or_exprContext>(i);
}

std::vector<tree::TerminalNode *> FormulaParser::XorContext::XOR() { return getTokens(FormulaParser::XOR); }

tree::TerminalNode *FormulaParser::XorContext::XOR(size_t i) { return getToken(FormulaParser::XOR, i); }

FormulaParser::XorContext::XorContext(Xor_exprContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::XorContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitXor(this);
    else
        return visitor->visitChildren(this);
}
FormulaParser::Xor_exprContext *FormulaParser::xor_expr() {
    Xor_exprContext *_localctx = _tracker.createInstance<Xor_exprContext>(_ctx, getState());
    enterRule(_localctx, 4, FormulaParser::RuleXor_expr);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        size_t alt;
        _localctx = _tracker.createInstance<FormulaParser::XorContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(38);
        or_expr();
        setState(43);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
            if (alt == 1) {
                setState(39);
                antlrcpp::downCast<XorContext *>(_localctx)->op = match(FormulaParser::XOR);
                setState(40);
                or_expr();
            }
            setState(45);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx);
        }

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- Or_exprContext ------------------------------------------------------------------

FormulaParser::Or_exprContext::Or_exprContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

size_t FormulaParser::Or_exprContext::getRuleIndex() const { return FormulaParser::RuleOr_expr; }

void FormulaParser::Or_exprContext::copyFrom(Or_exprContext *ctx) { ParserRuleContext::copyFrom(ctx); }

//----------------- OrContext ------------------------------------------------------------------

std::vector<FormulaParser::And_exprContext *> FormulaParser::OrContext::and_expr() {
    return getRuleContexts<FormulaParser::And_exprContext>();
}

FormulaParser::And_exprContext *FormulaParser::OrContext::and_expr(size_t i) {
    return getRuleContext<FormulaParser::And_exprContext>(i);
}

std::vector<tree::TerminalNode *> FormulaParser::OrContext::OR() { return getTokens(FormulaParser::OR); }

tree::TerminalNode *FormulaParser::OrContext::OR(size_t i) { return getToken(FormulaParser::OR, i); }

FormulaParser::OrContext::OrContext(Or_exprContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::OrContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitOr(this);
    else
        return visitor->visitChildren(this);
}
FormulaParser::Or_exprContext *FormulaParser::or_expr() {
    Or_exprContext *_localctx = _tracker.createInstance<Or_exprContext>(_ctx, getState());
    enterRule(_localctx, 6, FormulaParser::RuleOr_expr);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        size_t alt;
        _localctx = _tracker.createInstance<FormulaParser::OrContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(46);
        and_expr();
        setState(51);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
            if (alt == 1) {
                setState(47);
                antlrcpp::downCast<OrContext *>(_localctx)->op = match(FormulaParser::OR);
                setState(48);
                and_expr();
            }
            setState(53);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx);
        }

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- And_exprContext ------------------------------------------------------------------

FormulaParser::And_exprContext::And_exprContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

size_t FormulaParser::And_exprContext::getRuleIndex() const { return FormulaParser::RuleAnd_expr; }

void FormulaParser::And_exprContext::copyFrom(And_exprContext *ctx) { ParserRuleContext::copyFrom(ctx); }

//----------------- AndContext ------------------------------------------------------------------

std::vector<FormulaParser::Release_until_exprContext *> FormulaParser::AndContext::release_until_expr() {
    return getRuleContexts<FormulaParser::Release_until_exprContext>();
}

FormulaParser::Release_until_exprContext *FormulaParser::AndContext::release_until_expr(size_t i) {
    return getRuleContext<FormulaParser::Release_until_exprContext>(i);
}

std::vector<tree::TerminalNode *> FormulaParser::AndContext::AND() { return getTokens(FormulaParser::AND); }

tree::TerminalNode *FormulaParser::AndContext::AND(size_t i) { return getToken(FormulaParser::AND, i); }

FormulaParser::AndContext::AndContext(And_exprContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::AndContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitAnd(this);
    else
        return visitor->visitChildren(this);
}
FormulaParser::And_exprContext *FormulaParser::and_expr() {
    And_exprContext *_localctx = _tracker.createInstance<And_exprContext>(_ctx, getState());
    enterRule(_localctx, 8, FormulaParser::RuleAnd_expr);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        size_t alt;
        _localctx = _tracker.createInstance<FormulaParser::AndContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(54);
        release_until_expr();
        setState(59);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx);
        while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
            if (alt == 1) {
                setState(55);
                antlrcpp::downCast<AndContext *>(_localctx)->op = match(FormulaParser::AND);
                setState(56);
                release_until_expr();
            }
            setState(61);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx);
        }

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- Release_until_exprContext ------------------------------------------------------------------

FormulaParser::Release_until_exprContext::Release_until_exprContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

size_t FormulaParser::Release_until_exprContext::getRuleIndex() const { return FormulaParser::RuleRelease_until_expr; }

void FormulaParser::Release_until_exprContext::copyFrom(Release_until_exprContext *ctx) {
    ParserRuleContext::copyFrom(ctx);
}

//----------------- SinceIntervalContext ------------------------------------------------------------------

std::vector<FormulaParser::UnaryContext *> FormulaParser::SinceIntervalContext::unary() {
    return getRuleContexts<FormulaParser::UnaryContext>();
}

FormulaParser::UnaryContext *FormulaParser::SinceIntervalContext::unary(size_t i) {
    return getRuleContext<FormulaParser::UnaryContext>(i);
}

tree::TerminalNode *FormulaParser::SinceIntervalContext::SINCE() { return getToken(FormulaParser::SINCE, 0); }

FormulaParser::IntervalContext *FormulaParser::SinceIntervalContext::interval() {
    return getRuleContext<FormulaParser::IntervalContext>(0);
}

FormulaParser::SinceIntervalContext::SinceIntervalContext(Release_until_exprContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::SinceIntervalContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitSinceInterval(this);
    else
        return visitor->visitChildren(this);
}
//----------------- ReleaseIntervalContext ------------------------------------------------------------------

std::vector<FormulaParser::UnaryContext *> FormulaParser::ReleaseIntervalContext::unary() {
    return getRuleContexts<FormulaParser::UnaryContext>();
}

FormulaParser::UnaryContext *FormulaParser::ReleaseIntervalContext::unary(size_t i) {
    return getRuleContext<FormulaParser::UnaryContext>(i);
}

tree::TerminalNode *FormulaParser::ReleaseIntervalContext::RELEASE() { return getToken(FormulaParser::RELEASE, 0); }

FormulaParser::IntervalContext *FormulaParser::ReleaseIntervalContext::interval() {
    return getRuleContext<FormulaParser::IntervalContext>(0);
}

FormulaParser::ReleaseIntervalContext::ReleaseIntervalContext(Release_until_exprContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::ReleaseIntervalContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitReleaseInterval(this);
    else
        return visitor->visitChildren(this);
}
//----------------- SinceContext ------------------------------------------------------------------

std::vector<FormulaParser::UnaryContext *> FormulaParser::SinceContext::unary() {
    return getRuleContexts<FormulaParser::UnaryContext>();
}

FormulaParser::UnaryContext *FormulaParser::SinceContext::unary(size_t i) {
    return getRuleContext<FormulaParser::UnaryContext>(i);
}

tree::TerminalNode *FormulaParser::SinceContext::SINCE() { return getToken(FormulaParser::SINCE, 0); }

FormulaParser::SinceContext::SinceContext(Release_until_exprContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::SinceContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitSince(this);
    else
        return visitor->visitChildren(this);
}
//----------------- UntilIntervalContext ------------------------------------------------------------------

std::vector<FormulaParser::UnaryContext *> FormulaParser::UntilIntervalContext::unary() {
    return getRuleContexts<FormulaParser::UnaryContext>();
}

FormulaParser::UnaryContext *FormulaParser::UntilIntervalContext::unary(size_t i) {
    return getRuleContext<FormulaParser::UnaryContext>(i);
}

tree::TerminalNode *FormulaParser::UntilIntervalContext::UNTIL() { return getToken(FormulaParser::UNTIL, 0); }

FormulaParser::IntervalContext *FormulaParser::UntilIntervalContext::interval() {
    return getRuleContext<FormulaParser::IntervalContext>(0);
}

FormulaParser::UntilIntervalContext::UntilIntervalContext(Release_until_exprContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::UntilIntervalContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitUntilInterval(this);
    else
        return visitor->visitChildren(this);
}
//----------------- UnaryExprContext ------------------------------------------------------------------

FormulaParser::UnaryContext *FormulaParser::UnaryExprContext::unary() {
    return getRuleContext<FormulaParser::UnaryContext>(0);
}

FormulaParser::UnaryExprContext::UnaryExprContext(Release_until_exprContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::UnaryExprContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitUnaryExpr(this);
    else
        return visitor->visitChildren(this);
}
//----------------- ReleaseContext ------------------------------------------------------------------

std::vector<FormulaParser::UnaryContext *> FormulaParser::ReleaseContext::unary() {
    return getRuleContexts<FormulaParser::UnaryContext>();
}

FormulaParser::UnaryContext *FormulaParser::ReleaseContext::unary(size_t i) {
    return getRuleContext<FormulaParser::UnaryContext>(i);
}

tree::TerminalNode *FormulaParser::ReleaseContext::RELEASE() { return getToken(FormulaParser::RELEASE, 0); }

FormulaParser::ReleaseContext::ReleaseContext(Release_until_exprContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::ReleaseContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitRelease(this);
    else
        return visitor->visitChildren(this);
}
//----------------- UntilContext ------------------------------------------------------------------

std::vector<FormulaParser::UnaryContext *> FormulaParser::UntilContext::unary() {
    return getRuleContexts<FormulaParser::UnaryContext>();
}

FormulaParser::UnaryContext *FormulaParser::UntilContext::unary(size_t i) {
    return getRuleContext<FormulaParser::UnaryContext>(i);
}

tree::TerminalNode *FormulaParser::UntilContext::UNTIL() { return getToken(FormulaParser::UNTIL, 0); }

FormulaParser::UntilContext::UntilContext(Release_until_exprContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::UntilContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitUntil(this);
    else
        return visitor->visitChildren(this);
}
FormulaParser::Release_until_exprContext *FormulaParser::release_until_expr() {
    Release_until_exprContext *_localctx = _tracker.createInstance<Release_until_exprContext>(_ctx, getState());
    enterRule(_localctx, 10, FormulaParser::RuleRelease_until_expr);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(90);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx)) {
        case 1: {
            _localctx = _tracker.createInstance<FormulaParser::ReleaseContext>(_localctx);
            enterOuterAlt(_localctx, 1);
            setState(62);
            antlrcpp::downCast<ReleaseContext *>(_localctx)->left = unary();
            setState(63);
            antlrcpp::downCast<ReleaseContext *>(_localctx)->op = match(FormulaParser::RELEASE);
            setState(64);
            antlrcpp::downCast<ReleaseContext *>(_localctx)->right = unary();
            break;
        }

        case 2: {
            _localctx = _tracker.createInstance<FormulaParser::UntilContext>(_localctx);
            enterOuterAlt(_localctx, 2);
            setState(66);
            antlrcpp::downCast<UntilContext *>(_localctx)->left = unary();
            setState(67);
            antlrcpp::downCast<UntilContext *>(_localctx)->op = match(FormulaParser::UNTIL);
            setState(68);
            antlrcpp::downCast<UntilContext *>(_localctx)->right = unary();
            break;
        }

        case 3: {
            _localctx = _tracker.createInstance<FormulaParser::ReleaseIntervalContext>(_localctx);
            enterOuterAlt(_localctx, 3);
            setState(70);
            antlrcpp::downCast<ReleaseIntervalContext *>(_localctx)->left = unary();
            setState(71);
            antlrcpp::downCast<ReleaseIntervalContext *>(_localctx)->op = match(FormulaParser::RELEASE);
            setState(72);
            antlrcpp::downCast<ReleaseIntervalContext *>(_localctx)->iv = interval();
            setState(73);
            antlrcpp::downCast<ReleaseIntervalContext *>(_localctx)->right = unary();
            break;
        }

        case 4: {
            _localctx = _tracker.createInstance<FormulaParser::UntilIntervalContext>(_localctx);
            enterOuterAlt(_localctx, 4);
            setState(75);
            antlrcpp::downCast<UntilIntervalContext *>(_localctx)->left = unary();
            setState(76);
            antlrcpp::downCast<UntilIntervalContext *>(_localctx)->op = match(FormulaParser::UNTIL);
            setState(77);
            antlrcpp::downCast<UntilIntervalContext *>(_localctx)->iv = interval();
            setState(78);
            antlrcpp::downCast<UntilIntervalContext *>(_localctx)->right = unary();
            break;
        }

        case 5: {
            _localctx = _tracker.createInstance<FormulaParser::SinceContext>(_localctx);
            enterOuterAlt(_localctx, 5);
            setState(80);
            antlrcpp::downCast<SinceContext *>(_localctx)->left = unary();
            setState(81);
            antlrcpp::downCast<SinceContext *>(_localctx)->op = match(FormulaParser::SINCE);
            setState(82);
            antlrcpp::downCast<SinceContext *>(_localctx)->right = unary();
            break;
        }

        case 6: {
            _localctx = _tracker.createInstance<FormulaParser::SinceIntervalContext>(_localctx);
            enterOuterAlt(_localctx, 6);
            setState(84);
            antlrcpp::downCast<SinceIntervalContext *>(_localctx)->left = unary();
            setState(85);
            antlrcpp::downCast<SinceIntervalContext *>(_localctx)->op = match(FormulaParser::SINCE);
            setState(86);
            antlrcpp::downCast<SinceIntervalContext *>(_localctx)->iv = interval();
            setState(87);
            antlrcpp::downCast<SinceIntervalContext *>(_localctx)->right = unary();
            break;
        }

        case 7: {
            _localctx = _tracker.createInstance<FormulaParser::UnaryExprContext>(_localctx);
            enterOuterAlt(_localctx, 7);
            setState(89);
            unary();
            break;
        }

        default:
            break;
        }

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- UnaryContext ------------------------------------------------------------------

FormulaParser::UnaryContext::UnaryContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

size_t FormulaParser::UnaryContext::getRuleIndex() const { return FormulaParser::RuleUnary; }

void FormulaParser::UnaryContext::copyFrom(UnaryContext *ctx) { ParserRuleContext::copyFrom(ctx); }

//----------------- GloballyContext ------------------------------------------------------------------

tree::TerminalNode *FormulaParser::GloballyContext::GLOBALLY() { return getToken(FormulaParser::GLOBALLY, 0); }

FormulaParser::AtomContext *FormulaParser::GloballyContext::atom() {
    return getRuleContext<FormulaParser::AtomContext>(0);
}

FormulaParser::GloballyContext::GloballyContext(UnaryContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::GloballyContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitGlobally(this);
    else
        return visitor->visitChildren(this);
}
//----------------- OnceContext ------------------------------------------------------------------

tree::TerminalNode *FormulaParser::OnceContext::ONCE() { return getToken(FormulaParser::ONCE, 0); }

FormulaParser::AtomContext *FormulaParser::OnceContext::atom() { return getRuleContext<FormulaParser::AtomContext>(0); }

FormulaParser::OnceContext::OnceContext(UnaryContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::OnceContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitOnce(this);
    else
        return visitor->visitChildren(this);
}
//----------------- GloballyIntervalContext ------------------------------------------------------------------

tree::TerminalNode *FormulaParser::GloballyIntervalContext::GLOBALLY() { return getToken(FormulaParser::GLOBALLY, 0); }

FormulaParser::IntervalContext *FormulaParser::GloballyIntervalContext::interval() {
    return getRuleContext<FormulaParser::IntervalContext>(0);
}

FormulaParser::AtomContext *FormulaParser::GloballyIntervalContext::atom() {
    return getRuleContext<FormulaParser::AtomContext>(0);
}

FormulaParser::GloballyIntervalContext::GloballyIntervalContext(UnaryContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::GloballyIntervalContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitGloballyInterval(this);
    else
        return visitor->visitChildren(this);
}
//----------------- PreviouslyIntervalContext ------------------------------------------------------------------

tree::TerminalNode *FormulaParser::PreviouslyIntervalContext::PREVIOUSLY() {
    return getToken(FormulaParser::PREVIOUSLY, 0);
}

FormulaParser::IntervalContext *FormulaParser::PreviouslyIntervalContext::interval() {
    return getRuleContext<FormulaParser::IntervalContext>(0);
}

FormulaParser::AtomContext *FormulaParser::PreviouslyIntervalContext::atom() {
    return getRuleContext<FormulaParser::AtomContext>(0);
}

FormulaParser::PreviouslyIntervalContext::PreviouslyIntervalContext(UnaryContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::PreviouslyIntervalContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitPreviouslyInterval(this);
    else
        return visitor->visitChildren(this);
}
//----------------- HistoricallyIntervalContext ------------------------------------------------------------------

tree::TerminalNode *FormulaParser::HistoricallyIntervalContext::HISTORICALLY() {
    return getToken(FormulaParser::HISTORICALLY, 0);
}

FormulaParser::IntervalContext *FormulaParser::HistoricallyIntervalContext::interval() {
    return getRuleContext<FormulaParser::IntervalContext>(0);
}

FormulaParser::AtomContext *FormulaParser::HistoricallyIntervalContext::atom() {
    return getRuleContext<FormulaParser::AtomContext>(0);
}

FormulaParser::HistoricallyIntervalContext::HistoricallyIntervalContext(UnaryContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::HistoricallyIntervalContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitHistoricallyInterval(this);
    else
        return visitor->visitChildren(this);
}
//----------------- PreviouslyContext ------------------------------------------------------------------

tree::TerminalNode *FormulaParser::PreviouslyContext::PREVIOUSLY() { return getToken(FormulaParser::PREVIOUSLY, 0); }

FormulaParser::AtomContext *FormulaParser::PreviouslyContext::atom() {
    return getRuleContext<FormulaParser::AtomContext>(0);
}

FormulaParser::PreviouslyContext::PreviouslyContext(UnaryContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::PreviouslyContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitPreviously(this);
    else
        return visitor->visitChildren(this);
}
//----------------- NextWeakContext ------------------------------------------------------------------

tree::TerminalNode *FormulaParser::NextWeakContext::NEXT_WEAK() { return getToken(FormulaParser::NEXT_WEAK, 0); }

FormulaParser::AtomContext *FormulaParser::NextWeakContext::atom() {
    return getRuleContext<FormulaParser::AtomContext>(0);
}

FormulaParser::NextWeakContext::NextWeakContext(UnaryContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::NextWeakContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitNextWeak(this);
    else
        return visitor->visitChildren(this);
}
//----------------- NextWeakIntervalContext ------------------------------------------------------------------

tree::TerminalNode *FormulaParser::NextWeakIntervalContext::NEXT_WEAK() {
    return getToken(FormulaParser::NEXT_WEAK, 0);
}

FormulaParser::IntervalContext *FormulaParser::NextWeakIntervalContext::interval() {
    return getRuleContext<FormulaParser::IntervalContext>(0);
}

FormulaParser::AtomContext *FormulaParser::NextWeakIntervalContext::atom() {
    return getRuleContext<FormulaParser::AtomContext>(0);
}

FormulaParser::NextWeakIntervalContext::NextWeakIntervalContext(UnaryContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::NextWeakIntervalContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitNextWeakInterval(this);
    else
        return visitor->visitChildren(this);
}
//----------------- AtomExprContext ------------------------------------------------------------------

FormulaParser::AtomContext *FormulaParser::AtomExprContext::atom() {
    return getRuleContext<FormulaParser::AtomContext>(0);
}

FormulaParser::AtomExprContext::AtomExprContext(UnaryContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::AtomExprContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitAtomExpr(this);
    else
        return visitor->visitChildren(this);
}
//----------------- HistoricallyContext ------------------------------------------------------------------

tree::TerminalNode *FormulaParser::HistoricallyContext::HISTORICALLY() {
    return getToken(FormulaParser::HISTORICALLY, 0);
}

FormulaParser::AtomContext *FormulaParser::HistoricallyContext::atom() {
    return getRuleContext<FormulaParser::AtomContext>(0);
}

FormulaParser::HistoricallyContext::HistoricallyContext(UnaryContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::HistoricallyContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitHistorically(this);
    else
        return visitor->visitChildren(this);
}
//----------------- NextStrongIntervalContext ------------------------------------------------------------------

tree::TerminalNode *FormulaParser::NextStrongIntervalContext::NEXT_STRONG() {
    return getToken(FormulaParser::NEXT_STRONG, 0);
}

FormulaParser::IntervalContext *FormulaParser::NextStrongIntervalContext::interval() {
    return getRuleContext<FormulaParser::IntervalContext>(0);
}

FormulaParser::AtomContext *FormulaParser::NextStrongIntervalContext::atom() {
    return getRuleContext<FormulaParser::AtomContext>(0);
}

FormulaParser::NextStrongIntervalContext::NextStrongIntervalContext(UnaryContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::NextStrongIntervalContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitNextStrongInterval(this);
    else
        return visitor->visitChildren(this);
}
//----------------- PredicateContext ------------------------------------------------------------------

tree::TerminalNode *FormulaParser::PredicateContext::PREDICATE() { return getToken(FormulaParser::PREDICATE, 0); }

std::vector<tree::TerminalNode *> FormulaParser::PredicateContext::ATOMIC() { return getTokens(FormulaParser::ATOMIC); }

tree::TerminalNode *FormulaParser::PredicateContext::ATOMIC(size_t i) { return getToken(FormulaParser::ATOMIC, i); }

FormulaParser::PredicateContext::PredicateContext(UnaryContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::PredicateContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitPredicate(this);
    else
        return visitor->visitChildren(this);
}
//----------------- NextStrongContext ------------------------------------------------------------------

tree::TerminalNode *FormulaParser::NextStrongContext::NEXT_STRONG() { return getToken(FormulaParser::NEXT_STRONG, 0); }

FormulaParser::AtomContext *FormulaParser::NextStrongContext::atom() {
    return getRuleContext<FormulaParser::AtomContext>(0);
}

FormulaParser::NextStrongContext::NextStrongContext(UnaryContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::NextStrongContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitNextStrong(this);
    else
        return visitor->visitChildren(this);
}
//----------------- OnceIntervalContext ------------------------------------------------------------------

tree::TerminalNode *FormulaParser::OnceIntervalContext::ONCE() { return getToken(FormulaParser::ONCE, 0); }

FormulaParser::IntervalContext *FormulaParser::OnceIntervalContext::interval() {
    return getRuleContext<FormulaParser::IntervalContext>(0);
}

FormulaParser::AtomContext *FormulaParser::OnceIntervalContext::atom() {
    return getRuleContext<FormulaParser::AtomContext>(0);
}

FormulaParser::OnceIntervalContext::OnceIntervalContext(UnaryContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::OnceIntervalContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitOnceInterval(this);
    else
        return visitor->visitChildren(this);
}
//----------------- FutureIntervalContext ------------------------------------------------------------------

tree::TerminalNode *FormulaParser::FutureIntervalContext::FUTURE() { return getToken(FormulaParser::FUTURE, 0); }

FormulaParser::IntervalContext *FormulaParser::FutureIntervalContext::interval() {
    return getRuleContext<FormulaParser::IntervalContext>(0);
}

FormulaParser::AtomContext *FormulaParser::FutureIntervalContext::atom() {
    return getRuleContext<FormulaParser::AtomContext>(0);
}

FormulaParser::FutureIntervalContext::FutureIntervalContext(UnaryContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::FutureIntervalContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitFutureInterval(this);
    else
        return visitor->visitChildren(this);
}
//----------------- NotContext ------------------------------------------------------------------

tree::TerminalNode *FormulaParser::NotContext::NOT() { return getToken(FormulaParser::NOT, 0); }

FormulaParser::AtomContext *FormulaParser::NotContext::atom() { return getRuleContext<FormulaParser::AtomContext>(0); }

FormulaParser::NotContext::NotContext(UnaryContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::NotContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitNot(this);
    else
        return visitor->visitChildren(this);
}
//----------------- FutureContext ------------------------------------------------------------------

tree::TerminalNode *FormulaParser::FutureContext::FUTURE() { return getToken(FormulaParser::FUTURE, 0); }

FormulaParser::AtomContext *FormulaParser::FutureContext::atom() {
    return getRuleContext<FormulaParser::AtomContext>(0);
}

FormulaParser::FutureContext::FutureContext(UnaryContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::FutureContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitFuture(this);
    else
        return visitor->visitChildren(this);
}
//----------------- NotExprContext ------------------------------------------------------------------

tree::TerminalNode *FormulaParser::NotExprContext::NOT() { return getToken(FormulaParser::NOT, 0); }

FormulaParser::ExprContext *FormulaParser::NotExprContext::expr() {
    return getRuleContext<FormulaParser::ExprContext>(0);
}

FormulaParser::NotExprContext::NotExprContext(UnaryContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::NotExprContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitNotExpr(this);
    else
        return visitor->visitChildren(this);
}
FormulaParser::UnaryContext *FormulaParser::unary() {
    UnaryContext *_localctx = _tracker.createInstance<UnaryContext>(_ctx, getState());
    enterRule(_localctx, 12, FormulaParser::RuleUnary);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(149);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx)) {
        case 1: {
            _localctx = _tracker.createInstance<FormulaParser::GloballyContext>(_localctx);
            enterOuterAlt(_localctx, 1);
            setState(92);
            antlrcpp::downCast<GloballyContext *>(_localctx)->op = match(FormulaParser::GLOBALLY);
            setState(93);
            antlrcpp::downCast<GloballyContext *>(_localctx)->right = atom();
            break;
        }

        case 2: {
            _localctx = _tracker.createInstance<FormulaParser::FutureContext>(_localctx);
            enterOuterAlt(_localctx, 2);
            setState(94);
            antlrcpp::downCast<FutureContext *>(_localctx)->op = match(FormulaParser::FUTURE);
            setState(95);
            antlrcpp::downCast<FutureContext *>(_localctx)->right = atom();
            break;
        }

        case 3: {
            _localctx = _tracker.createInstance<FormulaParser::OnceContext>(_localctx);
            enterOuterAlt(_localctx, 3);
            setState(96);
            antlrcpp::downCast<OnceContext *>(_localctx)->op = match(FormulaParser::ONCE);
            setState(97);
            antlrcpp::downCast<OnceContext *>(_localctx)->right = atom();
            break;
        }

        case 4: {
            _localctx = _tracker.createInstance<FormulaParser::PreviouslyContext>(_localctx);
            enterOuterAlt(_localctx, 4);
            setState(98);
            antlrcpp::downCast<PreviouslyContext *>(_localctx)->op = match(FormulaParser::PREVIOUSLY);
            setState(99);
            antlrcpp::downCast<PreviouslyContext *>(_localctx)->right = atom();
            break;
        }

        case 5: {
            _localctx = _tracker.createInstance<FormulaParser::NextWeakContext>(_localctx);
            enterOuterAlt(_localctx, 5);
            setState(100);
            antlrcpp::downCast<NextWeakContext *>(_localctx)->op = match(FormulaParser::NEXT_WEAK);
            setState(101);
            antlrcpp::downCast<NextWeakContext *>(_localctx)->right = atom();
            break;
        }

        case 6: {
            _localctx = _tracker.createInstance<FormulaParser::NextStrongContext>(_localctx);
            enterOuterAlt(_localctx, 6);
            setState(102);
            antlrcpp::downCast<NextStrongContext *>(_localctx)->op = match(FormulaParser::NEXT_STRONG);
            setState(103);
            antlrcpp::downCast<NextStrongContext *>(_localctx)->right = atom();
            break;
        }

        case 7: {
            _localctx = _tracker.createInstance<FormulaParser::GloballyIntervalContext>(_localctx);
            enterOuterAlt(_localctx, 7);
            setState(104);
            antlrcpp::downCast<GloballyIntervalContext *>(_localctx)->op = match(FormulaParser::GLOBALLY);
            setState(105);
            antlrcpp::downCast<GloballyIntervalContext *>(_localctx)->iv = interval();
            setState(106);
            antlrcpp::downCast<GloballyIntervalContext *>(_localctx)->right = atom();
            break;
        }

        case 8: {
            _localctx = _tracker.createInstance<FormulaParser::FutureIntervalContext>(_localctx);
            enterOuterAlt(_localctx, 8);
            setState(108);
            antlrcpp::downCast<FutureIntervalContext *>(_localctx)->op = match(FormulaParser::FUTURE);
            setState(109);
            antlrcpp::downCast<FutureIntervalContext *>(_localctx)->iv = interval();
            setState(110);
            antlrcpp::downCast<FutureIntervalContext *>(_localctx)->right = atom();
            break;
        }

        case 9: {
            _localctx = _tracker.createInstance<FormulaParser::OnceIntervalContext>(_localctx);
            enterOuterAlt(_localctx, 9);
            setState(112);
            antlrcpp::downCast<OnceIntervalContext *>(_localctx)->op = match(FormulaParser::ONCE);
            setState(113);
            antlrcpp::downCast<OnceIntervalContext *>(_localctx)->iv = interval();
            setState(114);
            antlrcpp::downCast<OnceIntervalContext *>(_localctx)->right = atom();
            break;
        }

        case 10: {
            _localctx = _tracker.createInstance<FormulaParser::PreviouslyIntervalContext>(_localctx);
            enterOuterAlt(_localctx, 10);
            setState(116);
            antlrcpp::downCast<PreviouslyIntervalContext *>(_localctx)->op = match(FormulaParser::PREVIOUSLY);
            setState(117);
            antlrcpp::downCast<PreviouslyIntervalContext *>(_localctx)->iv = interval();
            setState(118);
            antlrcpp::downCast<PreviouslyIntervalContext *>(_localctx)->right = atom();
            break;
        }

        case 11: {
            _localctx = _tracker.createInstance<FormulaParser::NextWeakIntervalContext>(_localctx);
            enterOuterAlt(_localctx, 11);
            setState(120);
            antlrcpp::downCast<NextWeakIntervalContext *>(_localctx)->op = match(FormulaParser::NEXT_WEAK);
            setState(121);
            antlrcpp::downCast<NextWeakIntervalContext *>(_localctx)->iv = interval();
            setState(122);
            antlrcpp::downCast<NextWeakIntervalContext *>(_localctx)->right = atom();
            break;
        }

        case 12: {
            _localctx = _tracker.createInstance<FormulaParser::NextStrongIntervalContext>(_localctx);
            enterOuterAlt(_localctx, 12);
            setState(124);
            antlrcpp::downCast<NextStrongIntervalContext *>(_localctx)->op = match(FormulaParser::NEXT_STRONG);
            setState(125);
            antlrcpp::downCast<NextStrongIntervalContext *>(_localctx)->iv = interval();
            setState(126);
            antlrcpp::downCast<NextStrongIntervalContext *>(_localctx)->right = atom();
            break;
        }

        case 13: {
            _localctx = _tracker.createInstance<FormulaParser::HistoricallyContext>(_localctx);
            enterOuterAlt(_localctx, 13);
            setState(128);
            antlrcpp::downCast<HistoricallyContext *>(_localctx)->op = match(FormulaParser::HISTORICALLY);
            setState(129);
            antlrcpp::downCast<HistoricallyContext *>(_localctx)->right = atom();
            break;
        }

        case 14: {
            _localctx = _tracker.createInstance<FormulaParser::HistoricallyIntervalContext>(_localctx);
            enterOuterAlt(_localctx, 14);
            setState(130);
            antlrcpp::downCast<HistoricallyIntervalContext *>(_localctx)->op = match(FormulaParser::HISTORICALLY);
            setState(131);
            antlrcpp::downCast<HistoricallyIntervalContext *>(_localctx)->iv = interval();
            setState(132);
            antlrcpp::downCast<HistoricallyIntervalContext *>(_localctx)->right = atom();
            break;
        }

        case 15: {
            _localctx = _tracker.createInstance<FormulaParser::NotContext>(_localctx);
            enterOuterAlt(_localctx, 15);
            setState(134);
            antlrcpp::downCast<NotContext *>(_localctx)->op = match(FormulaParser::NOT);
            setState(135);
            antlrcpp::downCast<NotContext *>(_localctx)->right = atom();
            break;
        }

        case 16: {
            _localctx = _tracker.createInstance<FormulaParser::NotExprContext>(_localctx);
            enterOuterAlt(_localctx, 16);
            setState(136);
            antlrcpp::downCast<NotExprContext *>(_localctx)->op = match(FormulaParser::NOT);
            setState(137);
            antlrcpp::downCast<NotExprContext *>(_localctx)->right = expr(0);
            break;
        }

        case 17: {
            _localctx = _tracker.createInstance<FormulaParser::AtomExprContext>(_localctx);
            enterOuterAlt(_localctx, 17);
            setState(138);
            atom();
            break;
        }

        case 18: {
            _localctx = _tracker.createInstance<FormulaParser::PredicateContext>(_localctx);
            enterOuterAlt(_localctx, 18);
            setState(139);
            antlrcpp::downCast<PredicateContext *>(_localctx)->op = match(FormulaParser::PREDICATE);
            setState(140);
            antlrcpp::downCast<PredicateContext *>(_localctx)->right = match(FormulaParser::ATOMIC);
            setState(145);
            _errHandler->sync(this);
            _la = _input->LA(1);
            while (_la == FormulaParser::T__0) {
                setState(141);
                match(FormulaParser::T__0);
                setState(142);
                match(FormulaParser::ATOMIC);
                setState(147);
                _errHandler->sync(this);
                _la = _input->LA(1);
            }
            setState(148);
            match(FormulaParser::T__1);
            break;
        }

        default:
            break;
        }

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- IntervalContext ------------------------------------------------------------------

FormulaParser::IntervalContext::IntervalContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

size_t FormulaParser::IntervalContext::getRuleIndex() const { return FormulaParser::RuleInterval; }

void FormulaParser::IntervalContext::copyFrom(IntervalContext *ctx) { ParserRuleContext::copyFrom(ctx); }

//----------------- FiniteIntervalContext ------------------------------------------------------------------

std::vector<FormulaParser::NumberContext *> FormulaParser::FiniteIntervalContext::number() {
    return getRuleContexts<FormulaParser::NumberContext>();
}

FormulaParser::NumberContext *FormulaParser::FiniteIntervalContext::number(size_t i) {
    return getRuleContext<FormulaParser::NumberContext>(i);
}

FormulaParser::FiniteIntervalContext::FiniteIntervalContext(IntervalContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::FiniteIntervalContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitFiniteInterval(this);
    else
        return visitor->visitChildren(this);
}
//----------------- InfiniteIntervalContext ------------------------------------------------------------------

FormulaParser::NumberContext *FormulaParser::InfiniteIntervalContext::number() {
    return getRuleContext<FormulaParser::NumberContext>(0);
}

tree::TerminalNode *FormulaParser::InfiniteIntervalContext::INFINITY() { return getToken(FormulaParser::INFINITY, 0); }

FormulaParser::InfiniteIntervalContext::InfiniteIntervalContext(IntervalContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::InfiniteIntervalContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitInfiniteInterval(this);
    else
        return visitor->visitChildren(this);
}
FormulaParser::IntervalContext *FormulaParser::interval() {
    IntervalContext *_localctx = _tracker.createInstance<IntervalContext>(_ctx, getState());
    enterRule(_localctx, 14, FormulaParser::RuleInterval);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(163);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 8, _ctx)) {
        case 1: {
            _localctx = _tracker.createInstance<FormulaParser::FiniteIntervalContext>(_localctx);
            enterOuterAlt(_localctx, 1);
            setState(151);
            match(FormulaParser::T__2);
            setState(152);
            antlrcpp::downCast<FiniteIntervalContext *>(_localctx)->l = number();
            setState(153);
            match(FormulaParser::T__0);
            setState(154);
            antlrcpp::downCast<FiniteIntervalContext *>(_localctx)->u = number();
            setState(155);
            match(FormulaParser::T__3);
            break;
        }

        case 2: {
            _localctx = _tracker.createInstance<FormulaParser::InfiniteIntervalContext>(_localctx);
            enterOuterAlt(_localctx, 2);
            setState(157);
            match(FormulaParser::T__2);
            setState(158);
            antlrcpp::downCast<InfiniteIntervalContext *>(_localctx)->l = number();
            setState(159);
            match(FormulaParser::T__0);
            setState(160);
            antlrcpp::downCast<InfiniteIntervalContext *>(_localctx)->u = match(FormulaParser::INFINITY);
            setState(161);
            match(FormulaParser::T__3);
            break;
        }

        default:
            break;
        }

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- AtomContext ------------------------------------------------------------------

FormulaParser::AtomContext::AtomContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

size_t FormulaParser::AtomContext::getRuleIndex() const { return FormulaParser::RuleAtom; }

void FormulaParser::AtomContext::copyFrom(AtomContext *ctx) { ParserRuleContext::copyFrom(ctx); }

//----------------- BracketsContext ------------------------------------------------------------------

std::vector<FormulaParser::ExprContext *> FormulaParser::BracketsContext::expr() {
    return getRuleContexts<FormulaParser::ExprContext>();
}

FormulaParser::ExprContext *FormulaParser::BracketsContext::expr(size_t i) {
    return getRuleContext<FormulaParser::ExprContext>(i);
}

FormulaParser::BracketsContext::BracketsContext(AtomContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::BracketsContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitBrackets(this);
    else
        return visitor->visitChildren(this);
}
//----------------- AtomicContext ------------------------------------------------------------------

tree::TerminalNode *FormulaParser::AtomicContext::ATOMIC() { return getToken(FormulaParser::ATOMIC, 0); }

FormulaParser::AtomicContext::AtomicContext(AtomContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::AtomicContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitAtomic(this);
    else
        return visitor->visitChildren(this);
}
//----------------- BooleanContext ------------------------------------------------------------------

FormulaParser::Bool_Context *FormulaParser::BooleanContext::bool_() {
    return getRuleContext<FormulaParser::Bool_Context>(0);
}

FormulaParser::BooleanContext::BooleanContext(AtomContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::BooleanContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitBoolean(this);
    else
        return visitor->visitChildren(this);
}
FormulaParser::AtomContext *FormulaParser::atom() {
    AtomContext *_localctx = _tracker.createInstance<AtomContext>(_ctx, getState());
    enterRule(_localctx, 16, FormulaParser::RuleAtom);
    size_t _la = 0;

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(175);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
        case FormulaParser::TRUE:
        case FormulaParser::FALSE: {
            _localctx = _tracker.createInstance<FormulaParser::BooleanContext>(_localctx);
            enterOuterAlt(_localctx, 1);
            setState(165);
            bool_();
            break;
        }

        case FormulaParser::ATOMIC: {
            _localctx = _tracker.createInstance<FormulaParser::AtomicContext>(_localctx);
            enterOuterAlt(_localctx, 2);
            setState(166);
            match(FormulaParser::ATOMIC);
            break;
        }

        case FormulaParser::T__4: {
            _localctx = _tracker.createInstance<FormulaParser::BracketsContext>(_localctx);
            enterOuterAlt(_localctx, 3);
            setState(167);
            match(FormulaParser::T__4);
            setState(169);
            _errHandler->sync(this);
            _la = _input->LA(1);
            do {
                setState(168);
                antlrcpp::downCast<BracketsContext *>(_localctx)->content = expr(0);
                setState(171);
                _errHandler->sync(this);
                _la = _input->LA(1);
            } while ((((_la & ~0x3fULL) == 0) &&
                      ((1ULL << _la) &
                       ((1ULL << FormulaParser::T__4) | (1ULL << FormulaParser::TRUE) | (1ULL << FormulaParser::FALSE) |
                        (1ULL << FormulaParser::NOT) | (1ULL << FormulaParser::GLOBALLY) |
                        (1ULL << FormulaParser::FUTURE) | (1ULL << FormulaParser::NEXT_WEAK) |
                        (1ULL << FormulaParser::NEXT_STRONG) | (1ULL << FormulaParser::ONCE) |
                        (1ULL << FormulaParser::PREVIOUSLY) | (1ULL << FormulaParser::HISTORICALLY) |
                        (1ULL << FormulaParser::ATOMIC) | (1ULL << FormulaParser::PREDICATE))) != 0));
            setState(173);
            match(FormulaParser::T__1);
            break;
        }

        default:
            throw NoViableAltException(this);
        }

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- NumberContext ------------------------------------------------------------------

FormulaParser::NumberContext::NumberContext(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

size_t FormulaParser::NumberContext::getRuleIndex() const { return FormulaParser::RuleNumber; }

void FormulaParser::NumberContext::copyFrom(NumberContext *ctx) { ParserRuleContext::copyFrom(ctx); }

//----------------- DigitContext ------------------------------------------------------------------

tree::TerminalNode *FormulaParser::DigitContext::DIGIT() { return getToken(FormulaParser::DIGIT, 0); }

FormulaParser::DigitContext::DigitContext(NumberContext *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::DigitContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitDigit(this);
    else
        return visitor->visitChildren(this);
}
FormulaParser::NumberContext *FormulaParser::number() {
    NumberContext *_localctx = _tracker.createInstance<NumberContext>(_ctx, getState());
    enterRule(_localctx, 18, FormulaParser::RuleNumber);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        _localctx = _tracker.createInstance<FormulaParser::DigitContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(177);
        match(FormulaParser::DIGIT);

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

//----------------- Bool_Context ------------------------------------------------------------------

FormulaParser::Bool_Context::Bool_Context(ParserRuleContext *parent, size_t invokingState)
    : ParserRuleContext(parent, invokingState) {}

size_t FormulaParser::Bool_Context::getRuleIndex() const { return FormulaParser::RuleBool_; }

void FormulaParser::Bool_Context::copyFrom(Bool_Context *ctx) { ParserRuleContext::copyFrom(ctx); }

//----------------- TrueBooleanContext ------------------------------------------------------------------

tree::TerminalNode *FormulaParser::TrueBooleanContext::TRUE() { return getToken(FormulaParser::TRUE, 0); }

FormulaParser::TrueBooleanContext::TrueBooleanContext(Bool_Context *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::TrueBooleanContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitTrueBoolean(this);
    else
        return visitor->visitChildren(this);
}
//----------------- FalseBooleanContext ------------------------------------------------------------------

tree::TerminalNode *FormulaParser::FalseBooleanContext::FALSE() { return getToken(FormulaParser::FALSE, 0); }

FormulaParser::FalseBooleanContext::FalseBooleanContext(Bool_Context *ctx) { copyFrom(ctx); }

antlrcpp::Any FormulaParser::FalseBooleanContext::accept(tree::ParseTreeVisitor *visitor) {
    if (auto parserVisitor = dynamic_cast<FormulaVisitor *>(visitor))
        return parserVisitor->visitFalseBoolean(this);
    else
        return visitor->visitChildren(this);
}
FormulaParser::Bool_Context *FormulaParser::bool_() {
    Bool_Context *_localctx = _tracker.createInstance<Bool_Context>(_ctx, getState());
    enterRule(_localctx, 20, FormulaParser::RuleBool_);

#if __cplusplus > 201703L
    auto onExit = finally([=, this] {
#else
    auto onExit = finally([=] {
#endif
        exitRule();
    });
    try {
        setState(181);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
        case FormulaParser::TRUE: {
            _localctx = _tracker.createInstance<FormulaParser::TrueBooleanContext>(_localctx);
            enterOuterAlt(_localctx, 1);
            setState(179);
            match(FormulaParser::TRUE);
            break;
        }

        case FormulaParser::FALSE: {
            _localctx = _tracker.createInstance<FormulaParser::FalseBooleanContext>(_localctx);
            enterOuterAlt(_localctx, 2);
            setState(180);
            match(FormulaParser::FALSE);
            break;
        }

        default:
            throw NoViableAltException(this);
        }

    } catch (RecognitionException &e) {
        _errHandler->reportError(this, e);
        _localctx->exception = std::current_exception();
        _errHandler->recover(this, _localctx->exception);
    }

    return _localctx;
}

bool FormulaParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
    switch (ruleIndex) {
    case 1:
        return exprSempred(antlrcpp::downCast<ExprContext *>(context), predicateIndex);

    default:
        break;
    }
    return true;
}

bool FormulaParser::exprSempred(ExprContext *_localctx, size_t predicateIndex) {
    switch (predicateIndex) {
    case 0:
        return precpred(_ctx, 3);
    case 1:
        return precpred(_ctx, 2);

    default:
        break;
    }
    return true;
}

// Static vars and initialization.
std::vector<dfa::DFA> FormulaParser::_decisionToDFA;
atn::PredictionContextCache FormulaParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN FormulaParser::_atn;
std::vector<uint16_t> FormulaParser::_serializedATN;

std::vector<std::string> FormulaParser::_ruleNames = {
    "formula", "expr",     "xor_expr", "or_expr", "and_expr", "release_until_expr",
    "unary",   "interval", "atom",     "number",  "bool_"};

std::vector<std::string> FormulaParser::_literalNames = {"", "','", "')'", "'['", "']'", "'('",  "",     "",    "",
                                                         "", "'G'", "'F'", "'W'", "",    "",     "'R'",  "'O'", "",
                                                         "", "'H'", "",    "",    "",    "'->'", "'<->'"};

std::vector<std::string> FormulaParser::_symbolicNames = {"",          "",
                                                          "",          "",
                                                          "",          "",
                                                          "TRUE",      "FALSE",
                                                          "INFINITY",  "NOT",
                                                          "GLOBALLY",  "FUTURE",
                                                          "NEXT_WEAK", "NEXT_STRONG",
                                                          "UNTIL",     "RELEASE",
                                                          "ONCE",      "PREVIOUSLY",
                                                          "SINCE",     "HISTORICALLY",
                                                          "AND",       "OR",
                                                          "XOR",       "IMPL",
                                                          "EQ",        "ATOMIC",
                                                          "PREDICATE", "WS",
                                                          "DIGIT"};

dfa::Vocabulary FormulaParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> FormulaParser::_tokenNames;

FormulaParser::Initializer::Initializer() {
    for (size_t i = 0; i < _symbolicNames.size(); ++i) {
        std::string name = _vocabulary.getLiteralName(i);
        if (name.empty()) {
            name = _vocabulary.getSymbolicName(i);
        }

        if (name.empty()) {
            _tokenNames.push_back("<INVALID>");
        } else {
            _tokenNames.push_back(name);
        }
    }

    static const uint16_t serializedATNSegment0[] = {
        0x3,  0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 0x3,  0x1e, 0xba, 0x4,  0x2,  0x9,  0x2,
        0x4,  0x3,    0x9,    0x3,    0x4,    0x4,    0x9,    0x4,    0x4,    0x5,  0x9,  0x5,  0x4,  0x6,  0x9,  0x6,
        0x4,  0x7,    0x9,    0x7,    0x4,    0x8,    0x9,    0x8,    0x4,    0x9,  0x9,  0x9,  0x4,  0xa,  0x9,  0xa,
        0x4,  0xb,    0x9,    0xb,    0x4,    0xc,    0x9,    0xc,    0x3,    0x2,  0x3,  0x2,  0x3,  0x3,  0x3,  0x3,
        0x3,  0x3,    0x3,    0x3,    0x3,    0x3,    0x3,    0x3,    0x3,    0x3,  0x3,  0x3,  0x3,  0x3,  0x7,  0x3,
        0x24, 0xa,    0x3,    0xc,    0x3,    0xe,    0x3,    0x27,   0xb,    0x3,  0x3,  0x4,  0x3,  0x4,  0x3,  0x4,
        0x7,  0x4,    0x2c,   0xa,    0x4,    0xc,    0x4,    0xe,    0x4,    0x2f, 0xb,  0x4,  0x3,  0x5,  0x3,  0x5,
        0x3,  0x5,    0x7,    0x5,    0x34,   0xa,    0x5,    0xc,    0x5,    0xe,  0x5,  0x37, 0xb,  0x5,  0x3,  0x6,
        0x3,  0x6,    0x3,    0x6,    0x7,    0x6,    0x3c,   0xa,    0x6,    0xc,  0x6,  0xe,  0x6,  0x3f, 0xb,  0x6,
        0x3,  0x7,    0x3,    0x7,    0x3,    0x7,    0x3,    0x7,    0x3,    0x7,  0x3,  0x7,  0x3,  0x7,  0x3,  0x7,
        0x3,  0x7,    0x3,    0x7,    0x3,    0x7,    0x3,    0x7,    0x3,    0x7,  0x3,  0x7,  0x3,  0x7,  0x3,  0x7,
        0x3,  0x7,    0x3,    0x7,    0x3,    0x7,    0x3,    0x7,    0x3,    0x7,  0x3,  0x7,  0x3,  0x7,  0x3,  0x7,
        0x3,  0x7,    0x3,    0x7,    0x3,    0x7,    0x3,    0x7,    0x5,    0x7,  0x5d, 0xa,  0x7,  0x3,  0x8,  0x3,
        0x8,  0x3,    0x8,    0x3,    0x8,    0x3,    0x8,    0x3,    0x8,    0x3,  0x8,  0x3,  0x8,  0x3,  0x8,  0x3,
        0x8,  0x3,    0x8,    0x3,    0x8,    0x3,    0x8,    0x3,    0x8,    0x3,  0x8,  0x3,  0x8,  0x3,  0x8,  0x3,
        0x8,  0x3,    0x8,    0x3,    0x8,    0x3,    0x8,    0x3,    0x8,    0x3,  0x8,  0x3,  0x8,  0x3,  0x8,  0x3,
        0x8,  0x3,    0x8,    0x3,    0x8,    0x3,    0x8,    0x3,    0x8,    0x3,  0x8,  0x3,  0x8,  0x3,  0x8,  0x3,
        0x8,  0x3,    0x8,    0x3,    0x8,    0x3,    0x8,    0x3,    0x8,    0x3,  0x8,  0x3,  0x8,  0x3,  0x8,  0x3,
        0x8,  0x3,    0x8,    0x3,    0x8,    0x3,    0x8,    0x3,    0x8,    0x3,  0x8,  0x3,  0x8,  0x3,  0x8,  0x3,
        0x8,  0x3,    0x8,    0x7,    0x8,    0x92,   0xa,    0x8,    0xc,    0x8,  0xe,  0x8,  0x95, 0xb,  0x8,  0x3,
        0x8,  0x5,    0x8,    0x98,   0xa,    0x8,    0x3,    0x9,    0x3,    0x9,  0x3,  0x9,  0x3,  0x9,  0x3,  0x9,
        0x3,  0x9,    0x3,    0x9,    0x3,    0x9,    0x3,    0x9,    0x3,    0x9,  0x3,  0x9,  0x3,  0x9,  0x5,  0x9,
        0xa6, 0xa,    0x9,    0x3,    0xa,    0x3,    0xa,    0x3,    0xa,    0x3,  0xa,  0x6,  0xa,  0xac, 0xa,  0xa,
        0xd,  0xa,    0xe,    0xa,    0xad,   0x3,    0xa,    0x3,    0xa,    0x5,  0xa,  0xb2, 0xa,  0xa,  0x3,  0xb,
        0x3,  0xb,    0x3,    0xc,    0x3,    0xc,    0x5,    0xc,    0xb8,   0xa,  0xc,  0x3,  0xc,  0x2,  0x3,  0x4,
        0xd,  0x2,    0x4,    0x6,    0x8,    0xa,    0xc,    0xe,    0x10,   0x12, 0x14, 0x16, 0x2,  0x2,  0x2,  0xd0,
        0x2,  0x18,   0x3,    0x2,    0x2,    0x2,    0x4,    0x1a,   0x3,    0x2,  0x2,  0x2,  0x6,  0x28, 0x3,  0x2,
        0x2,  0x2,    0x8,    0x30,   0x3,    0x2,    0x2,    0x2,    0xa,    0x38, 0x3,  0x2,  0x2,  0x2,  0xc,  0x5c,
        0x3,  0x2,    0x2,    0x2,    0xe,    0x97,   0x3,    0x2,    0x2,    0x2,  0x10, 0xa5, 0x3,  0x2,  0x2,  0x2,
        0x12, 0xb1,   0x3,    0x2,    0x2,    0x2,    0x14,   0xb3,   0x3,    0x2,  0x2,  0x2,  0x16, 0xb7, 0x3,  0x2,
        0x2,  0x2,    0x18,   0x19,   0x5,    0x4,    0x3,    0x2,    0x19,   0x3,  0x3,  0x2,  0x2,  0x2,  0x1a, 0x1b,
        0x8,  0x3,    0x1,    0x2,    0x1b,   0x1c,   0x5,    0x6,    0x4,    0x2,  0x1c, 0x25, 0x3,  0x2,  0x2,  0x2,
        0x1d, 0x1e,   0xc,    0x5,    0x2,    0x2,    0x1e,   0x1f,   0x7,    0x19, 0x2,  0x2,  0x1f, 0x24, 0x5,  0x4,
        0x3,  0x5,    0x20,   0x21,   0xc,    0x4,    0x2,    0x2,    0x21,   0x22, 0x7,  0x1a, 0x2,  0x2,  0x22, 0x24,
        0x5,  0x4,    0x3,    0x4,    0x23,   0x1d,   0x3,    0x2,    0x2,    0x2,  0x23, 0x20, 0x3,  0x2,  0x2,  0x2,
        0x24, 0x27,   0x3,    0x2,    0x2,    0x2,    0x25,   0x23,   0x3,    0x2,  0x2,  0x2,  0x25, 0x26, 0x3,  0x2,
        0x2,  0x2,    0x26,   0x5,    0x3,    0x2,    0x2,    0x2,    0x27,   0x25, 0x3,  0x2,  0x2,  0x2,  0x28, 0x2d,
        0x5,  0x8,    0x5,    0x2,    0x29,   0x2a,   0x7,    0x18,   0x2,    0x2,  0x2a, 0x2c, 0x5,  0x8,  0x5,  0x2,
        0x2b, 0x29,   0x3,    0x2,    0x2,    0x2,    0x2c,   0x2f,   0x3,    0x2,  0x2,  0x2,  0x2d, 0x2b, 0x3,  0x2,
        0x2,  0x2,    0x2d,   0x2e,   0x3,    0x2,    0x2,    0x2,    0x2e,   0x7,  0x3,  0x2,  0x2,  0x2,  0x2f, 0x2d,
        0x3,  0x2,    0x2,    0x2,    0x30,   0x35,   0x5,    0xa,    0x6,    0x2,  0x31, 0x32, 0x7,  0x17, 0x2,  0x2,
        0x32, 0x34,   0x5,    0xa,    0x6,    0x2,    0x33,   0x31,   0x3,    0x2,  0x2,  0x2,  0x34, 0x37, 0x3,  0x2,
        0x2,  0x2,    0x35,   0x33,   0x3,    0x2,    0x2,    0x2,    0x35,   0x36, 0x3,  0x2,  0x2,  0x2,  0x36, 0x9,
        0x3,  0x2,    0x2,    0x2,    0x37,   0x35,   0x3,    0x2,    0x2,    0x2,  0x38, 0x3d, 0x5,  0xc,  0x7,  0x2,
        0x39, 0x3a,   0x7,    0x16,   0x2,    0x2,    0x3a,   0x3c,   0x5,    0xc,  0x7,  0x2,  0x3b, 0x39, 0x3,  0x2,
        0x2,  0x2,    0x3c,   0x3f,   0x3,    0x2,    0x2,    0x2,    0x3d,   0x3b, 0x3,  0x2,  0x2,  0x2,  0x3d, 0x3e,
        0x3,  0x2,    0x2,    0x2,    0x3e,   0xb,    0x3,    0x2,    0x2,    0x2,  0x3f, 0x3d, 0x3,  0x2,  0x2,  0x2,
        0x40, 0x41,   0x5,    0xe,    0x8,    0x2,    0x41,   0x42,   0x7,    0x11, 0x2,  0x2,  0x42, 0x43, 0x5,  0xe,
        0x8,  0x2,    0x43,   0x5d,   0x3,    0x2,    0x2,    0x2,    0x44,   0x45, 0x5,  0xe,  0x8,  0x2,  0x45, 0x46,
        0x7,  0x10,   0x2,    0x2,    0x46,   0x47,   0x5,    0xe,    0x8,    0x2,  0x47, 0x5d, 0x3,  0x2,  0x2,  0x2,
        0x48, 0x49,   0x5,    0xe,    0x8,    0x2,    0x49,   0x4a,   0x7,    0x11, 0x2,  0x2,  0x4a, 0x4b, 0x5,  0x10,
        0x9,  0x2,    0x4b,   0x4c,   0x5,    0xe,    0x8,    0x2,    0x4c,   0x5d, 0x3,  0x2,  0x2,  0x2,  0x4d, 0x4e,
        0x5,  0xe,    0x8,    0x2,    0x4e,   0x4f,   0x7,    0x10,   0x2,    0x2,  0x4f, 0x50, 0x5,  0x10, 0x9,  0x2,
        0x50, 0x51,   0x5,    0xe,    0x8,    0x2,    0x51,   0x5d,   0x3,    0x2,  0x2,  0x2,  0x52, 0x53, 0x5,  0xe,
        0x8,  0x2,    0x53,   0x54,   0x7,    0x14,   0x2,    0x2,    0x54,   0x55, 0x5,  0xe,  0x8,  0x2,  0x55, 0x5d,
        0x3,  0x2,    0x2,    0x2,    0x56,   0x57,   0x5,    0xe,    0x8,    0x2,  0x57, 0x58, 0x7,  0x14, 0x2,  0x2,
        0x58, 0x59,   0x5,    0x10,   0x9,    0x2,    0x59,   0x5a,   0x5,    0xe,  0x8,  0x2,  0x5a, 0x5d, 0x3,  0x2,
        0x2,  0x2,    0x5b,   0x5d,   0x5,    0xe,    0x8,    0x2,    0x5c,   0x40, 0x3,  0x2,  0x2,  0x2,  0x5c, 0x44,
        0x3,  0x2,    0x2,    0x2,    0x5c,   0x48,   0x3,    0x2,    0x2,    0x2,  0x5c, 0x4d, 0x3,  0x2,  0x2,  0x2,
        0x5c, 0x52,   0x3,    0x2,    0x2,    0x2,    0x5c,   0x56,   0x3,    0x2,  0x2,  0x2,  0x5c, 0x5b, 0x3,  0x2,
        0x2,  0x2,    0x5d,   0xd,    0x3,    0x2,    0x2,    0x2,    0x5e,   0x5f, 0x7,  0xc,  0x2,  0x2,  0x5f, 0x98,
        0x5,  0x12,   0xa,    0x2,    0x60,   0x61,   0x7,    0xd,    0x2,    0x2,  0x61, 0x98, 0x5,  0x12, 0xa,  0x2,
        0x62, 0x63,   0x7,    0x12,   0x2,    0x2,    0x63,   0x98,   0x5,    0x12, 0xa,  0x2,  0x64, 0x65, 0x7,  0x13,
        0x2,  0x2,    0x65,   0x98,   0x5,    0x12,   0xa,    0x2,    0x66,   0x67, 0x7,  0xe,  0x2,  0x2,  0x67, 0x98,
        0x5,  0x12,   0xa,    0x2,    0x68,   0x69,   0x7,    0xf,    0x2,    0x2,  0x69, 0x98, 0x5,  0x12, 0xa,  0x2,
        0x6a, 0x6b,   0x7,    0xc,    0x2,    0x2,    0x6b,   0x6c,   0x5,    0x10, 0x9,  0x2,  0x6c, 0x6d, 0x5,  0x12,
        0xa,  0x2,    0x6d,   0x98,   0x3,    0x2,    0x2,    0x2,    0x6e,   0x6f, 0x7,  0xd,  0x2,  0x2,  0x6f, 0x70,
        0x5,  0x10,   0x9,    0x2,    0x70,   0x71,   0x5,    0x12,   0xa,    0x2,  0x71, 0x98, 0x3,  0x2,  0x2,  0x2,
        0x72, 0x73,   0x7,    0x12,   0x2,    0x2,    0x73,   0x74,   0x5,    0x10, 0x9,  0x2,  0x74, 0x75, 0x5,  0x12,
        0xa,  0x2,    0x75,   0x98,   0x3,    0x2,    0x2,    0x2,    0x76,   0x77, 0x7,  0x13, 0x2,  0x2,  0x77, 0x78,
        0x5,  0x10,   0x9,    0x2,    0x78,   0x79,   0x5,    0x12,   0xa,    0x2,  0x79, 0x98, 0x3,  0x2,  0x2,  0x2,
        0x7a, 0x7b,   0x7,    0xe,    0x2,    0x2,    0x7b,   0x7c,   0x5,    0x10, 0x9,  0x2,  0x7c, 0x7d, 0x5,  0x12,
        0xa,  0x2,    0x7d,   0x98,   0x3,    0x2,    0x2,    0x2,    0x7e,   0x7f, 0x7,  0xf,  0x2,  0x2,  0x7f, 0x80,
        0x5,  0x10,   0x9,    0x2,    0x80,   0x81,   0x5,    0x12,   0xa,    0x2,  0x81, 0x98, 0x3,  0x2,  0x2,  0x2,
        0x82, 0x83,   0x7,    0x15,   0x2,    0x2,    0x83,   0x98,   0x5,    0x12, 0xa,  0x2,  0x84, 0x85, 0x7,  0x15,
        0x2,  0x2,    0x85,   0x86,   0x5,    0x10,   0x9,    0x2,    0x86,   0x87, 0x5,  0x12, 0xa,  0x2,  0x87, 0x98,
        0x3,  0x2,    0x2,    0x2,    0x88,   0x89,   0x7,    0xb,    0x2,    0x2,  0x89, 0x98, 0x5,  0x12, 0xa,  0x2,
        0x8a, 0x8b,   0x7,    0xb,    0x2,    0x2,    0x8b,   0x98,   0x5,    0x4,  0x3,  0x2,  0x8c, 0x98, 0x5,  0x12,
        0xa,  0x2,    0x8d,   0x8e,   0x7,    0x1c,   0x2,    0x2,    0x8e,   0x93, 0x7,  0x1b, 0x2,  0x2,  0x8f, 0x90,
        0x7,  0x3,    0x2,    0x2,    0x90,   0x92,   0x7,    0x1b,   0x2,    0x2,  0x91, 0x8f, 0x3,  0x2,  0x2,  0x2,
        0x92, 0x95,   0x3,    0x2,    0x2,    0x2,    0x93,   0x91,   0x3,    0x2,  0x2,  0x2,  0x93, 0x94, 0x3,  0x2,
        0x2,  0x2,    0x94,   0x96,   0x3,    0x2,    0x2,    0x2,    0x95,   0x93, 0x3,  0x2,  0x2,  0x2,  0x96, 0x98,
        0x7,  0x4,    0x2,    0x2,    0x97,   0x5e,   0x3,    0x2,    0x2,    0x2,  0x97, 0x60, 0x3,  0x2,  0x2,  0x2,
        0x97, 0x62,   0x3,    0x2,    0x2,    0x2,    0x97,   0x64,   0x3,    0x2,  0x2,  0x2,  0x97, 0x66, 0x3,  0x2,
        0x2,  0x2,    0x97,   0x68,   0x3,    0x2,    0x2,    0x2,    0x97,   0x6a, 0x3,  0x2,  0x2,  0x2,  0x97, 0x6e,
        0x3,  0x2,    0x2,    0x2,    0x97,   0x72,   0x3,    0x2,    0x2,    0x2,  0x97, 0x76, 0x3,  0x2,  0x2,  0x2,
        0x97, 0x7a,   0x3,    0x2,    0x2,    0x2,    0x97,   0x7e,   0x3,    0x2,  0x2,  0x2,  0x97, 0x82, 0x3,  0x2,
        0x2,  0x2,    0x97,   0x84,   0x3,    0x2,    0x2,    0x2,    0x97,   0x88, 0x3,  0x2,  0x2,  0x2,  0x97, 0x8a,
        0x3,  0x2,    0x2,    0x2,    0x97,   0x8c,   0x3,    0x2,    0x2,    0x2,  0x97, 0x8d, 0x3,  0x2,  0x2,  0x2,
        0x98, 0xf,    0x3,    0x2,    0x2,    0x2,    0x99,   0x9a,   0x7,    0x5,  0x2,  0x2,  0x9a, 0x9b, 0x5,  0x14,
        0xb,  0x2,    0x9b,   0x9c,   0x7,    0x3,    0x2,    0x2,    0x9c,   0x9d, 0x5,  0x14, 0xb,  0x2,  0x9d, 0x9e,
        0x7,  0x6,    0x2,    0x2,    0x9e,   0xa6,   0x3,    0x2,    0x2,    0x2,  0x9f, 0xa0, 0x7,  0x5,  0x2,  0x2,
        0xa0, 0xa1,   0x5,    0x14,   0xb,    0x2,    0xa1,   0xa2,   0x7,    0x3,  0x2,  0x2,  0xa2, 0xa3, 0x7,  0xa,
        0x2,  0x2,    0xa3,   0xa4,   0x7,    0x6,    0x2,    0x2,    0xa4,   0xa6, 0x3,  0x2,  0x2,  0x2,  0xa5, 0x99,
        0x3,  0x2,    0x2,    0x2,    0xa5,   0x9f,   0x3,    0x2,    0x2,    0x2,  0xa6, 0x11, 0x3,  0x2,  0x2,  0x2,
        0xa7, 0xb2,   0x5,    0x16,   0xc,    0x2,    0xa8,   0xb2,   0x7,    0x1b, 0x2,  0x2,  0xa9, 0xab, 0x7,  0x7,
        0x2,  0x2,    0xaa,   0xac,   0x5,    0x4,    0x3,    0x2,    0xab,   0xaa, 0x3,  0x2,  0x2,  0x2,  0xac, 0xad,
        0x3,  0x2,    0x2,    0x2,    0xad,   0xab,   0x3,    0x2,    0x2,    0x2,  0xad, 0xae, 0x3,  0x2,  0x2,  0x2,
        0xae, 0xaf,   0x3,    0x2,    0x2,    0x2,    0xaf,   0xb0,   0x7,    0x4,  0x2,  0x2,  0xb0, 0xb2, 0x3,  0x2,
        0x2,  0x2,    0xb1,   0xa7,   0x3,    0x2,    0x2,    0x2,    0xb1,   0xa8, 0x3,  0x2,  0x2,  0x2,  0xb1, 0xa9,
        0x3,  0x2,    0x2,    0x2,    0xb2,   0x13,   0x3,    0x2,    0x2,    0x2,  0xb3, 0xb4, 0x7,  0x1e, 0x2,  0x2,
        0xb4, 0x15,   0x3,    0x2,    0x2,    0x2,    0xb5,   0xb8,   0x7,    0x8,  0x2,  0x2,  0xb6, 0xb8, 0x7,  0x9,
        0x2,  0x2,    0xb7,   0xb5,   0x3,    0x2,    0x2,    0x2,    0xb7,   0xb6, 0x3,  0x2,  0x2,  0x2,  0xb8, 0x17,
        0x3,  0x2,    0x2,    0x2,    0xe,    0x23,   0x25,   0x2d,   0x35,   0x3d, 0x5c, 0x93, 0x97, 0xa5, 0xad, 0xb1,
        0xb7,
    };

    _serializedATN.insert(_serializedATN.end(), serializedATNSegment0,
                          serializedATNSegment0 + sizeof(serializedATNSegment0) / sizeof(serializedATNSegment0[0]));

    atn::ATNDeserializer deserializer;
    _atn = deserializer.deserialize(_serializedATN);

    size_t count = _atn.getNumberOfDecisions();
    _decisionToDFA.reserve(count);
    for (size_t i = 0; i < count; i++) {
        _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
    }
}

FormulaParser::Initializer FormulaParser::_init;
