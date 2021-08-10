//
// Created by Sebastian Maierhofer on 15.01.21.
//

#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/mtl_event_consuming/mtl_event_consuming.h"
#include "monitors/trace/trace.h"
#include "monitors/utils/utils.h"
#include "test_mtl_rewriting.h"

TEST(TestMTLEventConsuming, violated1) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"b"}, {"b"}, {"b"}, {"a"}, {"b"}, {"b"}, {"b"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b & O[0, 6](c)))");

    EXPECT_FALSE(MTLEventConsuming::eventConsuming(formula, trace));
}

TEST(TestMTLEventConsuming, satisfied1) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"b"}, {"a"}, {"b"}, {"c"}, {"b"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b & O[0, 6](c)))");

    EXPECT_TRUE(MTLEventConsuming::eventConsuming(formula, trace));
}

TEST(TestMTLEventConsuming, satisfied2) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b))");

    EXPECT_TRUE(MTLEventConsuming::eventConsuming(formula, trace));
}

TEST(TestMTLEventConsuming, violated2) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b))");

    EXPECT_FALSE(MTLEventConsuming::eventConsuming(formula, trace));
}

TEST(TestMTLEventConsuming, satisfied3) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a)");

    EXPECT_TRUE(MTLEventConsuming::eventConsuming(formula, trace));
}

TEST(TestMTLEventConsuming, violated3) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a)");

    EXPECT_FALSE(MTLEventConsuming::eventConsuming(formula, trace));
}

TEST(TestMTLEventConsuming, violated4) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> O[0, 4](b))");

    EXPECT_FALSE(MTLEventConsuming::eventConsuming(formula, trace));
}

TEST(TestMTLEventConsuming, satisfied4) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"b"}, {"c"}, {"a"}, {"b"}, {"b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(a -> O[0, 6](b))");

    EXPECT_TRUE(MTLEventConsuming::eventConsuming(formula, trace));
}

TEST(TestMTLEventConsuming, satisfied5) {
    Trace trace = Utils::createTrace({"a"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("O[0, 6](a)");

    EXPECT_TRUE(MTLEventConsuming::eventConsuming(formula, trace));
}

TEST(TestMTLEventConsuming, violated5) {
    Trace trace = Utils::createTrace({"a"}, {{}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("O[0, 6](a)");

    EXPECT_FALSE(MTLEventConsuming::eventConsuming(formula, trace));
}

TEST(TestMTLEventConsuming, violated6) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> P(b))");

    EXPECT_FALSE(MTLEventConsuming::eventConsuming(formula, trace));
}

TEST(TestMTLEventConsuming, satisfied6) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"a"}, {"b"}, {"b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(a -> P(b))");

    EXPECT_TRUE(MTLEventConsuming::eventConsuming(formula, trace));
}

TEST(TestMTLEventConsuming, violated7) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"b"}, {"b"}, {"b"}, {"c"}, {"a"}});
    Formula formula = Parser::parse("G(a -> O(b & P(!b)))");

    EXPECT_FALSE(MTLEventConsuming::eventConsuming(formula, trace));
}

TEST(TestMTLEventConsuming, satisfied7) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"a", "b"}, {"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> O(b & P(!b)))");

    EXPECT_TRUE(MTLEventConsuming::eventConsuming(formula, trace));
}

TEST(TestMTLEventConsuming, violated8) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"b"}, {"a"}, {"c"}});
    Formula formula = Parser::parse("G((a & O(b & P(!b))) -> c)");

    EXPECT_FALSE(MTLEventConsuming::eventConsuming(formula, trace));
}

TEST(TestMTLEventConsuming, satisfied8) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"b"}, {"c", "a"}, {"c"}});
    Formula formula = Parser::parse("G((a & O(b & P(!b))) -> c)");

    EXPECT_TRUE(MTLEventConsuming::eventConsuming(formula, trace));
}

TEST(TestMTLEventConsuming, satisfied9) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G((a & O(b & P(!b))) -> c)");

    EXPECT_TRUE(MTLEventConsuming::eventConsuming(formula, trace));
}

TEST(TestMTLEventConsuming, satisfied10) {
    Trace trace = Utils::createTrace(
        {"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,5](H[0,2](a & b))))");

    EXPECT_TRUE(MTLEventConsuming::eventConsuming(formula, trace));
}

TEST(TestMTLEventConsuming, violated10) {
    Trace trace =
        Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a", "b"}, {"a"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,5](H[0,2](a & b))))");

    EXPECT_FALSE(MTLEventConsuming::eventConsuming(formula, trace));
}

TEST(TestMTLEventConsuming, violated11) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a"}, {"a"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,3](a & b)))");

    EXPECT_FALSE(MTLEventConsuming::eventConsuming(formula, trace));
}

TEST(TestMTLEventConsuming, satisfied11) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,3](a & b)))");

    EXPECT_TRUE(MTLEventConsuming::eventConsuming(formula, trace));
}

TEST(TestMTLEventConsuming, satisfied12) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(c -> H[0,1](a & b))");

    EXPECT_TRUE(MTLEventConsuming::eventConsuming(formula, trace));
}

TEST(TestMTLEventConsuming, violated12) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(c -> H[0,1](a & b))");

    EXPECT_FALSE(MTLEventConsuming::eventConsuming(formula, trace));
}