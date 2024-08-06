//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2022 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//

#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra/hydra_monitors/constant_monitor.h"
#include "monitors/rv_algorithms/hydra/hydra_monitors/negation_monitor.h"
#include "monitors/utils/utils.h"
#include "test_or_monitor.h"

TEST(TestNegationMonitorHydra, testNegationMonitorFalseEvaluation) {
    const std::shared_ptr<Trace> trace = Utils::createTrace({"a"}, {{"a"}, {}}, std::vector<size_t>{0, 1});
    Formula formula = Parser::parse("!(true)");
    Formula formulaTrue = Parser::parse("true");
    formula.setId("R1");
    auto mon1{std::make_shared<ConstantMonitorHydra>(formulaTrue)};
    auto mon{NegationMonitorHydra(formula, mon1)};
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep().value, false);
    EXPECT_EQ(mon.concreteStep().value, false);
}

TEST(TestNegationMonitorHydra, testNegationMonitorTrueEvaluation) {
    const std::shared_ptr<Trace> trace = Utils::createTrace({"a"}, {{"a"}, {}}, std::vector<size_t>{0, 1});
    Formula formula = Parser::parse("!(false)");
    Formula formulaFalse = Parser::parse("false");
    formula.setId("R1");
    auto mon1{std::make_shared<ConstantMonitorHydra>(formulaFalse)};
    auto mon{NegationMonitorHydra(formula, mon1)};
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, true);
}

TEST(TestNegationMonitorHydra, testToString) {
    const std::shared_ptr<Trace> trace = Utils::createTrace({"a"}, {{"a"}, {}}, std::vector<size_t>{0, 1});
    Formula formula = Parser::parse("!(false)");
    Formula formulaFalse = Parser::parse("false");
    formula.setId("R1");
    auto mon1{std::make_shared<ConstantMonitorHydra>(formulaFalse)};
    auto mon{NegationMonitorHydra(formula, mon1)};
    EXPECT_EQ(mon.to_string(), "!(false)");
}

TEST(TestNegationMonitorHydra, testReset) {
    const std::shared_ptr<Trace> trace = Utils::createTrace({"a"}, {{"a"}, {}}, std::vector<size_t>{0, 1});
    Formula formula = Parser::parse("!(false)");
    Formula formulaFalse = Parser::parse("false");
    formula.setId("R1");
    auto mon1{std::make_shared<ConstantMonitorHydra>(formulaFalse)};
    auto mon{NegationMonitorHydra(formula, mon1)};
    EXPECT_NO_THROW(mon.reset());
}
