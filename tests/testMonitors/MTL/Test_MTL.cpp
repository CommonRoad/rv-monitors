//
// Created by Sebastian Maierhofer on 15.01.21.
//

#include "Test_MTL.h"
#include <RV-Algorithms/MTL/MTLEventConsuming.h>
#include "Parser/Parser.h"
#include "Trace/Trace.h"
#include "Utils/Utils.h"

TEST(TestMTL, violated1) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {
            {"b"},
            {"b"},
            {"b"},
            {"a"},
            {"b"},
            {"b"},
            {"b"}});
    Formula formula = Parser::parse("G[0, 6](a -> F[0, 6](b & O[0, 6](c)))");

    EXPECT_FALSE(MTLEventConsuming::eventConsuming(formula, trace));
}

TEST(TestMTL, satisfied1) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {
            {"c"},
            {"b"},
            {"b"},
            {"a"},
            {"b"},
            {"c"},
            {"b"}});
    Formula formula = Parser::parse("G[0, 6](a -> F[0, 6](b & O[0, 6](c)))");

    EXPECT_TRUE(MTLEventConsuming::eventConsuming(formula, trace));
}

TEST(TestMTL, satisfied2) {
    Trace trace = Utils::createTrace({"a", "b"}, {
            {"b"},
            {"a"},
            {"a"},
            {"a"},
            {"a"},
            {"a"},
            {"b"}});
    Formula formula = Parser::parse("G[0, 6](a -> F[0, 6](b))");

    EXPECT_TRUE(MTLEventConsuming::eventConsuming(formula, trace));
}

TEST(TestMTL, violated2) {
    Trace trace = Utils::createTrace({"a", "b"}, {
            {"b"},
            {"a"},
            {"a"},
            {"a"},
            {"a"},
            {"a"},
            {"a"}});
    Formula formula = Parser::parse("G[0, 6](a -> F[0, 6](b))");

    EXPECT_FALSE(MTLEventConsuming::eventConsuming(formula, trace));
}

TEST(TestMTL, satisfied3) {
    Trace trace = Utils::createTrace({"a", "b"}, {
            {"a"},
            {"a"},
            {"a"},
            {"a"},
            {"a"},
            {"a"},
            {"a"}});
    Formula formula = Parser::parse("G[0, 6](a)");

    EXPECT_TRUE(MTLEventConsuming::eventConsuming(formula, trace));
}

TEST(TestMTL, violated3) {
    Trace trace = Utils::createTrace({"a", "b"}, {
            {"b"},
            {"a"},
            {"a"},
            {"a"},
            {"a"},
            {"a"},
            {"a"}});
    Formula formula = Parser::parse("G[0, 6](a)");

    EXPECT_FALSE(MTLEventConsuming::eventConsuming(formula, trace));
}

TEST(TestMTL, violated4) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {
            {"c"},
            {"a"},
            {"b"},
            {"a"},
            {"a"}});
    Formula formula = Parser::parse("G[0, 4](a -> O[0, 4](b))");

    EXPECT_FALSE(MTLEventConsuming::eventConsuming(formula, trace));
}

TEST(TestMTL, satisfied4) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {
            {"b"},
            {"c"},
            {"a"},
            {"b"},
            {"b"},
            {"a"},
            {"b"}});
    Formula formula = Parser::parse("G[0, 6](a -> O[0, 6](b))");

    EXPECT_TRUE(MTLEventConsuming::eventConsuming(formula, trace));
}

TEST(TestMTL, satisfied5) {
    Trace trace = Utils::createTrace({"a"}, {
            {"a"},
            {"a"},
            {"a"},
            {"a"},
            {"a"},
            {"a"},
            {"a"}});
    Formula formula = Parser::parse("O[0, 6](a)");

    EXPECT_TRUE(MTLEventConsuming::eventConsuming(formula, trace));
}

TEST(TestMTL, violated5) {
    Trace trace = Utils::createTrace({"a"}, {
            {},
            {"a"},
            {"a"},
            {"a"},
            {"a"},
            {"a"},
            {"a"}});
    Formula formula = Parser::parse("O[0, 6](a)");

    EXPECT_FALSE(MTLEventConsuming::eventConsuming(formula, trace));
}

