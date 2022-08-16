
// Generated from Log.g4 by ANTLR 4.7.2

#pragma once


#include "antlr4-runtime.h"
#include "LogParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by LogParser.
 */
class  LogVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by LogParser.
   */
    virtual antlrcpp::Any visitLog(LogParser::LogContext *context) = 0;

    virtual antlrcpp::Any visitEntry(LogParser::EntryContext *context) = 0;

    virtual antlrcpp::Any visitIdentifier(LogParser::IdentifierContext *context) = 0;

    virtual antlrcpp::Any visitTime(LogParser::TimeContext *context) = 0;


};

