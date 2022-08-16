
// Generated from Log.g4 by ANTLR 4.7.2


#include "LogVisitor.h"

#include "LogParser.h"


using namespace antlrcpp;
using namespace antlr4;

LogParser::LogParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

LogParser::~LogParser() {
  delete _interpreter;
}

std::string LogParser::getGrammarFileName() const {
  return "Log.g4";
}

const std::vector<std::string>& LogParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& LogParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- LogContext ------------------------------------------------------------------

LogParser::LogContext::LogContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogParser::LogContext::EOF() {
  return getToken(LogParser::EOF, 0);
}

std::vector<LogParser::EntryContext *> LogParser::LogContext::entry() {
  return getRuleContexts<LogParser::EntryContext>();
}

LogParser::EntryContext* LogParser::LogContext::entry(size_t i) {
  return getRuleContext<LogParser::EntryContext>(i);
}

std::vector<tree::TerminalNode *> LogParser::LogContext::ENTRY_SEPARATOR() {
  return getTokens(LogParser::ENTRY_SEPARATOR);
}

tree::TerminalNode* LogParser::LogContext::ENTRY_SEPARATOR(size_t i) {
  return getToken(LogParser::ENTRY_SEPARATOR, i);
}


size_t LogParser::LogContext::getRuleIndex() const {
  return LogParser::RuleLog;
}

antlrcpp::Any LogParser::LogContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogVisitor*>(visitor))
    return parserVisitor->visitLog(this);
  else
    return visitor->visitChildren(this);
}

LogParser::LogContext* LogParser::log() {
  LogContext *_localctx = _tracker.createInstance<LogContext>(_ctx, getState());
  enterRule(_localctx, 0, LogParser::RuleLog);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(8);
    dynamic_cast<LogContext *>(_localctx)->first = entry();
    setState(13);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(9);
        match(LogParser::ENTRY_SEPARATOR);
        setState(10);
        dynamic_cast<LogContext *>(_localctx)->it = entry(); 
      }
      setState(15);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx);
    }
    setState(17);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == LogParser::ENTRY_SEPARATOR) {
      setState(16);
      match(LogParser::ENTRY_SEPARATOR);
    }
    setState(19);
    match(LogParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EntryContext ------------------------------------------------------------------

LogParser::EntryContext::EntryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogParser::TimeContext* LogParser::EntryContext::time() {
  return getRuleContext<LogParser::TimeContext>(0);
}

std::vector<LogParser::IdentifierContext *> LogParser::EntryContext::identifier() {
  return getRuleContexts<LogParser::IdentifierContext>();
}

LogParser::IdentifierContext* LogParser::EntryContext::identifier(size_t i) {
  return getRuleContext<LogParser::IdentifierContext>(i);
}


size_t LogParser::EntryContext::getRuleIndex() const {
  return LogParser::RuleEntry;
}

antlrcpp::Any LogParser::EntryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogVisitor*>(visitor))
    return parserVisitor->visitEntry(this);
  else
    return visitor->visitChildren(this);
}

LogParser::EntryContext* LogParser::entry() {
  EntryContext *_localctx = _tracker.createInstance<EntryContext>(_ctx, getState());
  enterRule(_localctx, 2, LogParser::RuleEntry);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(21);
    time();
    setState(25);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == LogParser::ATOMIC) {
      setState(22);
      identifier();
      setState(27);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IdentifierContext ------------------------------------------------------------------

LogParser::IdentifierContext::IdentifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogParser::IdentifierContext::ATOMIC() {
  return getToken(LogParser::ATOMIC, 0);
}


size_t LogParser::IdentifierContext::getRuleIndex() const {
  return LogParser::RuleIdentifier;
}

antlrcpp::Any LogParser::IdentifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogVisitor*>(visitor))
    return parserVisitor->visitIdentifier(this);
  else
    return visitor->visitChildren(this);
}

LogParser::IdentifierContext* LogParser::identifier() {
  IdentifierContext *_localctx = _tracker.createInstance<IdentifierContext>(_ctx, getState());
  enterRule(_localctx, 4, LogParser::RuleIdentifier);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(28);
    match(LogParser::ATOMIC);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TimeContext ------------------------------------------------------------------

LogParser::TimeContext::TimeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogParser::TimeContext::TIMESPEC() {
  return getToken(LogParser::TIMESPEC, 0);
}

tree::TerminalNode* LogParser::TimeContext::DIGIT() {
  return getToken(LogParser::DIGIT, 0);
}


size_t LogParser::TimeContext::getRuleIndex() const {
  return LogParser::RuleTime;
}

antlrcpp::Any LogParser::TimeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogVisitor*>(visitor))
    return parserVisitor->visitTime(this);
  else
    return visitor->visitChildren(this);
}

