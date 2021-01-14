#include <MT-FSM/MT_FSMCreation.h>
#include "MT-FSM/MT_FSMEvaluation.h"
#include "TestMT_FSMEvaluationFuture.h"
#include "Utils/Utils.h"
#include "Parser/Parser.h"
#include "Trace/Trace.h"

TEST(TestMT_FSMFuture, violated1) {
    Trace trace = Utils::createTrace({"a", "b", "c", "g", "f"}, {
            {"a"},
            {"b"},
            {"a"},
            {"b"},
            {"a"},
            {"c"},
            {"a"},
            {"a"},
            {"b"},
            {"g"},
            {"f"},
            {"h"},
            {"c"},
            {"b"},
            {"a"}});
    Formula formula = Parser::parse("G(a -> F(b))");
    auto mtFSM = MT_FSMCreation::formulaToMT_FSM(formula, false);
    EXPECT_FALSE(MT_FSMEvaluation::evaluateMT_FSM(mtFSM, trace, false));
}

TEST(TestMT_FSMFuture, satisfied1) {
    Trace trace = Utils::createTrace({"a", "b", "c", "g", "f"}, {
            {"a"},
            {"b"},
            {"a"},
            {"b"},
            {"a"},
            {"c"},
            {"a"},
            {"a"},
            {"b"},
            {"g"},
            {"f"},
            {"h"},
            {"c"},
            {"b"},
            {"a"}});
    Formula formula = Parser::parse("F(!G(b -> Fc))");
    auto mtFSM = MT_FSMCreation::formulaToMT_FSM(formula, false);

    EXPECT_TRUE(MT_FSMEvaluation::evaluateMT_FSM(mtFSM, trace, false));
}

TEST(TestMT_FSMFuture, satisfied2) {
    Formula formula = Parser::parse("G(b -> Fc)");
    Trace trace = Utils::createTrace({"a", "b", "c", "g", "f", "h"}, {
            {"a"},
            {"b"},
            {"a"},
            {"b"},
            {"a"},
            {"c"},
            {"a"},
            {"a"},
            {"b"},
            {"g"},
            {"f"},
            {"h"},
            {"c"},
            {"b"},
            {"c"}});

    auto mtFSM = MT_FSMCreation::formulaToMT_FSM(formula, false);

    EXPECT_TRUE(MT_FSMEvaluation::evaluateMT_FSM(mtFSM, trace, false));
}

TEST(TestMT_FSMFuture, violated2) {
    Trace trace = Utils::createTrace({"a", "b", "c", "g", "f", "h"}, {
            {"a"},
            {"b"},
            {"a"},
            {"b"},
            {"a"},
            {"c"},
            {"a"},
            {"a"},
            {"b"},
            {"g"},
            {"f"},
            {"h"},
            {"c"},
            {"b"},
            {"c"}});
    Formula formula = Parser::parse("F(!G(b -> Fc))");
    auto mtFSM = MT_FSMCreation::formulaToMT_FSM(formula, false);

    EXPECT_FALSE(MT_FSMEvaluation::evaluateMT_FSM(mtFSM, trace, false));
}

TEST(TestMT_FSMFuture, violatedEnd) {
    Trace trace = Utils::createTrace({"green", "yellow", "red"}, {
            {"green"},
            {"red"},
            {"yellow"},
            {"red"},
            {"green"},
            {"yellow"},
            {"green"},
            {"red"}});
    Formula formula = Parser::parse("G(green -> !red U yellow)");
    auto mtFSM = MT_FSMCreation::formulaToMT_FSM(formula, false);

    EXPECT_FALSE(MT_FSMEvaluation::evaluateMT_FSM(mtFSM, trace, false));
}

TEST(TestMT_FSMFuture, violatedBeginning) {
    Trace trace = Utils::createTrace({"green", "yellow", "red"}, {
            {"green"},
            {"red"},
            {"yellow"},
            {"red"},
            {"yellow"},
            {"red"},
            {"green"},
            {"yellow"}});
    Formula formula = Parser::parse("G(green -> !red U yellow)");
    auto mtFSM = MT_FSMCreation::formulaToMT_FSM(formula, false);

    EXPECT_FALSE(MT_FSMEvaluation::evaluateMT_FSM(mtFSM, trace, false));
}

TEST(TestMT_FSMFuture, satisfied) {
    Trace trace = Utils::createTrace({"green", "yellow", "red"}, {
            {"green"},
            {"yellow"},
            {"red"},
            {"green"},
            {"yellow"},
            {"red"},
            {"red"}});
    Formula formula = Parser::parse("G(green -> !red U yellow)");
    auto mtFSM = MT_FSMCreation::formulaToMT_FSM(formula, false);

    EXPECT_TRUE(MT_FSMEvaluation::evaluateMT_FSM(mtFSM, trace, false));
}


TEST(TestBTT_FSMEvaluation, mtl) {
    Trace trace = Utils::createTrace({"a", "b"}, {
            {"a"},
            {"a"},
            {"a"},
            {"a"},
            {"a"},
            {"b"}});
    Formula formula1 = Parser::parse("G[0, 3](a)");
    auto btt1 = MT_FSMCreation::formulaToMT_FSM(formula1, false);
    std::cout << btt1.to_string();
    Formula formula2 = Parser::parse("G(a)");
    auto btt2 = MT_FSMCreation::formulaToMT_FSM(formula2, false);
    std::cout << btt2.to_string();
    EXPECT_TRUE(MT_FSMEvaluation::evaluateMT_FSM(btt1, trace, false));
}