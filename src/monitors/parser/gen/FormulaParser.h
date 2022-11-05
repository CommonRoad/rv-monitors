
// Generated from Formula.g4 by ANTLR 4.7.2

#pragma once

#include "antlr4-runtime.h"

class FormulaParser : public antlr4::Parser {
  public:
    enum {
        T__0 = 1,
        T__1 = 2,
        T__2 = 3,
        T__3 = 4,
        T__4 = 5,
        T__5 = 6,
        T__6 = 7,
        T__7 = 8,
        T__8 = 9,
        TRUE = 10,
        FALSE = 11,
        INFINITY = 12,
        NOT = 13,
        GLOBALLY = 14,
        FUTURE = 15,
        NEXT_WEAK = 16,
        NEXT_STRONG = 17,
        UNTIL = 18,
        RELEASE = 19,
        ONCE = 20,
        PREVIOUSLY = 21,
        SINCE = 22,
        HISTORICALLY = 23,
        UNIVERSAL = 24,
        EXISTENTIAL = 25,
        QUOTE = 26,
        COUNTING = 27,
        EQUALITY = 28,
        AND = 29,
        OR = 30,
        XOR = 31,
        IMPL = 32,
        EQ = 33,
        IN = 34,
        ATOMIC = 35,
        PREDICATE = 36,
        DOMAIN = 37,
        CONST_VALUE = 38,
        WS = 39,
        DIGIT = 40,
        QUOTE_DIGIT = 41
    };

    enum {
        RuleFormula = 0,
        RuleExpr = 1,
        RuleXor_expr = 2,
        RuleOr_expr = 3,
        RuleAnd_expr = 4,
        RuleRelease_until_expr = 5,
        RuleUnary = 6,
        RuleInterval = 7,
        RuleQuote_interval = 8,
        RuleDomain_vars = 9,
        RuleAtom = 10,
        RuleNumber = 11,
        RuleQuote_number = 12,
        RuleBool_ = 13
    };

    FormulaParser(antlr4::TokenStream *input);
    ~FormulaParser();

    virtual std::string getGrammarFileName() const override;
    virtual const antlr4::atn::ATN &getATN() const override { return _atn; };
    virtual const std::vector<std::string> &getTokenNames() const override {
        return _tokenNames;
    }; // deprecated: use vocabulary instead.
    virtual const std::vector<std::string> &getRuleNames() const override;
    virtual antlr4::dfa::Vocabulary &getVocabulary() const override;

    class FormulaContext;
    class ExprContext;
    class Xor_exprContext;
    class Or_exprContext;
    class And_exprContext;
    class Release_until_exprContext;
    class UnaryContext;
    class IntervalContext;
    class Quote_intervalContext;
    class Domain_varsContext;
    class AtomContext;
    class NumberContext;
    class Quote_numberContext;
    class Bool_Context;

    class FormulaContext : public antlr4::ParserRuleContext {
      public:
        FormulaContext(antlr4::ParserRuleContext *parent, size_t invokingState);
        virtual size_t getRuleIndex() const override;
        ExprContext *expr();

        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    FormulaContext *formula();

    class ExprContext : public antlr4::ParserRuleContext {
      public:
        ExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);

        ExprContext() = default;
        void copyFrom(ExprContext *context);
        using antlr4::ParserRuleContext::copyFrom;

        virtual size_t getRuleIndex() const override;
    };

    class EquivalenceContext : public ExprContext {
      public:
        EquivalenceContext(ExprContext *ctx);

