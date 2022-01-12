//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2022 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//

#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/atomic_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/since_monitor.h"
#include "monitors/utils/utils.h"
#include "test_or_monitor.h"

TEST(TestSinceMonitor, testSinceMonitorEvaluation) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"b"}, {}, {"a", "b"}, {"a"}, {"a", "b"}, {"a"}, {"b"}},
                                     std::vector<size_t>{0, 1, 2, 3, 4, 5, 6, 20, 30});
    Formula formula = Parser::parse("a S[0, 4] b");
    Formula formulaA = Parser::parse("a");
    Formula formulaB = Parser::parse("b");
    formula.setId("R1");
    auto mon1{std::make_shared<AtomicMonitor>(formulaA, 2, false)};
    mon1->registerParent(1);
    auto mon2{std::make_shared<AtomicMonitor>(formulaB, 3, false)};
    mon2->registerParent(1);
    auto mon{SinceMonitor(formula, mon1, mon2, 0, 4, 1, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.step(0, 0).b, false);
    EXPECT_EQ(mon.step(0, 1).b, false);
    EXPECT_EQ(mon.step(0, 2).b, true);
    EXPECT_EQ(mon.step(0, 3).b, false);
    EXPECT_EQ(mon.step(0, 4).b, true);
    EXPECT_EQ(mon.step(0, 5).b, true);
    EXPECT_EQ(mon.step(0, 6).b, true);
    EXPECT_EQ(mon.step(0, 7).b, false);
    EXPECT_EQ(mon.step(0, 8).b, true);
}

TEST(TestSinceMonitor, testToString) {
    Formula formula = Parser::parse("a S[0, 4]  b");
    Formula formulaA = Parser::parse("a");
    Formula formulaB = Parser::parse("b");
    formula.setId("R1");
    auto mon1{std::make_shared<AtomicMonitor>(formulaA, 2, false)};
    mon1->registerParent(1);
    auto mon2{std::make_shared<AtomicMonitor>(formulaB, 3, false)};
    mon2->registerParent(1);
    auto mon{SinceMonitor(formula, mon1, mon2, 0, 4, 1, false)};
    EXPECT_EQ(mon.to_string(), "(a S[0, 4] b)");
}

TEST(TestSinceMonitor, testReset) {
    Formula formula = Parser::parse("a S[0, 4]  b");
    Formula formulaA = Parser::parse("a");
    Formula formulaB = Parser::parse("b");
    formula.setId("R1");
    auto mon1{std::make_shared<AtomicMonitor>(formulaA, 2, false)};
    mon1->registerParent(1);
    auto mon2{std::make_shared<AtomicMonitor>(formulaB, 3, false)};
    mon2->registerParent(1);
    auto mon{SinceMonitor(formula, mon1, mon2, 0, 4, 1, false)};
    EXPECT_NO_THROW(mon.reset());
}