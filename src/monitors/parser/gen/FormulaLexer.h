
// Generated from Formula.g4 by ANTLR 4.9.3

#pragma once

#include "antlr4-runtime.h"

class FormulaLexer : public antlr4::Lexer {
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

    explicit FormulaLexer(antlr4::CharStream *input);
    ~FormulaLexer();

    virtual std::string getGrammarFileName() const override;
    virtual const std::vector<std::string> &getRuleNames() const override;

    virtual const std::vector<std::string> &getChannelNames() const override;
    virtual const std::vector<std::string> &getModeNames() const override;
    virtual const std::vector<std::string> &getTokenNames() const override; // deprecated, use vocabulary instead
    virtual antlr4::dfa::Vocabulary &getVocabulary() const override;

    virtual const std::vector<uint16_t> getSerializedATN() const override;
    virtual const antlr4::atn::ATN &getATN() const override;

  private:
    static std::vector<antlr4::dfa::DFA> _decisionToDFA;
    static antlr4::atn::PredictionContextCache _sharedContextCache;
    static std::vector<std::string> _ruleNames;
    static std::vector<std::string> _tokenNames;
    static std::vector<std::string> _channelNames;
    static std::vector<std::string> _modeNames;

    static std::vector<std::string> _literalNames;
    static std::vector<std::string> _symbolicNames;
    static antlr4::dfa::Vocabulary _vocabulary;
    static antlr4::atn::ATN _atn;
    static std::vector<uint16_t> _serializedATN;

    // Individual action functions triggered by action() above.

    // Individual semantic predicate functions triggered by sempred() above.

    struct Initializer {
        Initializer();
    };
    static Initializer _init;
};
