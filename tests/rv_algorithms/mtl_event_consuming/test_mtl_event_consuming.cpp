//
// Created by Sebastian Maierhofer on 15.01.21.
//

#include "test_mtl_event_consuming.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/mtl_event_consuming//mtl_event_consuming.h"
#include "monitors/trace/trace.h"
#include "monitors/utils/utils.h"

// TEST(TestMTL, violated1) {
//    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"b"}, {"b"}, {"b"}, {"a"}, {"b"}, {"b"}, {"b"}});
//    Formula formula = Parser::parse("G(a -> F[0, 6](b & O[0, 6](c)))");
//    auto monitor{MTLEventConsuming(formula)};
//    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
//}
//
// TEST(TestMTL, satisfied1) {
//    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"b"}, {"a"}, {"b"}, {"c"}, {"b"}});
//    Formula formula = Parser::parse("G(a -> F[0, 6](b & O[0, 6](c)))");
//    auto monitor{MTLEventConsuming(formula)};
//    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
//}
//
// TEST(TestMTL, satisfied2) {
//    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"b"}});
//    Formula formula = Parser::parse("G(a -> F[0, 6](b))");
//    auto monitor{MTLEventConsuming(formula)};
//    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
//}
//
// TEST(TestMTL, violated2) {
//    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
//    Formula formula = Parser::parse("G(a -> F[0, 6](b))");
//    auto monitor{MTLEventConsuming(formula)};
//    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
//}
//
// TEST(TestMTL, satisfied3) {
//    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
//    Formula formula = Parser::parse("G(a)");
//    auto monitor{MTLEventConsuming(formula)};
//    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
//}
//
// TEST(TestMTL, violated3) {
//    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
//    Formula formula = Parser::parse("G(a)");
//    auto monitor{MTLEventConsuming(formula)};
//    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
//}
//
// TEST(TestMTL, violated4) {
//    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
//    Formula formula = Parser::parse("G(a -> O[0, 4](b))");
//    auto monitor{MTLEventConsuming(formula)};
//    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
//}
//
// TEST(TestMTL, satisfied4) {
//    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"b"}, {"c"}, {"a"}, {"b"}, {"b"}, {"a"}, {"b"}});
//    Formula formula = Parser::parse("G(a -> O[0, 6](b))");
//    auto monitor{MTLEventConsuming(formula)};
//    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
//}
//
// TEST(TestMTL, satisfied5) {
//    Trace trace = Utils::createTrace({"a"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
//    Formula formula = Parser::parse("O[0, 6](a)");
//    auto monitor{MTLEventConsuming(formula)};
//    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
//}
//
// TEST(TestMTL, violated5) {
//    Trace trace = Utils::createTrace({"a"}, {{}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
//    Formula formula = Parser::parse("O[0, 6](a)");
//    auto monitor{MTLEventConsuming(formula)};
//    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
//}
//
// TEST(TestMTL, violated6) {
//    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
//    Formula formula = Parser::parse("G(a -> P(b))");
//    auto monitor{MTLEventConsuming(formula)};
//    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
//}
//
// TEST(TestMTL, satisfied6) {
//    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"a"}, {"b"}, {"b"}, {"a"}, {"b"}});
//    Formula formula = Parser::parse("G(a -> P(b))");
//    auto monitor{MTLEventConsuming(formula)};
//    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
//}
//
// TEST(TestMTL, violated7) {
//    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"b"}, {"b"}, {"b"}, {"c"}, {"a"}});
//    Formula formula = Parser::parse("G(a -> O(b & P(!b)))");
//    auto monitor{MTLEventConsuming(formula)};
//    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
//}
//
// TEST(TestMTL, satisfied7) {
//    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"a", "b"}, {"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
//    Formula formula = Parser::parse("G(a -> O(b & P(!b)))");
//    auto monitor{MTLEventConsuming(formula)};
//    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
//}
//
// TEST(TestMTL, violated8) {
//    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"b"}, {"a"}, {"c"}});
//    Formula formula = Parser::parse("G((a & O(b & P(!b))) -> c)");
//    auto monitor{MTLEventConsuming(formula)};
//    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
//}
//
// TEST(TestMTL, satisfied8) {
//    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"b"}, {"c", "a"}, {"c"}});
//    Formula formula = Parser::parse("G((a & O(b & P(!b))) -> c)");
//    auto monitor{MTLEventConsuming(formula)};
//    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
//}
//
// TEST(TestMTL, satisfied9) {
//    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a"}});
//    Formula formula = Parser::parse("G((a & O(b & P(!b))) -> c)");
//    auto monitor{MTLEventConsuming(formula)};
//    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
//}
//
// TEST(TestMTL, satisfied10) {
//    Trace trace = Utils::createTrace(
//        {"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a"}, {"a"}, {"c"}, {"c"}});
//    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,5](H[0,2](a & b))))");
//    auto monitor{MTLEventConsuming(formula)};
//    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
//}
//
// TEST(TestMTL, violated10) {
//    Trace trace =
//        Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a", "b"}, {"a"}, {"a"}, {"c"}, {"c"}});
//    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,5](H[0,2](a & b))))");
//    auto monitor{MTLEventConsuming(formula)};
//    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
//}
//
// TEST(TestMTL, violated11) {
//    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a"}, {"a"}, {"a"}, {"c"}, {"c"}});
//    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,3](a & b)))");
//    auto monitor{MTLEventConsuming(formula)};
//    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
//}
//
// TEST(TestMTL, satisfied11) {
//    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a"}, {"c"}, {"c"}});
//    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,3](a & b)))");
//    auto monitor{MTLEventConsuming(formula)};
//    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
//}
//
// TEST(TestMTL, satisfied12) {
//    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b"}, {"a", "b", "c"}, {"a"}, {"a"},
//    {"b"}}); Formula formula = Parser::parse("G(c -> H[0,1](a & b))"); auto monitor{MTLEventConsuming(formula)};
//    EXPECT_TRUE(monitor.evaluate(trace)[formula.getId()].at(0).b);
//}
//
// TEST(TestMTL, violated12) {
//    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}});
//    Formula formula = Parser::parse("G(c -> H[0,1](a & b))");
//    auto monitor{MTLEventConsuming(formula)};
//    EXPECT_FALSE(monitor.evaluate(trace)[formula.getId()].at(0).b);
//}