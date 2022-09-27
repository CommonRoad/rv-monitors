//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2022 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//

#include "monitors/expression_tree/atomic/constant.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/atomic_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/until_monitor.h"
#include "monitors/utils/utils.h"
#include "test_or_monitor.h"

TEST(TestUntilMonitor, testUntilMonitorEvaluation) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"b"}, {}, {"a", "b"}, {"a"}, {"a", "b"}, {"a"}, {"b"}},
                                     std::vector<size_t>{0, 1, 2, 3, 4, 5, 6, 20, 30});
    Formula formula = Parser::parse("a U[0, 4] b");
    Formula formulaA = Parser::parse("a");
    Formula formulaB = Parser::parse("b");
    formula.setId("R1");
    auto operatorInterval{IntervalTemporalOperator(0, 4)};
    auto mon1{std::make_shared<AtomicMonitor>(formulaA, 2, false)};
    mon1->registerParent(1);
    auto mon2{std::make_shared<AtomicMonitor>(formulaB, 3, false)};
    mon2->registerParent(1);
    auto mon{UntilMonitor(formula, mon1, mon2, operatorInterval, 1, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.step(0, 0).value, true);
    EXPECT_EQ(mon.step(0, 1).value, true);
    EXPECT_EQ(mon.step(0, 2).value, true);
    EXPECT_EQ(mon.step(0, 3).value, false);
    EXPECT_EQ(mon.step(0, 4).value, true);
    EXPECT_EQ(mon.step(0, 5).value, true);
    EXPECT_EQ(mon.step(0, 6).value, true);
    EXPECT_EQ(mon.step(0, 7).value, false);
    EXPECT_EQ(mon.step(0, 8).value, true);
}

TEST(TestUntilMonitor, testToString) {
    Formula formula = Parser::parse("a U[0, 4]  b");
    Formula formulaA = Parser::parse("a");
    Formula formulaB = Parser::parse("b");
    formula.setId("R1");
    auto operatorInterval{IntervalTemporalOperator(0, 4)};
    auto mon1{std::make_shared<AtomicMonitor>(formulaA, 2, false)};
    mon1->registerParent(1);
    auto mon2{std::make_shared<AtomicMonitor>(formulaB, 3, false)};
    mon2->registerParent(1);
    auto mon{UntilMonitor(formula, mon1, mon2, operatorInterval, 1, false)};
    EXPECT_EQ(mon.to_string(), "(a U[0, 4] b)");

    formula = Parser::parse("a U[0, inf]  b");
    formulaA = Parser::parse("a");
    formulaB = Parser::parse("b");
    formula.setId("R1");
    operatorInterval = IntervalTemporalOperator(0);
    mon1 = std::make_shared<AtomicMonitor>(formulaA, 2, false);
    mon1->registerParent(1);
    mon2 = std::make_shared<AtomicMonitor>(formulaB, 3, false);
    mon2->registerParent(1);
    mon = UntilMonitor(formula, mon1, mon2, operatorInterval, 1, false);
    EXPECT_EQ(mon.to_string(), "(a U[0, inf] b)");

    formula = Parser::parse("a U b");
    formulaA = Parser::parse("a");
    formulaB = Parser::parse("b");
    formula.setId("R1");
    operatorInterval = IntervalTemporalOperator();
    mon1 = std::make_shared<AtomicMonitor>(formulaA, 2, false);
    mon1->registerParent(1);
    mon2 = std::make_shared<AtomicMonitor>(formulaB, 3, false);
    mon2->registerParent(1);
    mon = UntilMonitor(formula, mon1, mon2, operatorInterval, 1, false);
    EXPECT_EQ(mon.to_string(), "(a U b)");
}

TEST(TestUntilMonitor, testReset) {
    Formula formula = Parser::parse("a U[0, 4]  b");
    Formula formulaA = Parser::parse("a");
    Formula formulaB = Parser::parse("b");
    formula.setId("R1");
    auto operatorInterval{IntervalTemporalOperator(0, 4)};
    auto mon1{std::make_shared<AtomicMonitor>(formulaA, 2, false)};
    mon1->registerParent(1);
    auto mon2{std::make_shared<AtomicMonitor>(formulaB, 3, false)};
    mon2->registerParent(1);
    auto mon{UntilMonitor(formula, mon1, mon2, operatorInterval, 1, false)};
    EXPECT_NO_THROW(mon.reset());
}