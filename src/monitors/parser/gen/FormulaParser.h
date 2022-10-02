
// Generated from Formula.g4 by ANTLR 4.9.3

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
        TRUE = 6,
        FALSE = 7,
        INFINITY = 8,
        NOT = 9,
        GLOBALLY = 10,
        FUTURE = 11,
        NEXT_WEAK = 12,
        NEXT_STRONG = 13,
        UNTIL = 14,
        RELEASE = 15,
        ONCE = 16,
        PREVIOUSLY = 17,
        SINCE = 18,
        HISTORICALLY = 19,
        AND = 20,
        OR = 21,
        XOR = 22,
        IMPL = 23,
        EQ = 24,
        ATOMIC = 25,
        PREDICATE = 26,
        INTERVAL_FUNCTION = 27,
        WS = 28,
        DIGIT = 29
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
        RuleAtom = 8,
        RuleNumber = 9,
        RuleInterval_trigger_bool = 10,
        RuleInterval_trigger_function = 11,
        RuleBool_ = 12
    };

    explicit FormulaParser(antlr4::TokenStream *input);
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
    class AtomContext;
    class NumberContext;
    class Interval_trigger_boolContext;
    class Interval_trigger_functionContext;
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

    class NotExprContext : public UnaryContext {
      public:
        NotExprContext(UnaryContext *ctx);

        antlr4::Token *op = nullptr;
        FormulaParser::ExprContext *right = nullptr;
        antlr4::tree::TerminalNode *NOT();
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

    class DynamicIntervalContext : public IntervalContext {
      public:
        DynamicIntervalContext(IntervalContext *ctx);

        FormulaParser::Interval_trigger_boolContext *l = nullptr;
        FormulaParser::Interval_trigger_boolContext *u = nullptr;
        std::vector<Interval_trigger_boolContext *> interval_trigger_bool();
        Interval_trigger_boolContext *interval_trigger_bool(size_t i);

        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    IntervalContext *interval();

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

    class Interval_trigger_boolContext : public antlr4::ParserRuleContext {
      public:
        Interval_trigger_boolContext(antlr4::ParserRuleContext *parent, size_t invokingState);

        Interval_trigger_boolContext() = default;
        void copyFrom(Interval_trigger_boolContext *context);
        using antlr4::ParserRuleContext::copyFrom;

        virtual size_t getRuleIndex() const override;
    };

    class PredicateTriggerContext : public Interval_trigger_boolContext {
      public:
        PredicateTriggerContext(Interval_trigger_boolContext *ctx);

        antlr4::Token *op = nullptr;
        antlr4::Token *right = nullptr;
        antlr4::tree::TerminalNode *PREDICATE();
        std::vector<antlr4::tree::TerminalNode *> ATOMIC();
        antlr4::tree::TerminalNode *ATOMIC(size_t i);

        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    Interval_trigger_boolContext *interval_trigger_bool();

    class Interval_trigger_functionContext : public antlr4::ParserRuleContext {
      public:
        Interval_trigger_functionContext(antlr4::ParserRuleContext *parent, size_t invokingState);

        Interval_trigger_functionContext() = default;
        void copyFrom(Interval_trigger_functionContext *context);
        using antlr4::ParserRuleContext::copyFrom;

        virtual size_t getRuleIndex() const override;
    };

    class FunctionTriggerContext : public Interval_trigger_functionContext {
      public:
        FunctionTriggerContext(Interval_trigger_functionContext *ctx);

        antlr4::Token *op = nullptr;
        antlr4::Token *right = nullptr;
        antlr4::tree::TerminalNode *INTERVAL_FUNCTION();
        std::vector<antlr4::tree::TerminalNode *> ATOMIC();
        antlr4::tree::TerminalNode *ATOMIC(size_t i);

        virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    Interval_trigger_functionContext *interval_trigger_function();

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