        FormulaParser::ExprContext *left = nullptr;
        antlr4::Token *op = nullptr;
        FormulaParser::ExprContext *right = nullptr;
        std::vector<ExprContext *> expr();
        ExprContext *expr(size_t i);
        antlr4::tree::TerminalNode *EQ();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    class XorExprContext : public ExprContext {
      public:
        XorExprContext(ExprContext *ctx);

        Xor_exprContext *xor_expr();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    class ImplicationContext : public ExprContext {
      public:
        ImplicationContext(ExprContext *ctx);

        FormulaParser::ExprContext *left = nullptr;
        antlr4::Token *op = nullptr;
        FormulaParser::ExprContext *right = nullptr;
        std::vector<ExprContext *> expr();
        ExprContext *expr(size_t i);
        antlr4::tree::TerminalNode *IMPL();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    ExprContext *expr();
    ExprContext *expr(int precedence);
    class Xor_exprContext : public antlr4::ParserRuleContext {
      public:
        Xor_exprContext(antlr4::ParserRuleContext *parent, size_t invokingState);

        Xor_exprContext() = default;
        void copyFrom(Xor_exprContext *context);
        using antlr4::ParserRuleContext::copyFrom;

        virtual size_t getRuleIndex() const override;
    };

    class XorContext : public Xor_exprContext {
      public:
        XorContext(Xor_exprContext *ctx);

        antlr4::Token *op = nullptr;
        std::vector<Or_exprContext *> or_expr();
        Or_exprContext *or_expr(size_t i);
        std::vector<antlr4::tree::TerminalNode *> XOR();
        antlr4::tree::TerminalNode *XOR(size_t i);
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    Xor_exprContext *xor_expr();

    class Or_exprContext : public antlr4::ParserRuleContext {
      public:
        Or_exprContext(antlr4::ParserRuleContext *parent, size_t invokingState);

        Or_exprContext() = default;
        void copyFrom(Or_exprContext *context);
        using antlr4::ParserRuleContext::copyFrom;

        virtual size_t getRuleIndex() const override;
    };

    class OrContext : public Or_exprContext {
      public:
        OrContext(Or_exprContext *ctx);

        antlr4::Token *op = nullptr;
        std::vector<And_exprContext *> and_expr();
        And_exprContext *and_expr(size_t i);
        std::vector<antlr4::tree::TerminalNode *> OR();
        antlr4::tree::TerminalNode *OR(size_t i);
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    Or_exprContext *or_expr();

    class And_exprContext : public antlr4::ParserRuleContext {
      public:
        And_exprContext(antlr4::ParserRuleContext *parent, size_t invokingState);

        And_exprContext() = default;
        void copyFrom(And_exprContext *context);
        using antlr4::ParserRuleContext::copyFrom;

        virtual size_t getRuleIndex() const override;
    };

    class AndContext : public And_exprContext {
      public:
        AndContext(And_exprContext *ctx);

        antlr4::Token *op = nullptr;
        std::vector<Release_until_exprContext *> release_until_expr();
        Release_until_exprContext *release_until_expr(size_t i);
        std::vector<antlr4::tree::TerminalNode *> AND();
        antlr4::tree::TerminalNode *AND(size_t i);
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    And_exprContext *and_expr();

    class Release_until_exprContext : public antlr4::ParserRuleContext {
      public:
        Release_until_exprContext(antlr4::ParserRuleContext *parent, size_t invokingState);

        Release_until_exprContext() = default;
        void copyFrom(Release_until_exprContext *context);
        using antlr4::ParserRuleContext::copyFrom;

        virtual size_t getRuleIndex() const override;
    };

    class SinceIntervalContext : public Release_until_exprContext {
      public:
        SinceIntervalContext(Release_until_exprContext *ctx);

        FormulaParser::UnaryContext *left = nullptr;
        antlr4::Token *op = nullptr;
        FormulaParser::IntervalContext *iv = nullptr;
        FormulaParser::UnaryContext *right = nullptr;
        std::vector<UnaryContext *> unary();
        UnaryContext *unary(size_t i);
        antlr4::tree::TerminalNode *SINCE();
        IntervalContext *interval();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    class ReleaseIntervalContext : public Release_until_exprContext {
      public:
        ReleaseIntervalContext(Release_until_exprContext *ctx);

        FormulaParser::UnaryContext *left = nullptr;
        antlr4::Token *op = nullptr;
        FormulaParser::IntervalContext *iv = nullptr;
        FormulaParser::UnaryContext *right = nullptr;
        std::vector<UnaryContext *> unary();
        UnaryContext *unary(size_t i);
        antlr4::tree::TerminalNode *RELEASE();
        IntervalContext *interval();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    class SinceContext : public Release_until_exprContext {
      public:
        SinceContext(Release_until_exprContext *ctx);

        FormulaParser::UnaryContext *left = nullptr;
        antlr4::Token *op = nullptr;
        FormulaParser::UnaryContext *right = nullptr;
        std::vector<UnaryContext *> unary();
        UnaryContext *unary(size_t i);
        antlr4::tree::TerminalNode *SINCE();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    class UntilIntervalContext : public Release_until_exprContext {
      public:
        UntilIntervalContext(Release_until_exprContext *ctx);

        FormulaParser::UnaryContext *left = nullptr;
        antlr4::Token *op = nullptr;
        FormulaParser::IntervalContext *iv = nullptr;
        FormulaParser::UnaryContext *right = nullptr;
        std::vector<UnaryContext *> unary();
        UnaryContext *unary(size_t i);
        antlr4::tree::TerminalNode *UNTIL();
        IntervalContext *interval();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    class UnaryExprContext : public Release_until_exprContext {
      public:
        UnaryExprContext(Release_until_exprContext *ctx);

        UnaryContext *unary();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    class ReleaseContext : public Release_until_exprContext {
      public:
        ReleaseContext(Release_until_exprContext *ctx);

        FormulaParser::UnaryContext *left = nullptr;
        antlr4::Token *op = nullptr;
        FormulaParser::UnaryContext *right = nullptr;
        std::vector<UnaryContext *> unary();
        UnaryContext *unary(size_t i);
        antlr4::tree::TerminalNode *RELEASE();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    class EqualityOperatorContext : public Release_until_exprContext {
      public:
        EqualityOperatorContext(Release_until_exprContext *ctx);

        antlr4::Token *left = nullptr;
        antlr4::Token *op = nullptr;
        antlr4::Token *right = nullptr;
        antlr4::tree::TerminalNode *EQUALITY();
        std::vector<antlr4::tree::TerminalNode *> ATOMIC();
        antlr4::tree::TerminalNode *ATOMIC(size_t i);
        std::vector<antlr4::tree::TerminalNode *> CONST_VALUE();
        antlr4::tree::TerminalNode *CONST_VALUE(size_t i);
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    class UntilContext : public Release_until_exprContext {
      public:
        UntilContext(Release_until_exprContext *ctx);

        FormulaParser::UnaryContext *left = nullptr;
        antlr4::Token *op = nullptr;
        FormulaParser::UnaryContext *right = nullptr;
        std::vector<UnaryContext *> unary();
        UnaryContext *unary(size_t i);
        antlr4::tree::TerminalNode *UNTIL();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    Release_until_exprContext *release_until_expr();

    class UnaryContext : public antlr4::ParserRuleContext {
      public:
        UnaryContext(antlr4::ParserRuleContext *parent, size_t invokingState);

        UnaryContext() = default;
        void copyFrom(UnaryContext *context);
        using antlr4::ParserRuleContext::copyFrom;

        virtual size_t getRuleIndex() const override;
    };

    class GloballyContext : public UnaryContext {
      public:
        GloballyContext(UnaryContext *ctx);

        antlr4::Token *op = nullptr;
        FormulaParser::AtomContext *right = nullptr;
        antlr4::tree::TerminalNode *GLOBALLY();
        AtomContext *atom();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    class UniversalContext : public UnaryContext {
      public:
        UniversalContext(UnaryContext *ctx);

        antlr4::Token *op = nullptr;
        FormulaParser::Domain_varsContext *dv = nullptr;
        FormulaParser::ExprContext *right = nullptr;
        antlr4::tree::TerminalNode *UNIVERSAL();
        Domain_varsContext *domain_vars();
        ExprContext *expr();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    class OnceContext : public UnaryContext {
      public:
        OnceContext(UnaryContext *ctx);

        antlr4::Token *op = nullptr;
        FormulaParser::AtomContext *right = nullptr;
        antlr4::tree::TerminalNode *ONCE();
        AtomContext *atom();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    class GloballyIntervalContext : public UnaryContext {
      public:
        GloballyIntervalContext(UnaryContext *ctx);

        antlr4::Token *op = nullptr;
        FormulaParser::IntervalContext *iv = nullptr;
        FormulaParser::AtomContext *right = nullptr;
        antlr4::tree::TerminalNode *GLOBALLY();
        IntervalContext *interval();
        AtomContext *atom();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    class PreviouslyIntervalContext : public UnaryContext {
      public:
        PreviouslyIntervalContext(UnaryContext *ctx);

        antlr4::Token *op = nullptr;
        FormulaParser::IntervalContext *iv = nullptr;
        FormulaParser::AtomContext *right = nullptr;
        antlr4::tree::TerminalNode *PREVIOUSLY();
        IntervalContext *interval();
        AtomContext *atom();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    class HistoricallyIntervalContext : public UnaryContext {
      public:
        HistoricallyIntervalContext(UnaryContext *ctx);

        antlr4::Token *op = nullptr;
        FormulaParser::IntervalContext *iv = nullptr;
        FormulaParser::AtomContext *right = nullptr;
        antlr4::tree::TerminalNode *HISTORICALLY();
        IntervalContext *interval();
        AtomContext *atom();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    class PreviouslyContext : public UnaryContext {
      public:
        PreviouslyContext(UnaryContext *ctx);

        antlr4::Token *op = nullptr;
        FormulaParser::AtomContext *right = nullptr;
        antlr4::tree::TerminalNode *PREVIOUSLY();
        AtomContext *atom();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    class NextWeakContext : public UnaryContext {
      public:
        NextWeakContext(UnaryContext *ctx);

        antlr4::Token *op = nullptr;
        FormulaParser::AtomContext *right = nullptr;
        antlr4::tree::TerminalNode *NEXT_WEAK();
        AtomContext *atom();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    class NextWeakIntervalContext : public UnaryContext {
      public:
        NextWeakIntervalContext(UnaryContext *ctx);

        antlr4::Token *op = nullptr;
        FormulaParser::IntervalContext *iv = nullptr;
        FormulaParser::AtomContext *right = nullptr;
        antlr4::tree::TerminalNode *NEXT_WEAK();
        IntervalContext *interval();
        AtomContext *atom();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    class AtomExprContext : public UnaryContext {
      public:
        AtomExprContext(UnaryContext *ctx);

        AtomContext *atom();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    class HistoricallyContext : public UnaryContext {
      public:
        HistoricallyContext(UnaryContext *ctx);

        antlr4::Token *op = nullptr;
        FormulaParser::AtomContext *right = nullptr;
        antlr4::tree::TerminalNode *HISTORICALLY();
        AtomContext *atom();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    class NextStrongIntervalContext : public UnaryContext {
      public:
        NextStrongIntervalContext(UnaryContext *ctx);

        antlr4::Token *op = nullptr;
        FormulaParser::IntervalContext *iv = nullptr;
        FormulaParser::AtomContext *right = nullptr;
        antlr4::tree::TerminalNode *NEXT_STRONG();
        IntervalContext *interval();
        AtomContext *atom();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    class PredicateContext : public UnaryContext {
      public:
        PredicateContext(UnaryContext *ctx);

        antlr4::Token *op = nullptr;
        antlr4::Token *right = nullptr;
        antlr4::tree::TerminalNode *PREDICATE();
        std::vector<antlr4::tree::TerminalNode *> ATOMIC();
        antlr4::tree::TerminalNode *ATOMIC(size_t i);
        std::vector<antlr4::tree::TerminalNode *> CONST_VALUE();
        antlr4::tree::TerminalNode *CONST_VALUE(size_t i);
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    class NextStrongContext : public UnaryContext {
      public:
        NextStrongContext(UnaryContext *ctx);

        antlr4::Token *op = nullptr;
        FormulaParser::AtomContext *right = nullptr;
        antlr4::tree::TerminalNode *NEXT_STRONG();
        AtomContext *atom();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    class QuoteContext : public UnaryContext {
      public:
        QuoteContext(UnaryContext *ctx);

        antlr4::Token *op = nullptr;
        FormulaParser::Quote_intervalContext *piv = nullptr;
        FormulaParser::Domain_varsContext *dv = nullptr;
        FormulaParser::ExprContext *right = nullptr;
        antlr4::tree::TerminalNode *QUOTE();
        Quote_intervalContext *quote_interval();
        Domain_varsContext *domain_vars();
        ExprContext *expr();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    class OnceIntervalContext : public UnaryContext {
      public:
        OnceIntervalContext(UnaryContext *ctx);

        antlr4::Token *op = nullptr;
        FormulaParser::IntervalContext *iv = nullptr;
        FormulaParser::AtomContext *right = nullptr;
        antlr4::tree::TerminalNode *ONCE();
        IntervalContext *interval();
        AtomContext *atom();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    class FutureIntervalContext : public UnaryContext {
      public:
        FutureIntervalContext(UnaryContext *ctx);

        antlr4::Token *op = nullptr;
        FormulaParser::IntervalContext *iv = nullptr;
        FormulaParser::AtomContext *right = nullptr;
        antlr4::tree::TerminalNode *FUTURE();
        IntervalContext *interval();
        AtomContext *atom();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    class NotContext : public UnaryContext {
      public:
        NotContext(UnaryContext *ctx);

        antlr4::Token *op = nullptr;
        FormulaParser::AtomContext *right = nullptr;
        antlr4::tree::TerminalNode *NOT();
        AtomContext *atom();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    class FutureContext : public UnaryContext {
      public:
        FutureContext(UnaryContext *ctx);

        antlr4::Token *op = nullptr;
        FormulaParser::AtomContext *right = nullptr;
        antlr4::tree::TerminalNode *FUTURE();
        AtomContext *atom();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    class ExistentialContext : public UnaryContext {
      public:
        ExistentialContext(UnaryContext *ctx);

        antlr4::Token *op = nullptr;
        FormulaParser::Domain_varsContext *dv = nullptr;
        FormulaParser::ExprContext *right = nullptr;
        antlr4::tree::TerminalNode *EXISTENTIAL();
        Domain_varsContext *domain_vars();
        ExprContext *expr();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    class NotExprContext : public UnaryContext {
      public:
        NotExprContext(UnaryContext *ctx);

        antlr4::Token *op = nullptr;
        FormulaParser::ExprContext *right = nullptr;
        antlr4::tree::TerminalNode *NOT();
        ExprContext *expr();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    class CountingContext : public UnaryContext {
      public:
        CountingContext(UnaryContext *ctx);

        antlr4::Token *op = nullptr;
        antlr4::Token *cop = nullptr;
        antlr4::Token *num = nullptr;
        FormulaParser::Domain_varsContext *dv = nullptr;
        FormulaParser::ExprContext *right = nullptr;
        antlr4::tree::TerminalNode *COUNTING();
        antlr4::tree::TerminalNode *DIGIT();
        Domain_varsContext *domain_vars();
        ExprContext *expr();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    UnaryContext *unary();

    class IntervalContext : public antlr4::ParserRuleContext {
      public:
        IntervalContext(antlr4::ParserRuleContext *parent, size_t invokingState);

        IntervalContext() = default;
        void copyFrom(IntervalContext *context);
        using antlr4::ParserRuleContext::copyFrom;

        virtual size_t getRuleIndex() const override;
    };

    class FiniteIntervalContext : public IntervalContext {
      public:
        FiniteIntervalContext(IntervalContext *ctx);

        FormulaParser::NumberContext *l = nullptr;
        FormulaParser::NumberContext *u = nullptr;
        std::vector<NumberContext *> number();
        NumberContext *number(size_t i);
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    class InfiniteIntervalContext : public IntervalContext {
      public:
        InfiniteIntervalContext(IntervalContext *ctx);

        FormulaParser::NumberContext *l = nullptr;
        antlr4::Token *u = nullptr;
        NumberContext *number();
        antlr4::tree::TerminalNode *INFINITY();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    IntervalContext *interval();

    class Quote_intervalContext : public antlr4::ParserRuleContext {
      public:
        Quote_intervalContext(antlr4::ParserRuleContext *parent, size_t invokingState);

        Quote_intervalContext() = default;
        void copyFrom(Quote_intervalContext *context);
        using antlr4::ParserRuleContext::copyFrom;

        virtual size_t getRuleIndex() const override;
    };

    class FiniteQuoteIntervalContext : public Quote_intervalContext {
      public:
        FiniteQuoteIntervalContext(Quote_intervalContext *ctx);

        FormulaParser::Quote_numberContext *l = nullptr;
        FormulaParser::Quote_numberContext *u = nullptr;
        std::vector<Quote_numberContext *> quote_number();
        Quote_numberContext *quote_number(size_t i);
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    Quote_intervalContext *quote_interval();

    class Domain_varsContext : public antlr4::ParserRuleContext {
      public:
        Domain_varsContext(antlr4::ParserRuleContext *parent, size_t invokingState);

        Domain_varsContext() = default;
        void copyFrom(Domain_varsContext *context);
        using antlr4::ParserRuleContext::copyFrom;

        virtual size_t getRuleIndex() const override;
    };

    class DomainVariablesContext : public Domain_varsContext {
      public:
        DomainVariablesContext(Domain_varsContext *ctx);

        std::vector<antlr4::tree::TerminalNode *> ATOMIC();
        antlr4::tree::TerminalNode *ATOMIC(size_t i);
        std::vector<antlr4::tree::TerminalNode *> IN();
        antlr4::tree::TerminalNode *IN(size_t i);
        std::vector<antlr4::tree::TerminalNode *> DOMAIN();
        antlr4::tree::TerminalNode *DOMAIN(size_t i);
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    Domain_varsContext *domain_vars();

    class AtomContext : public antlr4::ParserRuleContext {
      public:
        AtomContext(antlr4::ParserRuleContext *parent, size_t invokingState);

        AtomContext() = default;
        void copyFrom(AtomContext *context);
        using antlr4::ParserRuleContext::copyFrom;

        virtual size_t getRuleIndex() const override;
    };

    class BracketsContext : public AtomContext {
      public:
        BracketsContext(AtomContext *ctx);

        FormulaParser::ExprContext *content = nullptr;
        std::vector<ExprContext *> expr();
        ExprContext *expr(size_t i);
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    class AtomicContext : public AtomContext {
      public:
        AtomicContext(AtomContext *ctx);

        antlr4::tree::TerminalNode *ATOMIC();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    class BooleanContext : public AtomContext {
      public:
        BooleanContext(AtomContext *ctx);

        Bool_Context *bool_();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    AtomContext *atom();

    class NumberContext : public antlr4::ParserRuleContext {
      public:
        NumberContext(antlr4::ParserRuleContext *parent, size_t invokingState);

        NumberContext() = default;
        void copyFrom(NumberContext *context);
        using antlr4::ParserRuleContext::copyFrom;

        virtual size_t getRuleIndex() const override;
    };

    class DigitContext : public NumberContext {
      public:
        DigitContext(NumberContext *ctx);

        antlr4::tree::TerminalNode *DIGIT();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    NumberContext *number();

    class Quote_numberContext : public antlr4::ParserRuleContext {
      public:
        Quote_numberContext(antlr4::ParserRuleContext *parent, size_t invokingState);

        Quote_numberContext() = default;
        void copyFrom(Quote_numberContext *context);
        using antlr4::ParserRuleContext::copyFrom;

        virtual size_t getRuleIndex() const override;
    };

    class QuoteDigitContext : public Quote_numberContext {
      public:
        QuoteDigitContext(Quote_numberContext *ctx);

        antlr4::tree::TerminalNode *QUOTE_DIGIT();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    Quote_numberContext *quote_number();

    class Bool_Context : public antlr4::ParserRuleContext {
      public:
        Bool_Context(antlr4::ParserRuleContext *parent, size_t invokingState);

        Bool_Context() = default;
        void copyFrom(Bool_Context *context);
        using antlr4::ParserRuleContext::copyFrom;

        virtual size_t getRuleIndex() const override;
    };

    class TrueBooleanContext : public Bool_Context {
      public:
        TrueBooleanContext(Bool_Context *ctx);

        antlr4::tree::TerminalNode *TRUE();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    class FalseBooleanContext : public Bool_Context {
      public:
        FalseBooleanContext(Bool_Context *ctx);

        antlr4::tree::TerminalNode *FALSE();
        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    Bool_Context *bool_();

    virtual bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;
    bool exprSempred(ExprContext *_localctx, size_t predicateIndex);

  private:
    static std::vector<antlr4::dfa::DFA> _decisionToDFA;
    static antlr4::atn::PredictionContextCache _sharedContextCache;
    static std::vector<std::string> _ruleNames;
    static std::vector<std::string> _tokenNames;

    static std::vector<std::string> _literalNames;
    static std::vector<std::string> _symbolicNames;
    static antlr4::dfa::Vocabulary _vocabulary;
    static antlr4::atn::ATN _atn;
    static std::vector<uint16_t> _serializedATN;

    struct Initializer {
        Initializer();
    };
    static Initializer _init;
};
