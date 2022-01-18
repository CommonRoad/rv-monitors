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
#include "monitors/parser/visitor.h"
#include <ANTLRInputStream.h>

TEST(TestMTL, violated1) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"b"}, {"b"}, {"b"}, {"a"}, {"b"}, {"b"}, {"b"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b & O[0, 6](c)))");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

 TEST(TestMTL, satisfied1) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"b"}, {"a"}, {"b"}, {"c"}, {"b"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b & O[0, 6](c)))");
    auto monitor{MTLEventConsuming(formula)};
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
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b); // should be EXPECT_FALSE?
}

TEST(TestMTL, satisfied2_p) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"b"}, {"b"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G[2,4](b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b); //should be false acc to hydra
}

TEST(TestMTL, satisfied2_x) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"b"}, {"a","b"}, {"a","b"}, {"a","b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G[2,4](a & b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b); //should be false acc to hydra
}

 TEST(TestMTL, violated2) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b))");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

 TEST(TestMTL, satisfied3) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

 TEST(TestMTL, violated3) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

 TEST(TestMTL, violated4) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> O[0, 4](b))");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

 TEST(TestMTL, satisfied4) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"b"}, {"c"}, {"a"}, {"b"}, {"b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(a -> O[0, 6](b))");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

 TEST(TestMTL, satisfied5) {
    Trace trace = Utils::createTrace({"a"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("O[0, 6](a)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

 TEST(TestMTL, violated5) {
    Trace trace = Utils::createTrace({"a"}, {{}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("O[0, 6](a)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

 TEST(TestMTL, violated6) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> P(b))");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

 TEST(TestMTL, satisfied6) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"a"}, {"b"}, {"b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(a -> P(b))");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

 TEST(TestMTL, violated7) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"b"}, {"b"}, {"b"}, {"c"}, {"a"}});
    Formula formula = Parser::parse("G(a -> O(b & P(!b)))");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

 TEST(TestMTL, satisfied7) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"a", "b"}, {"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> O(b & P(!b)))");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

 TEST(TestMTL, violated8) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"b"}, {"a"}, {"c"}});
    Formula formula = Parser::parse("G((a & O(b & P(!b))) -> c)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

 TEST(TestMTL, satisfied8) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"b"}, {"c", "a"}, {"c"}});
    Formula formula = Parser::parse("G((a & O(b & P(!b))) -> c)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

 TEST(TestMTL, satisfied9) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G((a & O(b & P(!b))) -> c)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

 TEST(TestMTL, satisfied10) {
    Trace trace = Utils::createTrace(
        {"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,5](H[0,2](a & b))))");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

 TEST(TestMTL, violated10) {
    Trace trace =
        Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a", "b"}, {"a"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,5](H[0,2](a & b))))");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

 TEST(TestMTL, violated11) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a"}, {"a"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,3](a & b)))");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

 TEST(TestMTL, satisfied11) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("G((a & P[0,1](b)) -> (O[1,3](a & b)))");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

 TEST(TestMTL, satisfied12) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b"}, {"a", "b", "c"}, {"a"}, {"a"},{"b"}});
    Formula formula = Parser::parse("G(c -> H[0,1](a & b))");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

 TEST(TestMTL, violated12) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(c -> H[0,1](a & b))");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

/*** Until ***/
TEST(TestMTL, violated_single_until_1) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a","b"}, {"a","b"}, {"a","b"}, {"a","b"}, {"a"}, {"c"},{"a"},{"a"}});
    Formula formula = Parser::parse("(a & b) U[0, 5] (c)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violated_single_until_2) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a","b"}, {"a","b"}, {"a","b"}, {"a","b"},{"a","b"},{"c"}, {"c"},{"a"},{"a"}});
    Formula formula = Parser::parse("(a & b) U[0, 4] c");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b); //false at event 0, 7, 8
}

TEST(TestMTL, violated_single_until_3) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a","b"}, {"a","b"}, {"a","b"}, {"a","b"}, {"a","b"},{"c"},{"a"}});
    Formula formula = Parser::parse("(a & b) U[0, 5] (c)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b); //should be false at event 0, 7
}

TEST(TestMTL, violated_single_until_4) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a","b"}, {"a","b"}, {"c"},{"c"}});
    Formula formula = Parser::parse("(a & b) U[0, 3] c");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b); // false at event 0
}

