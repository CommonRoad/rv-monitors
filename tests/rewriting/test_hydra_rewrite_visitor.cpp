#include "test_hydra_rewrite_visitor.h"
#include "monitors/parser/parser.h"
#include "monitors/rewriting/hydra_rewrite_visitor.h"

TEST(TestHydraRewriteVisitor, multipleUniversal) {
    auto formula = Parser::parse("A x in D0. A y in D1. A z in D2. x & y & z");
    HydraRewriteVisitor visitor;
    visitor.rewrite(formula);
    EXPECT_EQ(formula.to_string(), "A x in D0, y in D1, z in D2. (x & y & z)");
}

TEST(TestHydraRewriteVisitor, multipleExistential) {
    auto formula = Parser::parse("E x in D0. E y in D1. E z in D2. x & y & z");
    HydraRewriteVisitor visitor;
    visitor.rewrite(formula);
    EXPECT_EQ(formula.to_string(), "E x in D0, y in D1, z in D2. (x & y & z)");
}

TEST(TestHydraRewriteVisitor, multipleQuote) {
    auto formula = Parser::parse("Q[0.2, 0.8] x in D0. Q[0.2, 0.8] y in D1. x & y");
    HydraRewriteVisitor visitor;
    visitor.rewrite(formula);
    EXPECT_EQ(formula.to_string(), "Q[0.200000, 0.800000] x in D0, y in D1. (x & y)");
}

TEST(TestHydraRewriteVisitor, multipleCounting) {
    auto formula = Parser::parse("C=2 x in D0. C=2 x in D1. x");
    HydraRewriteVisitor visitor;
    visitor.rewrite(formula);
    EXPECT_EQ(formula.to_string(), "C=2 x in D1. x");
}

TEST(TestHydraRewriteVisitor, shadowedVarsFirstOrder) {
    auto formula_1 = Parser::parse("A x, y in D0. E x in D1. x & y");
    HydraRewriteVisitor visitor_1;
    visitor_1.rewrite(formula_1);
    EXPECT_EQ(formula_1.to_string(), "A y in D0. E x in D1. (x & y)");

    auto formula_2 = Parser::parse("A x in D0. E x in D1. x");
    HydraRewriteVisitor visitor_2;
    visitor_2.rewrite(formula_2);
    EXPECT_EQ(formula_2.to_string(), "E x in D1. x");

    auto formula_3 = Parser::parse("A x in D0. E y in D1. Q[0.1, 0.3] x, y in D3. x & y");
    HydraRewriteVisitor visitor_3;
    visitor_3.rewrite(formula_3);
    EXPECT_EQ(formula_3.to_string(), "Q[0.100000, 0.300000] x in D3, y in D3. (x & y)");
}

TEST(TestHydraRewriteVisitor, unusedVarsFirstOrder) {
    auto formula_1 = Parser::parse("A x, y, z in D. ((a)U(x & z))");
    HydraRewriteVisitor visitor_1;
    visitor_1.rewrite(formula_1);
    EXPECT_EQ(formula_1.to_string(), "A x in D, z in D. (a U (x & z))");

    auto formula_2 = Parser::parse("A x in D. ((a)U(A y in D. y))");
    HydraRewriteVisitor visitor_2;
    visitor_2.rewrite(formula_2);
    EXPECT_EQ(formula_2.to_string(), "(a U A y in D. y)");

    auto formula_3 = Parser::parse("E x, y, z in D. (X(y & z)U(C=2 y in D. y))");
    HydraRewriteVisitor visitor_3;
    visitor_3.rewrite(formula_3);
    EXPECT_EQ(formula_3.to_string(), "E y in D, z in D. (X((y & z)) U C=2 y in D. y)");
}

TEST(TestHydraRewriteVisitor, quantifierUnaryTempOperator) {
    auto formula_1 = Parser::parse("A x in D. X(x)");
    HydraRewriteVisitor visitor_1;
    visitor_1.rewrite(formula_1);
    EXPECT_EQ(formula_1.to_string(), "X(A x in D. x)");

    auto formula_2 = Parser::parse("E x in D. G[0, 10](x)");
    HydraRewriteVisitor visitor_2;
    visitor_2.rewrite(formula_2);
    EXPECT_EQ(formula_2.to_string(), "!((true U[0, 10] !(E x in D. x)))");

    auto formula_3 = Parser::parse("C=2 x in D. X((x)S(x))");
    HydraRewriteVisitor visitor_3;
    visitor_3.rewrite(formula_3);
    EXPECT_EQ(formula_3.to_string(), "X(C=2 x in D. (x S x))");

    auto formula_4 = Parser::parse("C=2 x in D. P(P(x))");
    HydraRewriteVisitor visitor_4;
    visitor_4.rewrite(formula_4);
    EXPECT_EQ(formula_4.to_string(), "P(P(C=2 x in D. x))");

    auto formula_5 = Parser::parse("A x in D. E y in D. P(x U y)");
    HydraRewriteVisitor visitor_5;
    visitor_5.rewrite(formula_5);
    EXPECT_EQ(formula_5.to_string(), "P(A x in D. E y in D. (x U y))");

    auto formula_6 = Parser::parse("Q[0., 0.5] x in D. F(F(x))");
    HydraRewriteVisitor visitor_6;
    visitor_6.rewrite(formula_6);
    EXPECT_EQ(formula_6.to_string(), "(true U (true U Q[0.000000, 0.500000] x in D. x))");
}