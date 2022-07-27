//
// Created by Sebastian Maierhofer and Evald Nexhipi.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//

#include "test_mtl_event_consuming.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/mtl_event_consuming//mtl_event_consuming.h"
#include "monitors/trace/trace.h"
#include "monitors/utils/utils.h"

TEST(TestMTL, violated1) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"b"}, {"b"}, {"b"}, {"a"}, {"b"}, {"b"}, {"b"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b & O[0, 6](c)))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, satisfied1) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"b"}, {"a"}, {"b"}, {"c"}, {"b"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b & O[0, 6](c)))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, weakNext) {
    Trace trace = Utils::createTrace({"a"}, {{"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(W(a))");
    auto eventConsuming{MTLEventConsuming({formula})};
    EXPECT_TRUE(eventConsuming.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, satisfied2) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"b"}});
    Formula formula = Parser::parse("G(a -> F[2, 5](b))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, satisfied2_p) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"b"}, {"b"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G[2,4](b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, satisfied2Compound) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G[2,4](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, violated2) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, satisfied3) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, violated3) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, violated4) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> O[0, 4](b))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, satisfied4) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"b"}, {"c"}, {"a"}, {"b"}, {"b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(a -> O[0, 6](b))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, satisfied5) {
    Trace trace = Utils::createTrace({"a"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("O[0, 6](a)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, violated5) {
    Trace trace = Utils::createTrace({"a"}, {{}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("O[0, 6](a)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, violated6) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> P(b))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, satisfied6) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"a"}, {"b"}, {"b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(a -> P(b))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, violated7) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"b"}, {"b"}, {"b"}, {"c"}, {"a"}});
    Formula formula = Parser::parse("G(a -> O(b & P(!b)))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, satisfied7) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"a", "b"}, {"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> O(b & P(!b)))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, violated8) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"b"}, {"a"}, {"c"}});
    Formula formula = Parser::parse("G((a & O(b & P(!b))) -> c)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, satisfied8) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"b"}, {"c", "a"}, {"c"}});
    Formula formula = Parser::parse("G((a & O(b & P(!b))) -> c)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, satisfied9) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G((a & O(b & P(!b))) -> c)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, satisfied10) {
    Trace trace = Utils::createTrace(
        {"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,5](H[0,2](a & b))))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, violated10) {
    Trace trace =
        Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a", "b"}, {"a"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,5](H[0,2](a & b))))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, violated11) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a"}, {"a"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,3](a & b)))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, satisfied11) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("G((a & P[0,1](b)) -> (O[1,3](a & b)))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, satisfied12) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(c -> H[0,1](a & b))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, violated12) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(c -> H[0,1](a & b))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

/*** Until ***/
TEST(TestMTL, violatedSingleUntil1) {
    Trace trace = Utils::createTrace({"a", "b", "c"},
                                     {{"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a"}, {"c"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("(a & b) U[0, 5] (c)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, violatedSingleUntil2) {
    Trace trace = Utils::createTrace(
        {"a", "b", "c"}, {{"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"c"}, {"c"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("(a & b) U[0, 4] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, violatedSingleUntil3) {
    Trace trace = Utils::createTrace({"a", "b", "c"},
                                     {{"a"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"c"}, {"a"}});
    Formula formula = Parser::parse("(a & b) U[0, 5] (c)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, violatedSingleUntil4) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a", "b"}, {"a", "b"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("(a & b) U[0, 3] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, violatedSingleUntil5) {
    Trace trace = Utils::createTrace(
        {"a", "b", "c", "d"},
        {{"d"}, {"d", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"c"}, {"a"}});
    Formula formula = Parser::parse("(a & b) U[0, 3] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, violatedSingleUntil6) {
    Trace trace =
        Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"c"}, {"c"}, {"a"}});
    Formula formula = Parser::parse("(a & b) U[2, 4] (c)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, violatedSingleUntil0) {
    Trace trace = Utils::createTrace(
        {"a", "b", "c"}, {{"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b", "c"}, {"c"}, {"a", "c"}});
    Formula formula = Parser::parse("(a & b) U[2, 4] (c)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, satisfiedSingleUntil2) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("(a)U[0, 4](b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, violated_single_until_7) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a", "b"}, {"a", "b"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("(a & b) U[0, 3] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, violatedSingleUntil8) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("a U[0, 2] b");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, satisfiedSingleUntil3) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("a U[0, 2] b");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, violatedSingleUntil) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"c"}});
    Formula formula = Parser::parse("(a & b) U[1, 3] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, unsatisfiedSingleUntil) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"c"}});
    Formula formula = Parser::parse("(a & b) U[1, 3] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, violatedSingleUntil7) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a", "b"}, {"a"}});
    Formula formula = Parser::parse("(a & b) U[0, 1] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, violatedSingleUntil9) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a", "b"}, {"a"}});
    Formula formula = Parser::parse("(a & b) U[0, 1] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, satisfiedSingleUntilExtra1) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"c"}, {"a", "b"}, {"a", "b"}, {"c"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, unsatisfiedSingleUntilExtra1) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"c"}, {"a", "b"}, {"a", "b"}, {"a", "b"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, unsatisfiedSingleUntilExtra2) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"c"}, {"a", "b"}, {"c"}, {"a", "b"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

/** Until Special cases **/
TEST(TestMTL, unsatisfiedSingleUntilExtra3) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"c"}, {"a", "b"}, {"a", "b"}, {"a", "b"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, satisifedSingleUntilExtra1) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"c"}, {"a", "b"}, {"a", "b"}, {"c"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, satisfiedSingleUntilExtra2) {
    Trace trace = Utils::createTrace(
        {"a", "b", "c"}, {{"c"}, {"c"}, {"a", "b"}, {"a", "b"}, {"c"}, {"a", "b"}, {"a", "b"}, {"a", "b", "c"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, satisfiedSingleUntilExtra3) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a", "b"}, {"a", "b"}, {"c"}, {"a", "b"}, {"a", "b"}, {"c"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, unsatisfiedSingleUntilExtra4) {
    Trace trace =
        Utils::createTrace({"a", "b", "c"}, {{"a", "b"}, {"a", "b"}, {"c"}, {"a", "b"}, {"a", "b"}, {"a", "b"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, satisfiedSingleUntilExtra4) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a", "b"}, {"a", "b"}, {"c"}, {"a", "b"}, {"c"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, satisfiedSingleUntilExtraTest4) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("a U[0, 2] b");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, satisfiedSingleUntilExtra5) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a", "b"}, {"c"}, {"a", "b"}, {"a", "b"}, {"c"}});
    Formula formula = Parser::parse("(a & b) U[0, 4] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, satisfiedSingleUntilExtra6) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a", "b"}, {"c"}, {"c"}, {"a", "b"}, {"a", "b"}, {"c"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, satisfiedSingleUntilExtra7) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a", "b"}, {"c"}, {"a", "b"}, {"c"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, satisfiedSingleUntilExtra8) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a", "b"}, {"c"}, {"c"}, {"a", "b"}, {"a", "b"}, {"c"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] (c)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

/*** Since ***/
TEST(TestMTL, satisfiedSingleSince1) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"b"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("a S[0, 2] b");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, violatedSingleSince1) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"c"}, {"c"}, {"a", "b"}, {"a", "b"}, {"a", "b"}});
    Formula formula = Parser::parse("(a & b) S[0, 2] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, satisfiedSingleSince2) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"c"}, {"c"}, {"a", "b"}, {"a", "b"}, {"a", "b"}});
    Formula formula = Parser::parse("(a & b) S[0, 3] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, violatedSingleSince2) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{}, {"c"}, {"c"}, {"a", "b"}, {"a", "b"}});
    Formula formula = Parser::parse("(a & b) S[0, 2] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, violatedSingleSincePrime) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"c"}, {"c"}, {"a", "b"}, {"a", "b"}});
    Formula formula = Parser::parse("(a & b) S[1, 3] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, satisfiedSinceUntilOr) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse(" (a)S[0, 4](b) | (a)U[0, 4](b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, violatedSinceAsHistorically) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("!(true S(!a))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

/*** Next Strong ***/
TEST(TestMTL, satisfiedSingleNextStrong1) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}});
    Formula formula = Parser::parse("X[0,1](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, violatedSingleNexStrong1) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"b"}, {"b"}});
    Formula formula = Parser::parse("X[0,1](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, violatedSingleNextStrong2) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"b"}, {"b"}, {"a", "b"}, {"b"}});
    Formula formula = Parser::parse("X[0,1](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, violatedSingleNextStrong3) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"b"}, {"b"}, {"a", "b"}});
    Formula formula = Parser::parse("X[0,1](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

/*** Previously ***/
TEST(TestMTL, unsatisfiedSinglePreviously) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"b"}});
    Formula formula = Parser::parse("P[0,1](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, unsatisfiedSinglePreviously2) {
    Trace trace =
        Utils::createTrace({"a", "b", "c"}, {{"b", "c"}, {"a", "c"}, {}, {"a", "b", "c"}, {"b", "c"}, {"a", "b", "c"}});
    Formula formula = Parser::parse("P[0,1](c)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, unsatisfiedSinglePreviouslyNegated) {
    Trace trace =
        Utils::createTrace({"a", "b", "c"}, {{"b", "c"}, {"a", "c"}, {}, {"a", "b", "c"}, {"b", "c"}, {"a", "b", "c"}});
    Formula formula = Parser::parse("P[0,4](!c)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, unsatisfiedSinglePreviouslyNegation) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"a", "b"}, {"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("b & P(!b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, unsatisfiedSinglePreviouslyNegation2) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"a", "b"}, {"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("P(!(b)) & b");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, unsatisfiedNegationAndPrevious) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a"}, {"a"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("!a & P(a)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

/*** Globally ***/
TEST(TestMTL, satisfiedGloballyAsUntil) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("!((true) U (!a))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, satisfiedSingleGlobally1) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}});
    Formula formula = Parser::parse("G[0,2](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, satisfiedSingleGloballyIntervalStartingFromTimeStep2) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {}, {"b"}, {"a", "b"}, {"a", "b"}});
    Formula formula = Parser::parse("G[2,4](b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, unsatisfiedViolatedTwiceGlobally) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"a"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, violatedSingleGlobally1) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"b"}});
    Formula formula = Parser::parse("G[0,2](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value); // violated at last index 4
}

TEST(TESTMTL, violatedSingleExtraTest) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"b"}});
    Formula formula = Parser::parse("true U[0,2] !(a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, violatedSingleGlobally2) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a", "b"}, {"a"}, {"a"}, {"a", "b"}, {"a", "b"}, {"b"}});
    Formula formula = Parser::parse("G[0,2](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, violatedSingleGlobally3) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"a", "b"}, {"a", "b"}, {"b"}});
    Formula formula = Parser::parse("G[0,2](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, violatedSingleGlobally4) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a", "b"}, {"a", "b"}, {"a"}, {"a", "b"}, {"b"}});
    Formula formula = Parser::parse("G[0,2](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, violatedSingleGlobally5) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}});
    Formula formula = Parser::parse("G[0,2](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, satisfiedSGlobally2) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}});
    Formula formula = Parser::parse("G[1,2](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, satisfiedGloballyAsUntil) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}});
    Formula formula = Parser::parse("!((true) U[1,2] (!(a & b)))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, satisfiedGlobally3) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a,"
                                                   "b"},
                                                  {"a", "b"},
                                                  {"a", "b"},
                                                  {"a", "b"},
                                                  {"a", "b"}});
    Formula formula = Parser::parse("G[1,2](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, satisfiedGloballyAsUntil2) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a,"
                                                   "b"},
                                                  {"a", "b"},
                                                  {"a", "b"},
                                                  {"a", "b"},
                                                  {"a", "b"}});
    Formula formula = Parser::parse("!((true) U[1,2] (!(a & b)))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

/*** Future ***/
TEST(TestMTL, satisfiedTestFutureAsUntil) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("true U a");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, satisfiedFutureSimple) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("F(a)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, unsatisfiedSingleFutureWithIntervalVacuous) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}});
    Formula formula = Parser::parse("F[1,3](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, violatedSingleFuture1) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"b"}, {"a"}, {"a", "b"}, {"a", "b"}});
    Formula formula = Parser::parse("F[1,2](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, satisfiedSingleFuture2) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}});
    Formula formula = Parser::parse("F[0,2](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, violatedSingleFuture2) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"b"}, {"a"}, {"a", "b"}, {"a", "b"}});
    Formula formula = Parser::parse("F[0,2](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, satisfiedSingleFuture3) {
    Trace trace =
        Utils::createTrace({"a", "b"}, {{"a"}, {"b"}, {"a"}, {"a", "b"}, {"a", "b"}, {"a"}, {"b"}, {"a"}, {"a", "b"}});
    Formula formula = Parser::parse("F[1,3](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, violatedSingleFuture3) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a"}});
    Formula formula = Parser::parse("F[0,2](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

/*** Once ***/
TEST(TESTMTL, satisfiedSingleOnce1) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("O[0, 6](a)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, violatedSingleOnce1) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("O[0, 6](a)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, violatedSingleOnce2) {
    Trace trace = Utils::createTrace({"a", "b"}, {{}, {"a"}, {"b"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("O[0, 2](a)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, violatedSingleOnce3) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"b"}, {"b"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("O[1, 2](a)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, satisfiedSingleOnce) {
    Trace trace = Utils::createTrace({"a"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("O[0, 6](a)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, violatedSingleOnce) {
    Trace trace = Utils::createTrace({"a"}, {{}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("O[0, 6](a)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, satisfiedOnceInterval) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("O[0, 2](a)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, unsatisfiedOnceInterval) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"b"}, {"b"}, {"b", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("O[0, 2](a)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, unsatisfiedOnceIntervalAnd) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a"}, {"a"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("O[1, 3](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

/*** Historically ***/
TEST(TESTMTL, satisfiedSingleHistorically1) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"b"}});
    Formula formula = Parser::parse("H[0,2](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, violatedSingleHistorically1) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a", "b"}, {"a", "b"}, {}, {"a"}, {"b"}});
    Formula formula = Parser::parse("H[0,2](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, satisfiedSingleHistorically2) {
    Trace trace =
        Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"a", "b"}});
    Formula formula = Parser::parse("H(a)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, unsatisfiedSingleHistorically2) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("H(a)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

/*** Combined **/
TEST(TESTMTL, satisifedSinceUntilOr) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("(a)S[0, 4](b) or (a)U[0, 4](b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, unsatisfiedSinceUntilAnd) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("(a)S[0, 4](b) and (a)U[0, 4](b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMTL, unsatisfiedPreviouslySingleExample) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"b"}, {"a"}, {"b"}, {"b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("P[0,4](!c)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, unsatisfiedGloballyFutureOnceIntervalViolatedOnce) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"b"}, {"b"}, {"b"}, {"a"}, {"b"}, {"b"}, {"b"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b & O[0, 6](c)))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, satisfiedGloballyFutureOnceInterval) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"b"}, {"a"}, {"b"}, {"c"}, {"b"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b & O[0, 6](c)))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, satisfiedGloballyFutureInterval) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, unsatisfiedGloballyFutureInterval) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, unsatisfiedGloballyFutureInterval2) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, satisfiedGloballyOnceInterval) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"b"}, {"c"}, {"a"}, {"b"}, {"b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(a -> O[0, 6](b))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, unsatisfiedGloballyPreviously) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> P(b))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, unsatisfiedGloballyOncePrevious) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"b"}, {"b"}, {"b"}, {"c"}, {"a"}});
    Formula formula = Parser::parse("G(a -> O(b & P(!b)))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, satisfiedGloballyOncePrevious) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"a", "b"}, {"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> O(b & P(!b)))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, unsatisifedGloballyOncePreviously2) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"a", "b"}, {"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("true S (P(!(b)) & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, unsatisfiedGloballyAndOncePreviousImplicationOnce) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"b"}, {"a"}, {"c"}});
    Formula formula = Parser::parse("G((a & O(b & P(!b))) -> c)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, satisfiedGloballyAndOncePreviousImplicationOnce) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"b"}, {"c", "a"}, {"c"}});
    Formula formula = Parser::parse("G((a & O(b & P(!b))) -> c)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, satisfiedGloballyAndOncePreviousImplicationOnce2) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G((a & O(b & P(!b))) -> c)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, satisfiedGloballyAndOnceHistoricallyImplication) {
    Trace trace = Utils::createTrace(
        {"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,5](H[0,2](a & b))))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, unsatisfiedGloballyAndOnceHistoricallyImplication) {
    Trace trace =
        Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a", "b"}, {"a"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,5](H[0,2](a & b))))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, unsatisfiedGloballyAndPreviouslyOnceImplication) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a"}, {"a"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,3](a & b)))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, satisfiedGloballyImplicationOncePrevious) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,3](a & b)))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, satisfiedGloballyImplicationHistorically) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(c -> H[0,1](a & b))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, unsatisfiedGloballyImplicationHistorically) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(c -> H[0,1](a & b)))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, unsatisfiedGloballyNext) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(b -> (X(a)))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