TEST(TestMTL, violated_single_until_5) {
    Trace trace = Utils::createTrace({"a", "b", "c","d"}, {{"d"}, {"d","b"}, {"a","b"}, {"a","b"},{"a","b"},{"a","b"}, {"a","b"},{"c"},{"a"}});
    Formula formula = Parser::parse("(a & b) U[0, 3] c");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violated_single_until_6) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a","b"}, {"a","b"}, {"a","b"}, {"c"},{"c"},{"a"}});
    Formula formula = Parser::parse("(a & b) U[2, 4] (c)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violated_single_until_0) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a","b"}, {"a","b"}, {"a","b"}, {"a","b"}, {"a","b", "c"},{"c"},{"a","c"}});
    Formula formula = Parser::parse("(a & b) U[2, 4] (c)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, satisfied_single_until_2) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("(a)U[0, 4](b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violated_single_until_7) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a","b"}, {"a","b"}, {"c"},{"c"}});
    Formula formula = Parser::parse("(a & b) U[0, 3] c");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b); //false at event 0
}

TEST(TestMTL, violated_single_until_8) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("a U[0, 2] b");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b); // Check this
}

TEST(TestMTL, satisfied_single_until_3) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a","b"}, {"a","b"},{"c"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, satisfied_single_until) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a","b"},{"a","b"},{"a","b"},{"a","b"},{"c"}});
    Formula formula = Parser::parse("(a & b) U[1, 3] c");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, unsatisfied_single_until) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"},{"a","b"},{"a","b"},{"a","b"},{"c"}});
    Formula formula = Parser::parse("(a & b) U[1, 3] c");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b); // fails at index 0, 4
}

TEST(TestMTL, violated_single_until_x) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a","b"},{"a"}});
    Formula formula = Parser::parse("(a & b) U[0, 1] c");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, satisfied_single_until_extra) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a","b"},{"a"}});
    Formula formula = Parser::parse("(a & b) U[0, 1] c");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, sat_special_single_until_extra_1) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"},{"c"},{"a","b"},{"a","b"},{"c"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b); //false at 0,
}

TEST(TestMTL, unsat_special_single_until_extra_1) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"},{"c"},{"a","b"},{"a","b"},{"a","b"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b); // false 2,3,4
}

TEST(TestMTL, unsat_special_single_until_extra_2) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"},{"c"},{"a","b"},{"c"},{"a","b"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b); // false 2,4
}

/** Until Special cases **/
TEST(TestMTL, unsat_special_single_until_1) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"},{"c"},{"a","b"},{"a","b"},{"a","b"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b); // false 2,4
}

TEST(TestMTL, sat_special_single_until_1) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"},{"c"},{"a","b"},{"a","b"},{"c"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, sat_special_single_until_2) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"},{"c"},{"a","b"},{"a","b"},{"c"},{"a","b"},{"a","b"},{"a","b","c"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b); // false 2,4
}

TEST(TestMTL, sat_special_single_until_3) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a","b"},{"a","b"},{"c"},{"a","b"},{"a","b"},{"c"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b); // false 2,4
}

TEST(TestMTL, unsat_special_single_until_2) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a","b"},{"a","b"},{"c"},{"a","b"},{"a","b"},{"a","b"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b); // false 2,4
}

TEST(TestMTL, sat_special_single_until_4) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a","b"},{"a","b"},{"c"},{"a","b"},{"c"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b); // false 2,4
}

TEST(TestMTL, sat_special_single_until_5) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a","b"},{"c"},{"a","b"},{"a","b"},{"c"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b); // false 2,4
}

TEST(TestMTL, unsat_special_single_until_3) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a","b"},{"c"},{"c"},{"a","b"},{"a","b"},{"c"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, sat_special_single_until_6) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a","b"},{"c"},{"a","b"},{"c"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b); // false 2,4
}

TEST(TestMTL, sat_single_until_and) {
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"a", "b"}, {"c"}, {"c"}, {"a", "b"}, {"a", "b"}, {"c"}});
    Formula formula = Parser::parse("(a & b) U[0, 2] (c)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

/*** Since ***/
TEST(TestMTL, satisfied_single_since_1) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"},{"c"},{"c"},{"a","b"},{"a","b"}});
    Formula formula = Parser::parse("(a & b) S[0, 2] c");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violated_single_since_1) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"},{"c"},{"c"},{"a","b"},{"a","b"},{"a","b"}});
    Formula formula = Parser::parse("(a & b) S[0, 2] c");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b); //false at time step 3, 4, 5
}

