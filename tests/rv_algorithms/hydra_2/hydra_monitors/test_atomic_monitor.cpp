//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2022 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//

#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/atomic_monitor.h"
#include "monitors/utils/utils.h"
#include "test_constant_monitor.h"

TEST(TestAtomicMonitor, testAtomicEvaluation) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"b"}, {"a"}, {}}, std::vector<size_t>{0, 1, 2, 3});
    Formula formula = Parser::parse("a");
    formula.setId("R1");
    auto mon{AtomicMonitor(formula, 1, false)};
    mon.registerParent(0, OperatorType::BASE, {});
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep(0, 0).value, true);
    EXPECT_EQ(mon.step(0, 0).value, true);
    EXPECT_EQ(mon.concreteStep(0, 1).value, false);
    EXPECT_EQ(mon.step(0, 1).value, false);
    EXPECT_EQ(mon.concreteStep(0, 2).value, true);
    EXPECT_EQ(mon.step(0, 2).value, true);
    EXPECT_EQ(mon.concreteStep(0, 3).value, false);
    EXPECT_EQ(mon.step(0, 3).value, false);
    EXPECT_EQ(mon.concreteStep(0, 4).value, false);
    EXPECT_EQ(mon.step(0, 4).value, false);
}

TEST(TestAtomicMonitor, testToString) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"b"}, {"a"}, {}}, std::vector<size_t>{0, 1, 2, 3});
    Formula formula = Parser::parse("a");
    formula.setId("R1");
    auto mon{AtomicMonitor(formula, 0, false)};
    EXPECT_EQ(mon.to_string(), "a");
}