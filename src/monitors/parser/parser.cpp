#include "parser.h"
#include "log_visitor.h"
#include "monitors/parser/gen/FormulaLexer.h"
#include "monitors/parser/gen/FormulaParser.h"
#include "monitors/parser/gen/LogLexer.h"
#include "monitors/parser/gen/LogParser.h"
#include "visitor.h"
#include <ANTLRInputStream.h>

Formula Parser::parse(const std::string &formula, const std::string &formulaId) {

    antlr4::ANTLRInputStream input(formula);
    FormulaLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    tokens.fill();
    FormulaParser parser(&tokens);
    antlr4::tree::ParseTree *treePointer = parser.formula();
    Visitor visitor;
    Formula form{visitor.visit(treePointer).as<Formula>()};
    form.setId(formulaId);
    return form;
}

Trace Parser::parse_log(const std::string &log) {
    antlr4::ANTLRInputStream input(log);
    LogLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    tokens.fill();
    LogParser parser(&tokens);
    antlr4::tree::ParseTree *treePointer = parser.log();
    impl::LogVisitor visitor;
    Trace trace{visitor.visit(treePointer).as<Trace>()};
    return trace;
}
