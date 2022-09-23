//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2022 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//

#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/and_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/constant_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/negation_monitor.h"
#include "monitors/utils/utils.h"
#include "test_or_monitor.h"

TEST(TestAndMonitor, testSingleAndMonitorFalseEvaluation) {
    Trace trace = Utils::createTrace({"a"}, {{"a"}, {}}, std::vector<size_t>{0, 1});
    Formula formula = Parser::parse("false & true");
    Formula formulaFalse = Parser::parse("false");
    Formula formulaTrue = Parser::parse("true");
    formula.setId("R1");
    auto mon1{std::make_shared<ConstantMonitor>(formulaFalse, 2, false)};
    mon1->registerParent(1, OperatorType::AND, {});
    auto mon2{std::make_shared<ConstantMonitor>(formulaTrue, 3, false)};
    mon2->registerParent(1, OperatorType::AND, {});
    auto mon{AndMonitor(formula, {mon1, mon2}, 1, false, false)};
    mon.registerParent(0, OperatorType::BASE, {});
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep(0, 0).value, false);
    EXPECT_EQ(mon.step(0, 0).value, false);
    EXPECT_EQ(mon.step(0, 4).value, false);
}

TEST(TestAndMonitor, testSingleAndMonitorTrueEvaluation) {
    Trace trace = Utils::createTrace({"a"}, {{"a"}, {}}, std::vector<size_t>{0, 1});
    Formula formula = Parser::parse("true & true");
    Formula formulaTrue = Parser::parse("true");
    formula.setId("R1");
    auto mon1{std::make_shared<ConstantMonitor>(formulaTrue, 2, false)};
    mon1->registerParent(1, OperatorType::AND, {});
    auto mon2{std::make_shared<ConstantMonitor>(formulaTrue, 3, false)};
    mon2->registerParent(1, OperatorType::AND, {});
    auto mon{AndMonitor(formula, {mon1, mon2}, 1, false, false)};
    mon.registerParent(0, OperatorType::BASE, {});
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep(0, 0).value, true);
    EXPECT_EQ(mon.step(0, 0).value, true);
}

TEST(TestAndMonitor, testSingleAndMonitorFalseEvaluationShortCircuiting) {
    Trace trace = Utils::createTrace({"a"}, {{"a"}, {}}, std::vector<size_t>{0, 1});
    Formula formula = Parser::parse("false & true");
    Formula formulaFalse = Parser::parse("false");
    Formula formulaTrue = Parser::parse("true");
    formula.setId("R1");
    auto mon1{std::make_shared<ConstantMonitor>(formulaFalse, 2, false)};
    mon1->registerParent(1, OperatorType::AND, {});
    auto mon2{std::make_shared<ConstantMonitor>(formulaTrue, 3, false)};
    mon2->registerParent(1, OperatorType::AND, {});
    auto mon{AndMonitor(formula, {mon1, mon2}, 1, false, true)};
    mon.registerParent(0, OperatorType::BASE, {});
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep(0, 0).value, false);
    EXPECT_EQ(mon.step(0, 0).value, false);
    EXPECT_EQ(mon.step(0, 4).value, false);
}

TEST(TestAndMonitor, testSingleAndMonitorTrueEvaluationShortCircuiting) {
    Trace trace = Utils::createTrace({"a"}, {{"a"}, {}}, std::vector<size_t>{0, 1});
    Formula formula = Parser::parse("true & true");
    Formula formulaTrue = Parser::parse("true");
    formula.setId("R1");
    auto mon1{std::make_shared<ConstantMonitor>(formulaTrue, 2, false)};
    mon1->registerParent(1, OperatorType::AND, {});
    auto mon2{std::make_shared<ConstantMonitor>(formulaTrue, 3, false)};
    mon2->registerParent(1, OperatorType::AND, {});
    auto mon{AndMonitor(formula, {mon1, mon2}, 1, false, true)};
    mon.registerParent(0, OperatorType::BASE, {});
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep(0, 0).value, true);
    EXPECT_EQ(mon.step(0, 0).value, true);
}

TEST(TestAndMonitor, testSingleAndMonitorShortCircuitingForLoop) {
    Trace trace = Utils::createTrace({"a"}, {{"a"}, {}}, std::vector<size_t>{0, 1});
    Formula formula = Parser::parse("!(false) & false");
    Formula formulaFalse = Parser::parse("false");
    Formula formulaNegation = Parser::parse("!(false)");
    formula.setId("R1");
    auto mon1{std::make_shared<NegationMonitor>(formulaNegation,
                                                std::make_shared<ConstantMonitor>(formulaFalse, 4, false), 2, false)};
    mon1->registerParent(1, OperatorType::AND, {});
    auto mon2{std::make_shared<ConstantMonitor>(formulaFalse, 3, false)};
    mon2->registerParent(1, OperatorType::AND, {});
    auto mon{AndMonitor(formula, {mon1, mon2}, 1, false, true)};
    mon.registerParent(0, OperatorType::BASE, {});
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep(0, 0).value, false);
    EXPECT_EQ(mon.step(0, 0).value, false);
    EXPECT_EQ(mon.step(0, 4).value, false);
    EXPECT_EQ(mon.concreteStep(0, 4).value, false);
}

TEST(TestAndMonitor, testToString) {
    Trace trace = Utils::createTrace({"a"}, {{"a"}, {}}, std::vector<size_t>{0, 1});
    Formula formula = Parser::parse("false & true");
    Formula formulaFalse = Parser::parse("false");
    Formula formulaTrue = Parser::parse("true");
    formula.setId("R1");
    auto mon1{std::make_shared<ConstantMonitor>(formulaFalse, 2, false)};
    mon1->registerParent(1, OperatorType::AND, {});
    auto mon2{std::make_shared<ConstantMonitor>(formulaTrue, 3, false)};
    mon2->registerParent(1, OperatorType::AND, {});
    auto mon{AndMonitor(formula, {mon1, mon2}, 1, false, false)};
    EXPECT_EQ(mon.to_string(), "(false & true)");
}

TEST(TestAndMonitor, testReset) {
    Trace trace = Utils::createTrace({"a"}, {{"a"}, {}}, std::vector<size_t>{0, 1});
    Formula formula = Parser::parse("false & true");
    Formula formulaFalse = Parser::parse("false");
    Formula formulaTrue = Parser::parse("true");
    formula.setId("R1");
    auto mon1{std::make_shared<ConstantMonitor>(formulaFalse, 2, false)};
    mon1->registerParent(1, OperatorType::AND, {});
    auto mon2{std::make_shared<ConstantMonitor>(formulaTrue, 3, false)};
    mon2->registerParent(1, OperatorType::AND, {});
    auto mon{AndMonitor(formula, {mon1, mon2}, 1, false, false)};
    EXPECT_NO_THROW(mon.reset());
}