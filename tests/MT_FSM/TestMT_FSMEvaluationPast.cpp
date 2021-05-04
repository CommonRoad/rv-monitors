#include "Parser/Parser.h"
#include "RV-Algorithms/MT-FSM/MT_FSMCreation.h"
#include "RV-Algorithms/MT-FSM/MT_FSMEvaluation.h"
#include "TestMT_FSMEvaluationFuture.h"
#include "Trace/Trace.h"
#include "Utils/Utils.h"

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