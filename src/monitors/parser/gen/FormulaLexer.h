
// Generated from Formula.g4 by ANTLR 4.7.2

#pragma once


#include "antlr4-runtime.h"




class  FormulaLexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, TRUE = 7, 
    FALSE = 8, INFINITY = 9, NOT = 10, GLOBALLY = 11, FUTURE = 12, NEXT_WEAK = 13, 
    NEXT_STRONG = 14, UNTIL = 15, RELEASE = 16, ONCE = 17, PREVIOUSLY = 18, 
    SINCE = 19, HISTORICALLY = 20, UNIVERSAL = 21, EXISTENTIAL = 22, COUNTING = 23, 
    AND = 24, OR = 25, XOR = 26, IMPL = 27, EQ = 28, SAME = 29, IN = 30, 
    ATOMIC = 31, PREDICATE = 32, DOMAIN = 33, CONST_VALUE = 34, WS = 35, 
    DIGIT = 36, PROB_DIGIT = 37
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

