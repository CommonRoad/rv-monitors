
// Generated from Log.g4 by ANTLR 4.7.2

#pragma once


#include "antlr4-runtime.h"
#include "LogVisitor.h"


/**
 * This class provides an empty implementation of LogVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  LogBaseVisitor : public LogVisitor {
public:

  virtual antlrcpp::Any visitLog(LogParser::LogContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitEntry(LogParser::EntryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIdentifier(LogParser::IdentifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTime(LogParser::TimeContext *ctx) override {
    return visitChildren(ctx);
  }


};

