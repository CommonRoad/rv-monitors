
// Generated from Formula.g4 by ANTLR 4.7.2

#pragma once


#include "antlr4-runtime.h"




class  FormulaLexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, TRUE = 10, FALSE = 11, INFINITY = 12, NOT = 13, 
    GLOBALLY = 14, FUTURE = 15, NEXT_WEAK = 16, NEXT_STRONG = 17, UNTIL = 18, 
    RELEASE = 19, ONCE = 20, PREVIOUSLY = 21, SINCE = 22, HISTORICALLY = 23, 
    UNIVERSAL = 24, EXISTENTIAL = 25, QUOTE = 26, COUNTING = 27, EQUALITY = 28, 
    AND = 29, OR = 30, XOR = 31, IMPL = 32, EQ = 33, IN = 34, ATOMIC = 35, 
    PREDICATE = 36, DOMAIN = 37, CONST_VALUE = 38, WS = 39, DIGIT = 40, 
    QUOTE_DIGIT = 41
  };

  FormulaLexer(antlr4::CharStream *input);
  ~FormulaLexer();

  virtual std::string getGrammarFileName() const override;
  virtual const std::vector<std::string>& getRuleNames() const override;

  virtual const std::vector<std::string>& getChannelNames() const override;
  virtual const std::vector<std::string>& getModeNames() const override;
  virtual const std::vector<std::string>& getTokenNames() const override; // deprecated, use vocabulary instead
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;

  virtual const std::vector<uint16_t> getSerializedATN() const override;
  virtual const antlr4::atn::ATN& getATN() const override;

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

