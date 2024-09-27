#include "monitors/parser/parser.h"
#include "monitors/parser/log_visitor.h"
#include "monitors/parser/visitor.h"
#include <ANTLRInputStream.h>
#include <monitors/parser/generated/FormulaGrammarParser/FormulaLexer.h>
#include <monitors/parser/generated/FormulaGrammarParser/FormulaParser.h>
#include <monitors/parser/generated/LogGrammarParser/LogLexer.h>
#include <monitors/parser/generated/LogGrammarParser/LogParser.h>

Formula Parser::parse(const std::string &formula, const std::string &formulaId) {

    antlr4::ANTLRInputStream input(formula);
    FormulaLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    tokens.fill();
    FormulaParser parser(&tokens);
    antlr4::tree::ParseTree *treePointer = parser.formula();
    Visitor visitor;
    Formula form{std::any_cast<Formula>(visitor.visit(treePointer))};
    form.setId(formulaId);
    return form;
}

std::shared_ptr<Trace> Parser::parse_log(const std::string &log) {
    antlr4::ANTLRInputStream input(log);
    LogLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    tokens.fill();
    LogParser parser(&tokens);
    antlr4::tree::ParseTree *treePointer = parser.log();
    impl::LogVisitor visitor;
    return std::any_cast<std::shared_ptr<Trace>>(visitor.visit(treePointer));
}