/** Combined 2 **/
TEST(TESTMTL, unsatisfiedSince) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("(a)S[0, 4](b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, satisfiedUntilSingleExample) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("(a)U[0, 4](b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, unsatisfiedUntilAndSince) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("((a)U[0, 4](b)) & a ");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, satisfiedCombinedSinceUntilFormulas) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("(a)S[0, 4](b) or (a)U[0, 4](b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, unsatisfiedNegatedAndOnceInterval) {
    Trace trace = Utils::createTrace({"a", "b"}, {{}, {}, {"a", "b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("a & !O[0,30](b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, unsatisfiedNegatedAndOnceInterval2) {
    Trace trace = Utils::createTrace({"a", "b"}, {{}, {}, {"a", "b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("a & O[0,30](b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, unsatisfiedNegatedAndOnceIntervalAnd1) {
    Trace trace = Utils::createTrace(
        {"in_same_lane", "in_single_lane"},
        {{"in_single_lane"}, {"in_single_lane"}, {"in_same_lane"}, {"in_same_lane"}, {"in_same_lane"}});
    Formula formula = Parser::parse("in_same_lane & !O[0,30](in_same_lane & !(in_single_lane) & P(in_single_lane))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TESTMTL, unsatisfiedComplex) {
    Trace trace =
        Utils::createTrace({"in_same_lane", "in_single_lane", "keeps_safe_distance_prec", "orientation_towards"},
                           {{"in_same_lane", "keeps_safe_distance_prec"},
                            {"in_same_lane", "keeps_safe_distance_prec"},
                            {"in_same_lane", "in_single_lane"},
                            {"in_same_lane", "in_single_lane"},
                            {"in_same_lane", "in_single_lane"}});
    Formula formula = Parser::parse("(in_same_lane & !O[0,30](in_same_lane & !(in_single_lane) & orientation_towards & "
                                    "P(in_single_lane & orientation_towards))) -> keeps_safe_distance_prec");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).value);
}