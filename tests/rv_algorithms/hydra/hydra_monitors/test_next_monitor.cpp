//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2022 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//

#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra/hydra_monitors/atomic_monitor.h"
#include "monitors/rv_algorithms/hydra/hydra_monitors/next_monitor.h"
#include "monitors/utils/utils.h"
#include "test_or_monitor.h"

TEST(TestNextMonitorHydra, testNextMonitorEvaluationStandard) {
    Trace trace = Utils::createTrace({"a"}, {{"a"}, {}, {"a"}, {"a"}}, std::vector<size_t>{0, 1, 2, 3});
    Formula formula = Parser::parse("X[0, 4](a)");
    Formula formulaA = Parser::parse("a");
    formula.setId("R1");
    auto mon1{std::make_shared<AtomicMonitorHydra>(formulaA)};
    auto mon{NextMonitorHydra(formula, mon1, IntervalTemporalOperator(0, 4))};
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep().value, false);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, false);
    EXPECT_EQ(mon.concreteStep().value, false);
}

TEST(TestNextMonitorHydra, testNextMonitorEvaluationInterval) {
    Trace trace = Utils::createTrace({"a"}, {{"a"}, {}, {"a"}, {"a"}}, std::vector<size_t>{0, 1, 2, 10});
    Formula formula = Parser::parse("X[0, 4](a)");
    Formula formulaA = Parser::parse("a");
    formula.setId("R1");
    auto mon1{std::make_shared<AtomicMonitorHydra>(formulaA)};
    auto mon{NextMonitorHydra(formula, mon1, IntervalTemporalOperator(0, 4))};
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep().value, false);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, false);
    EXPECT_EQ(mon.concreteStep().value, false);
    EXPECT_EQ(mon.concreteStep().value, false);
}

TEST(TestNextMonitorHydra, testToString) {
    Formula formula = Parser::parse("X[0, 4](a)");
    Formula formulaA = Parser::parse("a");
    formula.setId("R1");
    auto mon1{std::make_shared<AtomicMonitorHydra>(formulaA)};
    auto mon{NextMonitorHydra(formula, mon1, IntervalTemporalOperator(0, 4))};
    EXPECT_EQ(mon.to_string(), "X[0, 4](a)");
}

TEST(TestNextMonitorHydra, testReset) {
    Formula formula = Parser::parse("X[0, 4](a)");
    Formula formulaA = Parser::parse("a");
    formula.setId("R1");
    auto mon1{std::make_shared<AtomicMonitorHydra>(formulaA)};
    auto mon{NextMonitorHydra(formula, mon1, IntervalTemporalOperator(0, 4))};
    EXPECT_NO_THROW(mon.reset());
}