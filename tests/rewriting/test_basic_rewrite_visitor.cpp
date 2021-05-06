#include "test_basic_rewrite_visitor.h"
#include "monitors/parser/parser.h"
#include "monitors/rewriting/basic_rewrite_visitor.h"

TEST(TestBasicRewriteVisitor, absorption) {
    auto absorption = Parser::parse("!a & (b | !a)");
    BasicRewriteVisitor rw;
    rw.rewrite(absorption);
    EXPECT_EQ(absorption.to_string(), "!(a)");
}

TEST(TestBasicRewriteVisitor, absorption1) {
    auto testNot = Parser::parse("(a & b & c) | (a & b & c & d) | (a & b & c & e)");
    BasicRewriteVisitor rw;
    rw.rewrite(testNot);
    EXPECT_EQ(testNot.to_string(), "(a & b & c)");
}

TEST(TestBasicRewriteVisitor, absorption2) {
    auto absorption = Parser::parse("(a & b & c & d) or (a & b & d)");
    BasicRewriteVisitor rw;
    rw.rewrite(absorption);
    EXPECT_EQ(absorption.to_string(), "(a & b & d)");
}

TEST(TestBasicRewriteVisitor, absorption3) {
    auto absorption = Parser::parse("a & b & c & (a|b) & (a|c)");
    BasicRewriteVisitor rw;
    rw.rewrite(absorption);
    EXPECT_EQ(absorption.to_string(), "(a & b & c)");
}

TEST(TestBasicRewriteVisitor, absorption4) {
    auto absorption = Parser::parse("(a U b) & ((a U b) | c)");
    BasicRewriteVisitor rw;
    rw.rewrite(absorption);
    EXPECT_EQ(absorption.to_string(), "(a U b)");
}

TEST(TestBasicRewriteVisitor, reduceXor) {
    auto xorFormula = Parser::parse("a ^ b ^ c ^ (a | b)");
    BasicRewriteVisitor rw;
    rw.rewrite(xorFormula);
    EXPECT_EQ(xorFormula.to_string(), "((a | b) ^ a ^ b ^ c)");
}

TEST(TestBasicRewriteVisitor, moveNary) {
    auto xorFormula = Parser::parse("a & b & (c & d)");
    BasicRewriteVisitor rw;
    rw.rewrite(xorFormula);
    EXPECT_EQ(xorFormula.to_string(), "(a & b & c & d)");
}

TEST(TestBasicRewriteVisitor, testNot) {
    auto testNot = Parser::parse("a | b | !a | c");
    BasicRewriteVisitor rw;
    rw.rewrite(testNot);
    EXPECT_EQ(testNot.to_string(), "true");
}
