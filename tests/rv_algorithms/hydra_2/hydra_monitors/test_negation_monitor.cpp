//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2022 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//

#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/constant_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/negation_monitor.h"
#include "monitors/utils/utils.h"
#include "test_or_monitor.h"

TEST(TestNegationMonitor, testNegationMonitorFalseEvaluation) {
    Trace trace = Utils::createTrace({"a"}, {{"a"}, {}}, std::vector<size_t>{0, 1});
    Formula formula = Parser::parse("!(true)");
    Formula formulaTrue = Parser::parse("true");
    formula.setId("R1");
    auto mon1{std::make_shared<ConstantMonitor>(formulaTrue, 2, false)};
    mon1->registerParent(1);
    auto mon{NegationMonitor(formula, mon1, 1, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep(0, 0).value, false);
    EXPECT_EQ(mon.step(0, 0).value, false);
    EXPECT_EQ(mon.step(0, 4).value, true);
}

TEST(TestNegationMonitor, testNegationMonitorTrueEvaluation) {
    Trace trace = Utils::createTrace({"a"}, {{"a"}, {}}, std::vector<size_t>{0, 1});
    Formula formula = Parser::parse("!(false)");
    Formula formulaFalse = Parser::parse("false");
    formula.setId("R1");
    auto mon1{std::make_shared<ConstantMonitor>(formulaFalse, 2, false)};
    mon1->registerParent(1);
    auto mon{NegationMonitor(formula, mon1, 1, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep(0, 0).value, true);
    EXPECT_EQ(mon.step(0, 0).value, true);
    EXPECT_EQ(mon.step(0, 4).value, true);
}

TEST(TestNegationMonitor, testToString) {
    Trace trace = Utils::createTrace({"a"}, {{"a"}, {}}, std::vector<size_t>{0, 1});
    Formula formula = Parser::parse("!(false)");
    Formula formulaFalse = Parser::parse("false");
    formula.setId("R1");
    auto mon1{std::make_shared<ConstantMonitor>(formulaFalse, 2, false)};
    mon1->registerParent(1);
    auto mon{NegationMonitor(formula, mon1, 1, false)};
    EXPECT_EQ(mon.to_string(), "!(false)");
}

TEST(TestNegationMonitor, testReset) {
    Trace trace = Utils::createTrace({"a"}, {{"a"}, {}}, std::vector<size_t>{0, 1});
    Formula formula = Parser::parse("!(false)");
    Formula formulaFalse = Parser::parse("false");
    formula.setId("R1");
    auto mon1{std::make_shared<ConstantMonitor>(formulaFalse, 2, false)};
    mon1->registerParent(1);
    auto mon{NegationMonitor(formula, mon1, 1, false)};
    EXPECT_NO_THROW(mon.reset());
}