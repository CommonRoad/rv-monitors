//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2022 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//

#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/atomic_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/next_monitor.h"
#include "monitors/utils/utils.h"
#include "test_or_monitor.h"

TEST(TestNextMonitor, testNextMonitorEvaluationStandard) {
    Trace trace = Utils::createTrace({"a"}, {{"a"}, {}, {"a"}, {"a"}}, std::vector<size_t>{0, 1, 2, 3});
    Formula formula = Parser::parse("X[0, 4](a)");
    Formula formulaA = Parser::parse("a");
    formula.setId("R1");
    auto operatorInterval{IntervalTemporalOperator(0, 4)};
    auto mon1{std::make_shared<AtomicMonitor>(formulaA, 2, true)};
    mon1->registerParent(1);
    auto mon{NextMonitor(formula, mon1, operatorInterval, 1, true)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep(0, 0).value, false);
    EXPECT_EQ(mon.concreteStep(0, 1).value, true);
    EXPECT_EQ(mon.concreteStep(0, 2).value, true);
    EXPECT_EQ(mon.concreteStep(0, 3).value, false);
    EXPECT_EQ(mon.concreteStep(0, 4).value, false);
}

TEST(TestNextMonitor, testNextMonitorEvaluationInterval) {
    Trace trace = Utils::createTrace({"a"}, {{"a"}, {}, {"a"}, {"a"}}, std::vector<size_t>{0, 1, 2, 10});
    Formula formula = Parser::parse("X[0, 4](a)");
    Formula formulaA = Parser::parse("a");
    formula.setId("R1");
    auto operatorInterval{IntervalTemporalOperator(0, 4)};
    auto mon1{std::make_shared<AtomicMonitor>(formulaA, 2, false)};
    mon1->registerParent(1);
    auto mon{NextMonitor(formula, mon1, operatorInterval, 1, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep(0, 0).value, false);
    EXPECT_EQ(mon.concreteStep(0, 1).value, true);
    EXPECT_EQ(mon.concreteStep(0, 2).value, false);
    EXPECT_EQ(mon.concreteStep(0, 3).value, false);
    EXPECT_EQ(mon.concreteStep(0, 4).value, false);
}

TEST(TestNextMonitor, testToString) {
    Formula formula = Parser::parse("X[0, 4](a)");
    Formula formulaA = Parser::parse("a");
    formula.setId("R1");
    auto operatorInterval{IntervalTemporalOperator(0, 4)};
    auto mon1{std::make_shared<AtomicMonitor>(formulaA, 2, false)};
    mon1->registerParent(1);
    auto mon{NextMonitor(formula, mon1, operatorInterval, 1, false)};
    EXPECT_EQ(mon.to_string(), "X[0, 4](a)");

    formula = Parser::parse("X[0, inf](a)");
    formulaA = Parser::parse("a");
    formula.setId("R1");
    operatorInterval = IntervalTemporalOperator(0);
    mon1 = std::make_shared<AtomicMonitor>(formulaA, 2, false);
    mon1->registerParent(1);
    mon = NextMonitor(formula, mon1, operatorInterval, 1, false);
    EXPECT_EQ(mon.to_string(), "X[0, inf](a)");

    formula = Parser::parse("X(a)");
    formulaA = Parser::parse("a");
    formula.setId("R1");
    operatorInterval = IntervalTemporalOperator();
    mon1 = std::make_shared<AtomicMonitor>(formulaA, 2, false);
    mon1->registerParent(1);
    mon = NextMonitor(formula, mon1, operatorInterval, 1, false);
    EXPECT_EQ(mon.to_string(), "X(a)");
}

TEST(TestNextMonitor, testReset) {
    Formula formula = Parser::parse("X[0, 4](a)");
    Formula formulaA = Parser::parse("a");
    formula.setId("R1");
    auto operatorInterval{IntervalTemporalOperator(0, 4)};
    auto mon1{std::make_shared<AtomicMonitor>(formulaA, 2, false)};
    mon1->registerParent(1);
    auto mon{NextMonitor(formula, mon1, operatorInterval, 1, false)};
    EXPECT_NO_THROW(mon.reset());
}