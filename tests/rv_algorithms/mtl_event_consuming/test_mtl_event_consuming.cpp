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
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, satisfied1) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"b"}, {"a"}, {"b"}, {"c"}, {"b"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b & O[0, 6](c)))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, weakNext) {
    Trace trace = Utils::createTrace({"a"}, {{"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(W(a))");
    auto eventConsuming{MTLEventConsuming({formula})};
    EXPECT_TRUE(eventConsuming.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, satisfied2) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"b"}, {"b"}, {"b"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(a -> F[2, 5](b))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, satisfied2_p) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"b"}, {"b"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G[2,4](b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, satisfied2Compound) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G[2,4](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violated2) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, satisfied3) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violated3) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violated4) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> O[0, 4](b))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, satisfied4) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"b"}, {"c"}, {"a"}, {"b"}, {"b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(a -> O[0, 6](b))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, satisfied5) {
    Trace trace = Utils::createTrace({"a"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("O[0, 6](a)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violated5) {
    Trace trace = Utils::createTrace({"a"}, {{}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("O[0, 6](a)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violated6) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> P(b))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, satisfied6) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"a"}, {"b"}, {"b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(a -> P(b))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violated7) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"b"}, {"b"}, {"b"}, {"c"}, {"a"}});
    Formula formula = Parser::parse("G(a -> O(b & P(!b)))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, satisfied7) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"a", "b"}, {"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> O(b & P(!b)))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violated8) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"b"}, {"a"}, {"c"}});
    Formula formula = Parser::parse("G((a & O(b & P(!b))) -> c)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, satisfied8) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"b"}, {"c", "a"}, {"c"}});
    Formula formula = Parser::parse("G((a & O(b & P(!b))) -> c)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, satisfied9) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G((a & O(b & P(!b))) -> c)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, satisfied10) {
    Trace trace = Utils::createTrace(
        {"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,5](H[0,2](a & b))))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violated10) {
    Trace trace =
        Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a", "b"}, {"a"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,5](H[0,2](a & b))))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violated11) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a"}, {"a"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,3](a & b)))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, satisfied11) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("G((a & P[0,1](b)) -> (O[1,3](a & b)))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, satisfied12) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(c -> H[0,1](a & b))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violated12) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(c -> H[0,1](a & b))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

/*** Until ***/
TEST(TestMTL, violatedSingleUntil1) {
    Trace trace = Utils::createTrace({"a", "b", "c"},
                                     {{"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a"}, {"c"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("(a & b) U[0, 5] (c)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violatedSingleUntil2) {
    Trace trace = Utils::createTrace(
        {"a", "b", "c"}, {{"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"c"}, {"c"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("(a & b) U[0, 4] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violatedSingleUntil3) {
    Trace trace = Utils::createTrace({"a", "b", "c"},
                                     {{"a"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"c"}, {"a"}});
    Formula formula = Parser::parse("(a & b) U[0, 5] (c)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violatedSingleUntil4) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a", "b"}, {"a", "b"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("(a & b) U[0, 3] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violatedSingleUntil5) {
    Trace trace = Utils::createTrace(
        {"a", "b", "c", "d"},
        {{"d"}, {"d", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"c"}, {"a"}});
    Formula formula = Parser::parse("(a & b) U[0, 3] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violatedSingleUntil6) {
    Trace trace =
        Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"c"}, {"c"}, {"a"}});
    Formula formula = Parser::parse("(a & b) U[2, 4] (c)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violatedSingleUntil0) {
    Trace trace = Utils::createTrace(
        {"a", "b", "c"}, {{"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b", "c"}, {"c"}, {"a", "c"}});
    Formula formula = Parser::parse("(a & b) U[2, 4] (c)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, satisfiedSingleUntil2) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("(a)U[0, 4](b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violated_single_until_7) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a", "b"}, {"a", "b"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("(a & b) U[0, 3] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violatedSingleUntil8) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("a U[0, 2] b");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, satisfiedSingleUntil3) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a", "b"}, {"a", "b"}, {"c"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violatedSingleUntil) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"c"}});
    Formula formula = Parser::parse("(a & b) U[1, 3] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, unsatisfiedSingleUntil) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"c"}});
    Formula formula = Parser::parse("(a & b) U[1, 3] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violatedSingleUntil7) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a", "b"}, {"a"}});
    Formula formula = Parser::parse("(a & b) U[0, 1] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violatedSingleUntil9) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a", "b"}, {"a"}});
    Formula formula = Parser::parse("(a & b) U[0, 1] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, satisfiedSingleUntilExtra1) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"c"}, {"a", "b"}, {"a", "b"}, {"c"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, unsatisfiedSingleUntilExtra1) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"c"}, {"a", "b"}, {"a", "b"}, {"a", "b"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, unsatisfiedSingleUntilExtra2) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"c"}, {"a", "b"}, {"c"}, {"a", "b"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

/** Until Special cases **/
TEST(TestMTL, unsatisfiedSingleUntilExtra3) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"c"}, {"a", "b"}, {"a", "b"}, {"a", "b"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, satisifedSingleUntilExtra1) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"c"}, {"a", "b"}, {"a", "b"}, {"c"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, satisfiedSingleUntilExtra2) {
    Trace trace = Utils::createTrace(
        {"a", "b", "c"}, {{"c"}, {"c"}, {"a", "b"}, {"a", "b"}, {"c"}, {"a", "b"}, {"a", "b"}, {"a", "b", "c"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, satisfiedSingleUntilExtra3) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a", "b"}, {"a", "b"}, {"c"}, {"a", "b"}, {"a", "b"}, {"c"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, unsatisfiedSingleUntilExtra4) {
    Trace trace =
        Utils::createTrace({"a", "b", "c"}, {{"a", "b"}, {"a", "b"}, {"c"}, {"a", "b"}, {"a", "b"}, {"a", "b"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b); // false 2,4
}

TEST(TestMTL, satisfiedSingleUntilExtra4) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a", "b"}, {"a", "b"}, {"c"}, {"a", "b"}, {"c"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b); // false 2,4
}

TEST(TestMTL, satisfiedSingleUntilExtra5) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a", "b"}, {"c"}, {"a", "b"}, {"a", "b"}, {"c"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, satisfiedSingleUntilExtra6) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a", "b"}, {"c"}, {"c"}, {"a", "b"}, {"a", "b"}, {"c"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, satisfiedSingleUntilExtra7) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a", "b"}, {"c"}, {"a", "b"}, {"c"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, satisfiedSingleUntilExtra8) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a", "b"}, {"c"}, {"c"}, {"a", "b"}, {"a", "b"}, {"c"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] (c)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

/*** Since ***/
TEST(TestMTL, satisfiedSingleSince1) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"c"}, {"c"}, {"a", "b"}, {"a", "b"}});
    Formula formula = Parser::parse("(a & b) S[0, 2] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violatedSingleSince1) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"c"}, {"c"}, {"a", "b"}, {"a", "b"}, {"a", "b"}});
    Formula formula = Parser::parse("(a & b) S[0, 2] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, satisfiedSingleSince2) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"c"}, {"c"}, {"a", "b"}, {"a", "b"}, {"a", "b"}});
    Formula formula = Parser::parse("(a & b) S[0, 3] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violatedSingleSince2) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{}, {"c"}, {"c"}, {"a", "b"}, {"a", "b"}});
    Formula formula = Parser::parse("(a & b) S[0, 2] c");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, satisfiedSinceUntilOr) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse(" (a)S[0, 4](b) | (a)U[0, 4](b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violatedSinceAsHistorically) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("!(true S(!a))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

/*** Next Strong ***/
TEST(TestMTL, satisfiedSingleNextStrong1) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}});
    Formula formula = Parser::parse("X[0,1](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violatedSingleNexStrong1) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"b"}, {"b"}});
    Formula formula = Parser::parse("X[0,1](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violatedSingleNextStrong2) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"b"}, {"b"}, {"a", "b"}, {"b"}});
    Formula formula = Parser::parse("X[0,1](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violatedSingleNextStrong3) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"b"}, {"b"}, {"a", "b"}});
    Formula formula = Parser::parse("X[0,1](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

/*** Previously ***/
TEST(TestMTL, unsatisfiedSinglePreviously) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"b"}});
    Formula formula = Parser::parse("P[0,1](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, unsatisfiedSinglePreviously2) {
    Trace trace =
        Utils::createTrace({"a", "b", "c"}, {{"b", "c"}, {"a", "c"}, {}, {"a", "b", "c"}, {"b", "c"}, {"a", "b", "c"}});
    Formula formula = Parser::parse("P[0,1](c)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, unsatisfiedSinglePreviouslyNegated) {
    Trace trace =
        Utils::createTrace({"a", "b", "c"}, {{"b", "c"}, {"a", "c"}, {}, {"a", "b", "c"}, {"b", "c"}, {"a", "b", "c"}});
    Formula formula = Parser::parse("P[0,4](!c)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, unsatisfiedSinglePreviouslyNegation) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"a", "b"}, {"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("b & P(!b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, unsatisfiedSinglePreviouslyNegation2) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"a", "b"}, {"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("P(!(b)) & b");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, unsatisfiedNegationAndPrevious) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a"}, {"a"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("!a & P(a)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

/*** Globally ***/
TEST(TestMTL, satisfiedGloballyAsUntil) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("!((true) U (!a))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, satisfiedSingleGlobally1) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}});
    Formula formula = Parser::parse("G[0,2](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, satisfiedSingleGloballyIntervalStartingFromTimeStep2) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"},{}, {"b"}, {"a", "b"}, {"a", "b"}});
    Formula formula = Parser::parse("G[2,4](b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsatisfiedViolatedTwiceGlobally) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"a"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, violatedSingleGlobally1) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"b"}});
    Formula formula = Parser::parse("G[0,2](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b); // violated at last index 4
}

TEST(TESTMTL, violatedSingleExtraTest) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"b"}});
    Formula formula = Parser::parse("true U[0,2] !(a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, violatedSingleGlobally2) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a", "b"}, {"a"}, {"a"}, {"a", "b"}, {"a", "b"}, {"b"}});
    Formula formula = Parser::parse("G[0,2](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, violatedSingleGlobally3) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"a", "b"}, {"a", "b"}, {"b"}});
    Formula formula = Parser::parse("G[0,2](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, violatedSingleGlobally4) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a", "b"}, {"a", "b"}, {"a"}, {"a", "b"}, {"b"}});
    Formula formula = Parser::parse("G[0,2](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, violatedSingleGlobally5) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}});
    Formula formula = Parser::parse("G[0,2](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, satisfiedSGlobally2) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}});
    Formula formula = Parser::parse("G[1,2](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, satisfiedGloballyAsUntil) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}});
    Formula formula = Parser::parse("!((true) U[1,2] (!(a & b)))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, satisfiedGlobally3) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a,""b"},{"a", "b"},{"a", "b"},{"a", "b"},{"a", "b"}});
    Formula formula = Parser::parse("G[1,2](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, satisfiedGloballyAsUntil2) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a,""b"},{"a", "b"},{"a", "b"},{"a", "b"},{"a", "b"}});
    Formula formula = Parser::parse("!((true) U[1,2] (!(a & b)))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

/*** Future ***/
TEST(TestMTL, satisfiedTestFutureAsUntil) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("true U a");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, satisfiedFutureSimple) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("F(a)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsatisfiedSingleFuture) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}});
    Formula formula = Parser::parse("F[1,3](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, violatedSingleFuture1) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"b"}, {"a"}, {"a", "b"}, {"a", "b"}});
    Formula formula = Parser::parse("F[1,2](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, satisfiedSingleFuture2) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}});
    Formula formula = Parser::parse("F[0,2](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, violatedSingleFuture2) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"b"}, {"a"}, {"a", "b"}, {"a", "b"}});
    Formula formula = Parser::parse("F[0,2](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, satisfiedSingleFuture3) {
    Trace trace =
        Utils::createTrace({"a", "b"}, {{"a"}, {"b"}, {"a"}, {"a", "b"}, {"a", "b"}, {"a"}, {"b"}, {"a"}, {"a", "b"}});
    Formula formula = Parser::parse("F[1,3](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, violatedSingleFuture3) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a"}});
    Formula formula = Parser::parse("F[0,2](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

/*** Once ***/
TEST(TESTMTL, satisfiedSingleOnce1) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("O[0, 6](a)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, violatedSingleOnce1) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("O[0, 6](a)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, violatedSingleOnce2) {
    Trace trace = Utils::createTrace({"a", "b"}, {{}, {"a"}, {"b"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("O[0, 2](a)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, violatedSingleOnce3) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"b"}, {"b"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("O[1, 2](a)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, satisfiedSingleOnce) {
    Trace trace = Utils::createTrace({"a"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("O[0, 6](a)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, violatedSingleOnce) {
    Trace trace = Utils::createTrace({"a"}, {{}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("O[0, 6](a)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, satisfiedOnceInterval) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("O[0, 2](a)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsatisfiedOnceInterval) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"b"}, {"b"}, {"b", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("O[0, 2](a)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsatisfiedOnceIntervalAnd) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a"}, {"a"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("O[1, 3](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

/*** Historically ***/
TEST(TESTMTL, satisfiedSingleHistorically1) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"b"}});
    Formula formula = Parser::parse("H[0,2](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, violatedSingleHistorically1) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a", "b"}, {"a", "b"}, {}, {"a"}, {"b"}});
    Formula formula = Parser::parse("H[0,2](a & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, satisfiedSingleHistorically2) {
    Trace trace =
        Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"a", "b"}});
    Formula formula = Parser::parse("H(a)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsatisfiedSingleHistorically2) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("H(a)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

/*** Combined **/
TEST(TESTMTL, satisifedSinceUntilOr) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("(a)S[0, 4](b) or (a)U[0, 4](b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsatisfiedSinceUntilAnd) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("(a)S[0, 4](b) and (a)U[0, 4](b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, unsatisfiedPreviouslySingleExample) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"b"}, {"a"}, {"b"}, {"b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("P[0,4](!c)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsatisfiedGloballyFutureOnceIntervalViolatedOnce) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"b"}, {"b"}, {"b"}, {"a"}, {"b"}, {"b"}, {"b"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b & O[0, 6](c)))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, satisfiedGloballyFutureOnceInterval) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"b"}, {"a"}, {"b"}, {"c"}, {"b"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b & O[0, 6](c)))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, satisfiedGloballyFutureInterval) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsatisfiedGloballyFutureInterval) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsatisfiedGloballyFutureInterval2) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, satisfiedGloballyOnceInterval) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"b"}, {"c"}, {"a"}, {"b"}, {"b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(a -> O[0, 6](b))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsatisfiedGloballyPreviously) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> P(b))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsatisfiedGloballyOncePrevious) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"b"}, {"b"}, {"b"}, {"c"}, {"a"}});
    Formula formula = Parser::parse("G(a -> O(b & P(!b)))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, satisfiedGloballyOncePrevious) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"a", "b"}, {"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> O(b & P(!b)))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsatisifedGloballyOncePreviously2) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"a", "b"}, {"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("true S (P(!(b)) & b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsatisfiedGloballyAndOncePreviousImplicationOnce) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"b"}, {"a"}, {"c"}});
    Formula formula = Parser::parse("G((a & O(b & P(!b))) -> c)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, satisfiedGloballyAndOncePreviousImplicationOnce) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"b"}, {"c", "a"}, {"c"}});
    Formula formula = Parser::parse("G((a & O(b & P(!b))) -> c)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, satisfiedGloballyAndOncePreviousImplicationOnce2) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G((a & O(b & P(!b))) -> c)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, satisfiedGloballyAndOnceHistoricallyImplication) {
    Trace trace = Utils::createTrace(
        {"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,5](H[0,2](a & b))))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsatisfiedGloballyAndOnceHistoricallyImplication) {
    Trace trace =
        Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a", "b"}, {"a"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,5](H[0,2](a & b))))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsatisfiedGloballyAndPreviouslyOnceImplication) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a"}, {"a"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,3](a & b)))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, satisfiedGloballyImplicationOncePrevious) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,3](a & b)))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, satisfiedGloballyImplicationHistorically) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(c -> H[0,1](a & b))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsatisfiedGloballyImplicationHistorically) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(c -> H[0,1](a & b)))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsatisfiedGloballyNext) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(b -> (X(a)))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