LogParser::TimeContext* LogParser::time() {
  TimeContext *_localctx = _tracker.createInstance<TimeContext>(_ctx, getState());
  enterRule(_localctx, 6, LogParser::RuleTime);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(30);
    match(LogParser::TIMESPEC);
    setState(31);
    match(LogParser::DIGIT);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

// Static vars and initialization.
std::vector<dfa::DFA> LogParser::_decisionToDFA;
atn::PredictionContextCache LogParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN LogParser::_atn;
std::vector<uint16_t> LogParser::_serializedATN;

std::vector<std::string> LogParser::_ruleNames = {
  "log", "entry", "identifier", "time"
};

std::vector<std::string> LogParser::_literalNames = {
  "", "", "", "", "", "", "'\u0040'"
};

std::vector<std::string> LogParser::_symbolicNames = {
  "", "ATOMIC", "PREDICATE", "DIGIT", "ENTRY_SEPARATOR", "WS", "TIMESPEC"
};

dfa::Vocabulary LogParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> LogParser::_tokenNames;

LogParser::Initializer::Initializer() {
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

  _serializedATN = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
    0x3, 0x8, 0x24, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 0x9, 
    0x4, 0x4, 0x5, 0x9, 0x5, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x7, 0x2, 0xe, 
    0xa, 0x2, 0xc, 0x2, 0xe, 0x2, 0x11, 0xb, 0x2, 0x3, 0x2, 0x5, 0x2, 0x14, 
    0xa, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x3, 0x3, 0x3, 0x7, 0x3, 0x1a, 0xa, 
    0x3, 0xc, 0x3, 0xe, 0x3, 0x1d, 0xb, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x5, 
    0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x2, 0x2, 0x6, 0x2, 0x4, 0x6, 0x8, 0x2, 
    0x2, 0x2, 0x22, 0x2, 0xa, 0x3, 0x2, 0x2, 0x2, 0x4, 0x17, 0x3, 0x2, 0x2, 
    0x2, 0x6, 0x1e, 0x3, 0x2, 0x2, 0x2, 0x8, 0x20, 0x3, 0x2, 0x2, 0x2, 0xa, 
    0xf, 0x5, 0x4, 0x3, 0x2, 0xb, 0xc, 0x7, 0x6, 0x2, 0x2, 0xc, 0xe, 0x5, 
    0x4, 0x3, 0x2, 0xd, 0xb, 0x3, 0x2, 0x2, 0x2, 0xe, 0x11, 0x3, 0x2, 0x2, 
    0x2, 0xf, 0xd, 0x3, 0x2, 0x2, 0x2, 0xf, 0x10, 0x3, 0x2, 0x2, 0x2, 0x10, 
    0x13, 0x3, 0x2, 0x2, 0x2, 0x11, 0xf, 0x3, 0x2, 0x2, 0x2, 0x12, 0x14, 
    0x7, 0x6, 0x2, 0x2, 0x13, 0x12, 0x3, 0x2, 0x2, 0x2, 0x13, 0x14, 0x3, 
    0x2, 0x2, 0x2, 0x14, 0x15, 0x3, 0x2, 0x2, 0x2, 0x15, 0x16, 0x7, 0x2, 
    0x2, 0x3, 0x16, 0x3, 0x3, 0x2, 0x2, 0x2, 0x17, 0x1b, 0x5, 0x8, 0x5, 
    0x2, 0x18, 0x1a, 0x5, 0x6, 0x4, 0x2, 0x19, 0x18, 0x3, 0x2, 0x2, 0x2, 
    0x1a, 0x1d, 0x3, 0x2, 0x2, 0x2, 0x1b, 0x19, 0x3, 0x2, 0x2, 0x2, 0x1b, 
    0x1c, 0x3, 0x2, 0x2, 0x2, 0x1c, 0x5, 0x3, 0x2, 0x2, 0x2, 0x1d, 0x1b, 
    0x3, 0x2, 0x2, 0x2, 0x1e, 0x1f, 0x7, 0x3, 0x2, 0x2, 0x1f, 0x7, 0x3, 
    0x2, 0x2, 0x2, 0x20, 0x21, 0x7, 0x8, 0x2, 0x2, 0x21, 0x22, 0x7, 0x5, 
    0x2, 0x2, 0x22, 0x9, 0x3, 0x2, 0x2, 0x2, 0x5, 0xf, 0x13, 0x1b, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

LogParser::Initializer LogParser::_init;