TEST(TestMTL, satisfied_single_since_2) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"},{"c"},{"c"},{"a","b"},{"a","b"},{"a","b"}});
    Formula formula = Parser::parse("(a & b) S[0, 3] c");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violated_single_since_2) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{},{"c"},{"c"},{"a","b"},{"a","b"}});
    Formula formula = Parser::parse("(a & b) S[0, 2] c");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, sat_since_until_or) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse(" (a)S[0, 4](b) | (a)U[0, 4](b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violated_since_as_historically) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("!(true S(!a))");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

/*** Next Strong ***/   // Considers the absolute case
TEST(TestMTL, satisfied_single_next_strong_1) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"},{"a","b"},{"a","b"},{"a","b"},{"a","b"}});
    Formula formula = Parser::parse("X[0,1](a & b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b); // is this True? Hydra says its false
}

TEST(TestMTL, violated_single_next_strong_1) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"b"},{"b"}});
    Formula formula = Parser::parse("X[0,1](a & b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b); //violates at index 4
}

TEST(TestMTL, violated_single_next_strong_2) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"b"}, {"b"}, {"a", "b"}, {"b"}});
    Formula formula = Parser::parse("X[0,1](a & b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, violated_single_next_strong_3) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a","b"}, {"a","b"}, {"a", "b"},{"b"},{"b"}, {"a","b"}});
    Formula formula = Parser::parse("X[0,1](a & b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b); //violates at index 4, 5
}

/*** Previously ***/
TEST(TestMTL, unsatisfied_single_previousl){
    Trace trace = Utils::createTrace({"a", "b"}, {{"a","b"}, {"a","b"}, {"a","b"}, {"a", "b"},{"b"}});
    Formula formula = Parser::parse("P[0,1](a & b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, unsatisfied_single_previously_1){
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"b", "c"}, {"a", "c"}, {}, {"a", "b", "c"}, {"b", "c"}, {"a", "b", "c"}});
    Formula formula = Parser::parse("P[0,1](c)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b); // should be true?
}

