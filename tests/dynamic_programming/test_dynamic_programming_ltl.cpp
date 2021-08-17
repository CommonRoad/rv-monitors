#include "test_dynamic_programming_ltl.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/dynamic_programming/dynamic.h"
#include "monitors/trace/trace.h"
#include "monitors/utils/utils.h"

TEST(TestDynamicFuture, violated1) {
    Trace trace1 = Utils::createTrace(
        {"a", "b", "c", "g", "f"},
        {{"a"}, {"b"}, {"a"}, {"b"}, {"a"}, {"c"}, {"a"}, {"a"}, {"b"}, {"g"}, {"f"}, {"h"}, {"c"}, {"b"}, {"a"}});
    Formula formula = Parser::parse("G(a -> Fb)");
    auto dynamic{DynamicProgrammingLTL(formula)};
    EXPECT_FALSE(dynamic.evaluate(trace1).at(0).b);
}

TEST(TestDynamicFuture, satisfied2) {
    Formula formula = Parser::parse("G(b -> Fc)");
    Trace trace2 = Utils::createTrace(
        {"a", "b", "c", "g", "f", "h"},
        {{"a"}, {"b"}, {"a"}, {"b"}, {"a"}, {"c"}, {"a"}, {"a"}, {"b"}, {"g"}, {"f"}, {"h"}, {"c"}, {"b"}, {"c"}});
    auto dynamic{DynamicProgrammingLTL(formula)};
    EXPECT_TRUE(dynamic.evaluate(trace2).at(0).b);
}

TEST(TestDynamicFuture, violated2) {
    Trace trace2 = Utils::createTrace(
        {"a", "b", "c", "g", "f", "h"},
        {{"a"}, {"b"}, {"a"}, {"b"}, {"a"}, {"c"}, {"a"}, {"a"}, {"b"}, {"g"}, {"f"}, {"h"}, {"c"}, {"b"}, {"c"}});
    Formula formula = Parser::parse("F(!(G(b -> Fc)))");
    auto dynamic{DynamicProgrammingLTL(formula)};
    EXPECT_FALSE(dynamic.evaluate(trace2).at(0).b);
}

TEST(TestDynamicFuture, violatedEnd) {
    Trace trace = Utils::createTrace({"green", "yellow", "red"},
                                     {{"green"}, {"yellow"}, {"red"}, {"green"}, {"yellow"}, {"green"}, {"red"}});
    Formula formula = Parser::parse("G(green -> !red U yellow)");
    auto dynamic{DynamicProgrammingLTL(formula)};
    EXPECT_FALSE(dynamic.evaluate(trace).at(0).b);
}

TEST(TestDynamicFuture, violatedBeginning) {
    Trace trace =
        Utils::createTrace({"green", "yellow", "red"},
                           {{"green"}, {"red"}, {"yellow"}, {"red"}, {"yellow"}, {"red"}, {"green"}, {"yellow"}});
    Formula formula = Parser::parse("G(green -> !red U yellow)");
    auto dynamic{DynamicProgrammingLTL(formula)};
    EXPECT_FALSE(dynamic.evaluate(trace).at(0).b);
}

TEST(TestDynamicFuture, satisfied) {
    Trace trace = Utils::createTrace({"green", "yellow", "red"},
                                     {{"green"}, {"yellow"}, {"red"}, {"yellow"}, {"red"}, {"green"}, {"yellow"}});
    Formula formula = Parser::parse("G(green -> !red U yellow)");
    auto dynamic{DynamicProgrammingLTL(formula)};
    EXPECT_TRUE(dynamic.evaluate(trace).at(0).b);
}

TEST(TestDynamicFuture, strongNext) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(b -> (X(a)))");

    auto dynamic{DynamicProgrammingLTL(formula)};
    EXPECT_FALSE(dynamic.evaluate(trace).at(0).b);
}

TEST(TestDynamicFuture, until) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("a U b");
    auto dynamic{DynamicProgrammingLTL(formula)};
    EXPECT_TRUE(dynamic.evaluate(trace).at(0).b);
}

TEST(TestDynamicPast, violatedEnd) {
    Trace trace =
        Utils::createTrace({"green", "yellow", "red"}, {{"green"}, {"red"}, {"yellow"}, {"green"}, {"red"}, {"green"}});
    Formula formula = Parser::parse("G(green -> P(yellow))");
    auto dynamic{DynamicProgrammingLTL(formula, true)};
    EXPECT_FALSE(dynamic.evaluate(trace).at(0).b);
}

TEST(TestDynamicPast, violatedBeginning) {
    Trace trace = Utils::createTrace({"green", "yellow", "red"},
                                     {{"red"}, {"green"}, {"red"}, {"yellow"}, {"red"}, {"green"}, {"yellow"}});
    Formula formula = Parser::parse("G(green -> P(yellow))");
    auto dynamic{DynamicProgrammingLTL(formula, true)};
    EXPECT_FALSE(dynamic.evaluate(trace).at(0).b);
}

TEST(TestDynamicPast, satisfied) {
    Trace trace =
        Utils::createTrace({"green", "yellow", "red"},
                           {{"yellow"}, {"green"}, {"red"}, {"yellow"}, {"green"}, {"red"}, {"yellow"}, {"green"}});
    Formula formula = Parser::parse("G(green -> P(yellow))");
    auto dynamic{DynamicProgrammingLTL(formula, true)};
    EXPECT_TRUE(dynamic.evaluate(trace).at(0).b);
}