/** Combined 1 **/
TEST(TESTMTL, unsatisfiedSince) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("(a)S[0, 4](b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, satisfiedUntilSingleExample) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("(a)U[0, 4](b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsatisfiedUntilAndSince) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("((a)U[0, 4](b)) and a");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, satisfiedCombinedSinceUntilFormulas) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("(a)S[0, 4](b) or (a)U[0, 4](b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsatisfiedNegatedAndOnceInterval) {
    Trace trace = Utils::createTrace({"a", "b"}, {{}, {}, {"a", "b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("a & !O[0,30](b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsatisfiedNegatedAndOnceInterval2) {
    Trace trace = Utils::createTrace({"a", "b"}, {{}, {}, {"a", "b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("a & O[0,30](b)");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsatisfiedNegatedAndOnceIntervalAnd1) {
    Trace trace = Utils::createTrace(
        {"in_same_lane", "in_single_lane"},
        {{"in_single_lane"}, {"in_single_lane"}, {"in_same_lane"}, {"in_same_lane"}, {"in_same_lane"}});
    Formula formula = Parser::parse("in_same_lane & !O[0,30](in_same_lane & !(in_single_lane) & P(in_single_lane))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsatisfiedNegatedAndOnceIntervalAnd2) {
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
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, largeTraceSizeOneHundred) {
    Trace trace =
        Utils::createTrace({"a","b","c","p","q"},
                           {
                               {"p","q"},{"a","p","q"},{"p","q"},{"p","q"},{"p","q"},{"p","q"},{"a","p","q"},{"p","q"},{"p","q"},{"p","q"},
                               {"p","q"},{"a","p","q"},{"p","q","b"},{"p","q"},{"p","q"},{"p","q"},{"a","p","q"},{"p","q"},{"p","q"},{"p","q"},
                               {"p","q"},{"a","p","q"},{"p","q","c"},{"p","q"},{"p","q"},{"p","q"},{"a","p","q"},{"p","q"},{"p","q"},{"p","q"},
                               {"p","q"},{"a","p","q"},{"p","q","d"},{"p","q"},{"p","q"},{"p","q"},{"a","p","q"},{"p","q"},{"p","q"},{"p","q"},
                               {"p","q"},{"a","p","q"},{"p","q"},{"p","q"},{"p","q"},{"p","q"},{"a","p","q"},{"p","q"},{"p","q"},{"p","q"},
                               {"p","q"},{"a","p","q"},{"p","q"},{"p","q"},{"p"},{"p","q"},{"a","p","q"},{"p","q"},{"p","q"},{"p","q"},
                               {"p","q"},{"a","p","q"},{"p","q"},{"p","q"},{"p","q"},{"p","q"},{"a","p","q"},{"p","q"},{"p","q"},{"p","q"},
                               {"p","q"},{"a","p","q"},{"p","q"},{"p","q"},{"p","q"},{"p","q"},{"a","p","q"},{"p","q"},{"p","q"},{"p","q"},
                               {"p","q"},{"a","p","q"},{"p","q"},{"p","q"},{"p","q"},{"p","q"},{"a","p","q"},{"p","q"},{"p","q"},{"p","q"},
                               {"p","q"},{"a","p","q"},{"p","q"},{"p","q"},{"p","q"},{"p","q"},{"a","p","q"},{"p","q"},{"p","q"},{"p","q"}
                           });
    Formula formula = Parser::parse("((((P(p) -> b) U c) -> q) -> ((q U p) & c))");
    auto monitor{MTLEventConsuming({formula})};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}