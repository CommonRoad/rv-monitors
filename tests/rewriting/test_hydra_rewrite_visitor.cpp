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

TEST(TestHydraRewriteVisitor, multipleCounting) {
    auto formula = Parser::parse("C[0.2, 0.8] x in D0. C[0.2, 0.8] y in D1. x & y");
    HydraRewriteVisitor visitor;
    visitor.rewrite(formula);
    EXPECT_EQ(formula.to_string(), "C[0.200000, 0.800000] x in D0, y in D1. (x & y)");
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

    auto formula_3 = Parser::parse("A x in D0. E y in D1. C[0.1, 0.3] x, y in D3. x & y");
    HydraRewriteVisitor visitor_3;
    visitor_3.rewrite(formula_3);
    EXPECT_EQ(formula_3.to_string(), "C[0.100000, 0.300000] x in D3, y in D3. (x & y)");
}
