#include "test_mt_fsm_evaluation_future.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/mt_fsm/mt_fsm.h"
#include "monitors/trace/trace.h"
#include "monitors/utils/utils.h"

TEST(TestMT_FSMFuture, testFormulaType) {
    Formula formulaOne = Parser::parse("G(a -> F(b))");
    Formula formulaTwo = Parser::parse("H(a -> F(b))");
    Formula formulaThree = Parser::parse("G(a -> F(b) & O(c))");
    EXPECT_NO_THROW(MtFsm({formulaOne}));
    EXPECT_THROW(MtFsm({formulaTwo}), std::logic_error);
    EXPECT_THROW(MtFsm({formulaThree}), std::logic_error);
}

TEST(TestMT_FSMFuture, violated1) {
    Trace trace = Utils::createTrace(
        {"a", "b", "c", "g", "f"},
        {{"a"}, {"b"}, {"a"}, {"b"}, {"a"}, {"c"}, {"a"}, {"a"}, {"b"}, {"g"}, {"f"}, {"h"}, {"c"}, {"b"}, {"a"}});
    Formula formula = Parser::parse("G(a -> F(b))");
    auto mtFSM = MtFsm({formula});
    EXPECT_FALSE(mtFSM.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMT_FSMFuture, satisfied1) {
    Trace trace = Utils::createTrace(
        {"a", "b", "c", "g", "f"},
        {{"a"}, {"b"}, {"a"}, {"b"}, {"a"}, {"c"}, {"a"}, {"a"}, {"b"}, {"g"}, {"f"}, {"h"}, {"c"}, {"b"}, {"a"}});
    Formula formula = Parser::parse("F(!G(b -> Fc))");
    auto mtFSM = MtFsm({formula});
    EXPECT_TRUE(mtFSM.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMT_FSMFuture, satisfied2) {
    Formula formula = Parser::parse("G(b -> Fc)");
    Trace trace = Utils::createTrace(
        {"a", "b", "c", "g", "f", "h"},
        {{"a"}, {"b"}, {"a"}, {"b"}, {"a"}, {"c"}, {"a"}, {"a"}, {"b"}, {"g"}, {"f"}, {"h"}, {"c"}, {"b"}, {"c"}});
    auto mtFSM = MtFsm({formula});
    EXPECT_TRUE(mtFSM.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMT_FSMFuture, violated2) {
    Trace trace = Utils::createTrace(
        {"a", "b", "c", "g", "f", "h"},
        {{"a"}, {"b"}, {"a"}, {"b"}, {"a"}, {"c"}, {"a"}, {"a"}, {"b"}, {"g"}, {"f"}, {"h"}, {"c"}, {"b"}, {"c"}});
    Formula formula = Parser::parse("F(!G(b -> Fc))");
    auto mtFSM = MtFsm({formula});
    EXPECT_FALSE(mtFSM.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMT_FSMFuture, violatedEnd) {
    Trace trace =
        Utils::createTrace({"green", "yellow", "red"},
                           {{"green"}, {"red"}, {"yellow"}, {"red"}, {"green"}, {"yellow"}, {"green"}, {"red"}});
    Formula formula = Parser::parse("G(green -> !red U yellow)");
    auto mtFSM = MtFsm({formula});
    EXPECT_FALSE(mtFSM.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMT_FSMFuture, violatedBeginning) {
    Trace trace =
        Utils::createTrace({"green", "yellow", "red"},
                           {{"green"}, {"red"}, {"yellow"}, {"red"}, {"yellow"}, {"red"}, {"green"}, {"yellow"}});
    Formula formula = Parser::parse("G(green -> !red U yellow)");
    auto mtFSM = MtFsm({formula});
    EXPECT_FALSE(mtFSM.evaluate(trace)[formula.getId()].at(0).value);
}

TEST(TestMT_FSMFuture, satisfied) {
    Trace trace = Utils::createTrace({"green", "yellow", "red"},
                                     {{"green"}, {"yellow"}, {"red"}, {"green"}, {"yellow"}, {"red"}, {"red"}});
    Formula formula = Parser::parse("G(green -> !red U yellow)");
    auto mtFSM = MtFsm({formula});
    EXPECT_TRUE(mtFSM.evaluate(trace)[formula.getId()].at(0).value);
}
