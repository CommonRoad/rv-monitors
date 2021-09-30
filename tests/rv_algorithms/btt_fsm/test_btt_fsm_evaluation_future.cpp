#include "test_btt_fsm_evaluation_future.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/btt_fsm/btt_fsm_utils_creation.h"
#include "monitors/trace/trace.h"
#include "monitors/utils/utils.h"

TEST(TestBTT_FSMEvaluation, violated1) {
    Trace trace = Utils::createTrace(
        {"a", "b", "c", "g", "f"},
        {{"a"}, {"b"}, {"a"}, {"b"}, {"a"}, {"c"}, {"a"}, {"a"}, {"b"}, {"g"}, {"f"}, {"h"}, {"c"}, {"b"}, {"a"}});
    Formula formula = Parser::parse("G(a -> F(b))");
    auto btt = BttFsm({formula});

    EXPECT_FALSE(btt.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestBTT_FSMEvaluation, satisfied1) {
    Trace trace = Utils::createTrace(
        {"a", "b", "c", "g", "f"},
        {{"a"}, {"b"}, {"a"}, {"b"}, {"a"}, {"c"}, {"a"}, {"a"}, {"b"}, {"g"}, {"f"}, {"h"}, {"c"}, {"b"}, {"a"}});
    Formula formula = Parser::parse("F(!G(b -> Fc))");
    auto btt = BttFsm({formula});

    EXPECT_TRUE(btt.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestBTT_FSMEvaluation, satisfied2) {
    Formula formula = Parser::parse("G(b -> Fc)");
    Trace trace = Utils::createTrace(
        {"a", "b", "c", "g", "f", "h"},
        {{"a"}, {"b"}, {"a"}, {"b"}, {"a"}, {"c"}, {"a"}, {"a"}, {"b"}, {"g"}, {"f"}, {"h"}, {"c"}, {"b"}, {"c"}});

    auto btt = BttFsm({formula});

    EXPECT_TRUE(btt.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestBTT_FSMEvaluation, violated2) {
    Trace trace = Utils::createTrace(
        {"a", "b", "c", "g", "f", "h"},
        {{"a"}, {"b"}, {"a"}, {"b"}, {"a"}, {"c"}, {"a"}, {"a"}, {"b"}, {"g"}, {"f"}, {"h"}, {"c"}, {"b"}, {"c"}});
    Formula formula = Parser::parse("F(!G(b -> Fc))");
    auto btt = BttFsm({formula});

    EXPECT_FALSE(btt.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestBTT_FSMEvaluation, satisfied3) {
    Trace trace = Utils::createTrace(
        {"a", "b", "c", "d", "e"},
        {{"a"}, {"b"}, {"a"}, {"b"}, {"a"}, {"c"}, {"a"}, {"a"}, {"b"}, {"e"}, {"d"}, {"e"}, {"c"}, {"b"}, {"c"}});
    Formula formula = Parser::parse("G((a & b & c & F(d)) -> (!(!(e) & F(e))))");
    auto btt = BttFsm({formula});

    EXPECT_TRUE(btt.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestBTT_FSMEvaluation, violatedEnd) {
    Trace trace =
        Utils::createTrace({"green", "yellow", "red"},
                           {{"green"}, {"red"}, {"yellow"}, {"red"}, {"green"}, {"yellow"}, {"green"}, {"red"}});
    Formula formula = Parser::parse("G(green -> !red U yellow)");
    auto btt = BttFsm({formula});

    EXPECT_FALSE(btt.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestBTT_FSMEvaluation, violatedBeginning) {
    Trace trace =
        Utils::createTrace({"green", "yellow", "red"},
                           {{"green"}, {"red"}, {"yellow"}, {"red"}, {"yellow"}, {"red"}, {"green"}, {"yellow"}});
    Formula formula = Parser::parse("G(green -> !red U yellow)");
    auto btt = BttFsm({formula});

    EXPECT_FALSE(btt.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestBTT_FSMEvaluation, satisfied) {
    Trace trace = Utils::createTrace({"green", "yellow", "red"},
                                     {{"green"}, {"yellow"}, {"red"}, {"green"}, {"yellow"}, {"red"}, {"red"}});
    Formula formula = Parser::parse("G(green -> !red U yellow)");
    auto btt = BttFsm({formula});

    EXPECT_TRUE(btt.evaluate(trace)[formula.getId()].at(0).b);
}
