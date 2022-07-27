#include "test_event_consuming_future.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/ltl_event_consuming/event_consuming.h"
#include "monitors/trace/trace.h"
#include "monitors/utils/utils.h"

TEST(TestECFuture, violated1) {
    Trace trace = Utils::createTrace(
        {"a", "b", "c", "g", "f"},
        {{"a"}, {"b"}, {"a"}, {"b"}, {"a"}, {"c"}, {"a"}, {"a"}, {"b"}, {"g"}, {"f"}, {"h"}, {"c"}, {"b"}, {"a"}});
    Formula formula = Parser::parse("G(a -> F(b))");

    auto eventConsuming{EventConsumingLTL({formula})};
    EXPECT_FALSE(eventConsuming.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestECFuture, satisfied1) {
    Trace trace = Utils::createTrace(
        {"a", "b", "c", "g", "f"},
        {{"a"}, {"b"}, {"a"}, {"b"}, {"a"}, {"c"}, {"a"}, {"a"}, {"b"}, {"g"}, {"f"}, {"h"}, {"c"}, {"b"}, {"a"}});
    Formula formula = Parser::parse("F(!G(b -> Fc))");
    auto eventConsuming{EventConsumingLTL({formula})};
    EXPECT_TRUE(eventConsuming.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestECFuture, satisfied2) {
    Formula formula = Parser::parse("G(b -> Fc)");
    Trace trace = Utils::createTrace(
        {"a", "b", "c", "g", "f", "h"},
        {{"a"}, {"b"}, {"a"}, {"b"}, {"a"}, {"c"}, {"a"}, {"a"}, {"b"}, {"g"}, {"f"}, {"h"}, {"c"}, {"b"}, {"c"}});
    auto eventConsuming{EventConsumingLTL({formula})};
    EXPECT_TRUE(eventConsuming.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestECFuture, violated2) {
    Trace trace = Utils::createTrace(
        {"a", "b", "c", "g", "f", "h"},
        {{"a"}, {"b"}, {"a"}, {"b"}, {"a"}, {"c"}, {"a"}, {"a"}, {"b"}, {"g"}, {"f"}, {"h"}, {"c"}, {"b"}, {"c"}});
    Formula formula = Parser::parse("F(!G(b -> Fc))");
    auto eventConsuming{EventConsumingLTL({formula})};
    EXPECT_FALSE(eventConsuming.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestECFuture, violatedEnd) {
    Trace trace =
        Utils::createTrace({"green", "yellow", "red"},
                           {{"green"}, {"red"}, {"yellow"}, {"red"}, {"green"}, {"yellow"}, {"green"}, {"red"}});
    Formula formula = Parser::parse("G(green -> !red U yellow)");
    auto eventConsuming{EventConsumingLTL({formula})};
    EXPECT_FALSE(eventConsuming.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestECFuture, violatedBeginning) {
    Trace trace =
        Utils::createTrace({"green", "yellow", "red"},
                           {{"green"}, {"red"}, {"yellow"}, {"red"}, {"yellow"}, {"red"}, {"green"}, {"yellow"}});
    Formula formula = Parser::parse("G(green -> !red U yellow)");
    auto eventConsuming{EventConsumingLTL({formula})};
    EXPECT_FALSE(eventConsuming.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestECFuture, satisfied) {
    Trace trace = Utils::createTrace({"green", "yellow", "red"},
                                     {{"green"}, {"yellow"}, {"red"}, {"green"}, {"yellow"}, {"red"}, {"red"}});
    Formula formula = Parser::parse("G(green -> !red U yellow)");
    auto eventConsuming{EventConsumingLTL({formula})};
    EXPECT_TRUE(eventConsuming.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestECFuture, strongNext) {
    Trace trace = Utils::createTrace({"a"}, {{"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(X(a))");
    auto eventConsuming{EventConsumingLTL({formula})};
    EXPECT_FALSE(eventConsuming.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestECFuture, weakNext) {
    Trace trace = Utils::createTrace({"a"}, {{"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(W(a))");
    auto eventConsuming{EventConsumingLTL({formula})};
    EXPECT_TRUE(eventConsuming.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestECFuture, atomic) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"b"}, {"a"}});
    Formula formula = Parser::parse("G(a)");
    auto eventConsuming{EventConsumingLTL({formula})};
    EXPECT_FALSE(eventConsuming.evaluate(trace)[formula.getId()].at(0).value);
}