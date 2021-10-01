#include "parser.h"
#include "monitors/parser/gen/FormulaLexer.h"
#include "monitors/parser/gen/FormulaParser.h"
#include "visitor.h"
#include <ANTLRInputStream.h>

Formula Parser::parse(const std::string &formula, const std::string& formulaId) {

    antlr4::ANTLRInputStream input(formula);
    FormulaLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    tokens.fill();
    FormulaParser parser(&tokens);
    antlr4::tree::ParseTree *treePointer = parser.formula();
    Visitor visitor;
    Formula fm{visitor.visit(treePointer).as<Formula>()};
    fm.setId(formulaId);
    return fm;
}
