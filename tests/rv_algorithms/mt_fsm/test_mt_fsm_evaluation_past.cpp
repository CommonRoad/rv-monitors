#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/mt_fsm/mt_fsm.h"
#include "monitors/trace/trace.h"
#include "monitors/utils/utils.h"
#include "test_mt_fsm_evaluation_future.h"

TEST(TestMT_FSMPast, violatedEnd) {
    Trace trace =
        Utils::createTrace({"green", "yellow", "red"}, {{"green"}, {"red"}, {"yellow"}, {"green"}, {"red"}, {"green"}});
    Formula formula = Parser::parse("G(green -> P(yellow))");
    auto mtFSM = MtFsm({formula}, true);
    EXPECT_FALSE(mtFSM.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMT_FSMPast, violatedBeginning) {
    Trace trace = Utils::createTrace({"green", "yellow", "red"},
                                     {{"red"}, {"green"}, {"red"}, {"yellow"}, {"red"}, {"green"}, {"yellow"}});
    Formula formula = Parser::parse("G(green -> P(yellow))");
    auto mtFSM = MtFsm({formula}, true);
    EXPECT_FALSE(mtFSM.evaluate(trace)[formula.getId()].at(0).b);
}

TEST(TestMT_FSMPast, satisfied) {
    Trace trace =
        Utils::createTrace({"green", "yellow", "red"},
                           {{"yellow"}, {"green"}, {"red"}, {"yellow"}, {"green"}, {"red"}, {"yellow"}, {"green"}});
    Formula formula = Parser::parse("G(green -> P(yellow))");
    auto mtFSM = MtFsm({formula}, true);
    EXPECT_TRUE(mtFSM.evaluate(trace)[formula.getId()].at(0).b);
}