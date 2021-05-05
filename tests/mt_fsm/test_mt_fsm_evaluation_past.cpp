#include "parser/parser.h"
#include "rv_algorithms/mt_fsm/mt_fsm_creation.h"
#include "rv_algorithms/mt_fsm/mt_fsm_evaluation.h"
#include "test_mt_fsm_evaluation_future.h"
#include "trace/trace.h"
#include "utils/utils.h"

TEST(TestMT_FSMPast, violatedEnd) {
    Trace trace =
        Utils::createTrace({"green", "yellow", "red"}, {{"green"}, {"red"}, {"yellow"}, {"green"}, {"red"}, {"green"}});
    Formula formula = Parser::parse("G(green -> P(yellow))");
    auto mtFSM = MT_FSMCreation::formulaToMT_FSM(formula, true);

    EXPECT_FALSE(MT_FSMEvaluation::evaluateMT_FSM(mtFSM, trace, true));
}

TEST(TestMT_FSMPast, violatedBeginning) {
    Trace trace = Utils::createTrace({"green", "yellow", "red"},
                                     {{"red"}, {"green"}, {"red"}, {"yellow"}, {"red"}, {"green"}, {"yellow"}});
    Formula formula = Parser::parse("G(green -> P(yellow))");
    auto mtFSM = MT_FSMCreation::formulaToMT_FSM(formula, true);

    EXPECT_FALSE(MT_FSMEvaluation::evaluateMT_FSM(mtFSM, trace, true));
}

TEST(TestMT_FSMPast, satisfied) {
    Trace trace =
        Utils::createTrace({"green", "yellow", "red"},
                           {{"yellow"}, {"green"}, {"red"}, {"yellow"}, {"green"}, {"red"}, {"yellow"}, {"green"}});
    Formula formula = Parser::parse("G(green -> P(yellow))");
    auto mtFSM = MT_FSMCreation::formulaToMT_FSM(formula, true);

    EXPECT_TRUE(MT_FSMEvaluation::evaluateMT_FSM(mtFSM, trace, true));
}