TEST(TestMTL, unsatisfied_single_previously_negated){
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"b", "c"}, {"a", "c"}, {}, {"a", "b", "c"}, {"b", "c"}, {"a", "b", "c"}});
    Formula formula = Parser::parse("P[0,4](!c)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, unsatisfied_single_previously_negation){
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"c"}, {"b"}, {"a", "b"}, {"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("b & P(!b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, unsatisfied_single_previously_negation_2){
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"c"}, {"b"}, {"a", "b"}, {"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("P(!(b)) & b");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, unsatisfied_negation_and_previous){
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"c"}, {"a"}, {"a", "b"}, {"a"}, {"a"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("!a & P(a)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

/*** Globally ***/
TEST(TestMTL, sat_globally_as_until){
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("!((true) U (!a))");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, satisfied_single_globally_1){
    Trace trace = Utils::createTrace({"a", "b"}, {{"a","b"}, {"a","b"}, {"a","b"}, {"a", "b"},{"a","b"}});
    Formula formula = Parser::parse("G[0,2](a & b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsat_viol_twice_globally){
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"a"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsat_viol_twice_globally_x){
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"a"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("true U (!a)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsat_viol_twice_globally_2){
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"a"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("true U a");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, violated_single_globally_1){
    Trace trace = Utils::createTrace({"a", "b"}, {{"a","b"}, {"a","b"}, {"a","b"}, {"a", "b"},{"b"}});
    Formula formula = Parser::parse("G[0,2](a & b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b); // violated at last index 4
}

TEST(TESTMTL, violated_single_globally_1_x){
    Trace trace = Utils::createTrace({"a", "b"}, {{"a","b"}, {"a","b"}, {"a","b"}, {"a", "b"},{"b"}});
    Formula formula = Parser::parse("true U[0,2] !(a & b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b); // violated at last index 4
}

TEST(TESTMTL, violated_single_globally_2){
    Trace trace = Utils::createTrace({"a", "b"}, {{"a","b"}, {"a"}, {"a"},{"a","b"}, {"a", "b"},{"b"}});
    Formula formula = Parser::parse("G[0,2](a & b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b); //violated at index 1
}

TEST(TESTMTL, violated_single_globally_3){
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"a","b"}, {"a", "b"},{"b"}});
    Formula formula = Parser::parse("G[0,2](a & b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b); //violated
}

TEST(TESTMTL, violated_single_globally_4){
    Trace trace = Utils::createTrace({"a", "b"}, {{"a","b"}, {"a","b"}, {"a"}, {"a", "b"},{"b"}});
    Formula formula = Parser::parse("G[0,2](a & b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b); //violated at index 2
}

TEST(TESTMTL, violated_single_globally_5){
    Trace trace = Utils::createTrace({"a","b"}, {{"b"}, {"a","b"}, {"a","b"}, {"a", "b"},{"a","b"}});
    Formula formula = Parser::parse("G[0,2](a & b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b); //violated at index 0
}

TEST(TESTMTL, violated_single_globally_6){
    Trace trace = Utils::createTrace({"a","b"}, {{"b"}, {"a","b"}, {"a","b"}, {"a", "b"},{"a","b"}});
    Formula formula = Parser::parse("G[1,2](a & b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b); // should be true acc to hydra
}

TEST(TESTMTL, violated_single_globally_7){
    Trace trace = Utils::createTrace({"a","b"}, {{"a,""b"}, {"a","b"}, {"a","b"}, {"a", "b"},{"a","b"}});
    Formula formula = Parser::parse("G[1,2](a & b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

/*** Future ***/
TEST(TestMTL, sat_test_future_as_until){
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("true U a");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, sat_future_simple_test){
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("F(a)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsatisfied_single_future_1){
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a","b"}, {"a","b"}, {"a", "b"},{"a","b"}});
    Formula formula = Parser::parse("F[1,3](a & b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, violated_single_future_1){
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"b"}, {"a"}, {"a", "b"},{"a","b"}});
    Formula formula = Parser::parse("F[1,2](a & b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, satisfied_single_future_2){
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"b"}, {"a","b"}, {"a", "b"},{"a","b"}});
    Formula formula = Parser::parse("F[0,2](a & b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b); // should be true acc to Hydra
}

TEST(TESTMTL, violated_single_future_2){
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"b"}, {"a"}, {"a", "b"},{"a","b"}});
    Formula formula = Parser::parse("F[0,2](a & b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, satisfied_single_future_3){
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"b"}, {"a"}, {"a", "b"},{"a","b"},{"a"},{"b"},{"a"},{"a","b"}});
    Formula formula = Parser::parse("F[1,3](a & b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b); // fails at time step 8 acc to Hydra
}

TEST(TESTMTL, violated_single_future_3){
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"b"},{"a","b"}, {"a", "b"},{"a","b"},{"a"}});
    Formula formula = Parser::parse("F[0,2](a & b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b); //violated at index 5
}

/*** Once ***/
TEST(TESTMTL, satisfied_single_once_1){
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("O[0, 6](a)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, violated_single_once_1){
    Trace trace = Utils::createTrace({"a", "b","c"}, {{}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("O[0, 6](a)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, violated_single_once_2){
    Trace trace = Utils::createTrace({"a", "b"}, {{}, {"a"}, {"b"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("O[0, 2](a)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b); //false at index 0
}

TEST(TESTMTL, violated_single_once_3){
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"b"}, {"b"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("O[1, 2](a)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b); //false at index 0
}

TEST(TESTMTL, sat_single_once){
    Trace trace = Utils::createTrace({"a"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("O[0, 6](a)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, violated_single_once){
    Trace trace = Utils::createTrace({"a"}, {{}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("O[0, 6](a)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, sat_once_interval){
    Trace trace = Utils::createTrace({"a","b","c"}, {{"a"}, {"a"}, {"a", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("O[0, 2](a)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsat_once_interval){
    Trace trace = Utils::createTrace({"a","b","c"}, {{"b"}, {"b"}, {"b", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("O[0, 2](a)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsat_once_interval_and){
    Trace trace = Utils::createTrace({"a","b","c"}, {{"c"}, {"a"}, {"a", "b"}, {"a"}, {"a"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("O[1, 3](a & b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

/*** Historically ***/
TEST(TESTMTL, satisfied_single_historically_1){
    Trace trace = Utils::createTrace({"a", "b"}, {{"a","b"}, {"a","b"}, {"a","b"}, {"a", "b"},{"b"}});
    Formula formula = Parser::parse("H[0,2](a & b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b); //maybe false?
}

TEST(TESTMTL, violated_single_historically_1){
    Trace trace = Utils::createTrace({"a", "b"}, {{"a","b"}, {"a","b"}, {}, {"a"},{"b"}});
    Formula formula = Parser::parse("H[0,2](a & b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b); //maybe false at index 2,3,4?
}

TEST(TESTMTL, satisfied_single_historically_2){
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"a", "b"}});
    Formula formula = Parser::parse("H(a)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsat_single_historically_2){
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("H(a)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

/*** Combined **/
TEST(TESTMTL, sat_since_until_or){
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("(a)S[0, 4](b) or (a)U[0, 4](b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsat_since_until_and){
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("(a)S[0, 4](b) and (a)U[0, 4](b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMTL, unsatisfied_previously_single_example){
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"a"}, {"b"}, {"a"}, {"b"}, {"b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("P[0,4](!c)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
} // violated at time step 0

TEST(TESTMTL, unsat_globally_future_once_interval_violated_once){
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"b"}, {"b"}, {"b"}, {"a"}, {"b"}, {"b"}, {"b"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b & O[0, 6](c)))");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, sat_globally_future_once_interval){
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"c"}, {"b"}, {"b"}, {"a"}, {"b"}, {"c"}, {"b"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b & O[0, 6](c)))");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, sat_globally_future_interval){
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b))");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsat_globally_future_interval){
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b))");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsat_globally_future_interval_2){
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b))");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, sat_globally_once_interval){
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"b"}, {"c"}, {"a"}, {"b"}, {"b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(a -> O[0, 6](b))");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsat_globally_prev){
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> P(b))");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsat_globally_once_previous){
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"b"}, {"b"}, {"b"}, {"c"}, {"a"}});
    Formula formula = Parser::parse("G(a -> O(b & P(!b)))");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, sat_globally_once_previous){
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"c"}, {"b"}, {"a", "b"}, {"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> O(b & P(!b)))");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsat_globally_once_previously_2){
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"c"}, {"b"}, {"a", "b"}, {"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("true S (P(!(b)) & b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsat_globally_and_once_previous_inplication_once){
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"c"}, {"b"}, {"b"}, {"a"}, {"c"}});
    Formula formula = Parser::parse("G((a & O(b & P(!b))) -> c)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, sat_globally_and_once_previous_inplication_once){
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"c"}, {"b"}, {"b"}, {"c", "a"}, {"c"}});
    Formula formula = Parser::parse("G((a & O(b & P(!b))) -> c)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, sat_globally_and_once_previous_inplication_once_2){
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G((a & O(b & P(!b))) -> c)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, sat_globally_and_once_historically_inplication){
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"c"}, {"a"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,5](H[0,2](a & b))))");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsat_globally_and_once_historically_implication){
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"c"}, {"a"}, {"a", "b"}, {"a", "b"}, {"a"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,5](H[0,2](a & b))))");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsat_globally_and_previously_once_inplication){
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"c"}, {"a"}, {"a", "b"}, {"a"}, {"a"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,3](a & b)))");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, sat_globally_implication_once_previous){
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"c"}, {"a"}, {"a", "b"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,3](a & b)))");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, sat_globally_implication_historically){
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"a"}, {"a"}, {"a", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(c -> H[0,1](a & b))");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsat_globally_implication_historically){
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"a"}, {"a"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(c -> H[0,1](a & b)))");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsat_globally_next){
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(b -> (X(a)))");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

/** Combined 1 **/
TEST(TESTMTL, unsat_since){
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("(a)S[0, 4](b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, sat_until_single_example){
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("(a)U[0, 4](b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsat_until_and_since){
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("((a)U[0, 4](b)) and a");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, sat_combined_since_until_formulas){
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("(a)S[0, 4](b) or (a)U[0, 4](b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsat_negated_and_once_interval){
    Trace trace = Utils::createTrace({"a", "b"}, {{}, {}, {"a", "b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("a & !O[0,30](b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsat_negated_and_once_interval_2){
    Trace trace = Utils::createTrace({"a", "b"}, {{}, {}, {"a", "b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("a & O[0,30](b)");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsat_negated_and_once_interval_and_1){
    Trace trace = Utils::createTrace({"in_same_lane", "in_single_lane"}, {{"in_single_lane"}, {"in_single_lane"}, {"in_same_lane"}, {"in_same_lane"}, {"in_same_lane"}});
    Formula formula = Parser::parse("in_same_lane & !O[0,30](in_same_lane & !(in_single_lane) & P(in_single_lane))");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TESTMTL, unsat_negated_and_once_interval_and_2) {
    Trace trace =
        Utils::createTrace({"in_same_lane", "in_single_lane", "keeps_safe_distance_prec", "orientation_towards"},
                           {{"in_same_lane", "keeps_safe_distance_prec"},
                            {"in_same_lane", "keeps_safe_distance_prec"},
                            {"in_same_lane", "in_single_lane"},
                            {"in_same_lane", "in_single_lane"},
                            {"in_same_lane", "in_single_lane"}});
    Formula formula = Parser::parse("(in_same_lane & !O[0,30](in_same_lane & !(in_single_lane) & orientation_towards & P(in_single_lane & orientation_towards))) -> keeps_safe_distance_prec");
    auto monitor{MTLEventConsuming(formula)};
    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
}