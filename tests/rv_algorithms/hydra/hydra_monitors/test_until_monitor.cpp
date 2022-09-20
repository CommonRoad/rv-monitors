//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2022 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//

#include "monitors/expression_tree/atomic/constant.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra/hydra_monitors/atomic_monitor.h"
#include "monitors/rv_algorithms/hydra/hydra_monitors/until_monitor.h"
#include "monitors/utils/utils.h"
#include "test_or_monitor.h"

TEST(TestUntilMonitorHydra, testUntilMonitorEvaluation) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"b"}, {}, {"a", "b"}, {"a"}, {"a", "b"}, {"a"}, {"b"}},
                                     std::vector<size_t>{0, 1, 2, 3, 4, 5, 6, 20, 30});
    Formula formula = Parser::parse("a U[0, 4] b");
    Formula formulaA = Parser::parse("a");
    Formula formulaB = Parser::parse("b");
    formula.setId("R1");
    auto mon1{std::make_shared<AtomicMonitorHydra>(formulaA)};
    auto mon2{std::make_shared<AtomicMonitorHydra>(formulaB)};
    auto mon{UntilMonitorHydra(formula, mon1, mon2, 0, 4)};
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, false);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, false);
    EXPECT_EQ(mon.concreteStep().value, true);
}

TEST(TestUntilMonitorHydra, testToString) {
    Formula formula = Parser::parse("a U[0, 4]  b");
    Formula formulaA = Parser::parse("a");
    Formula formulaB = Parser::parse("b");
    formula.setId("R1");
    auto mon1{std::make_shared<AtomicMonitorHydra>(formulaA)};
    auto mon2{std::make_shared<AtomicMonitorHydra>(formulaB)};
    auto mon{UntilMonitorHydra(formula, mon1, mon2, 0, 4)};
    EXPECT_EQ(mon.to_string(), "(a U[0, 4] b)");
}

TEST(TestUntilMonitorHydra, testReset) {
    Formula formula = Parser::parse("a U[0, 4]  b");
    Formula formulaA = Parser::parse("a");
    Formula formulaB = Parser::parse("b");
    formula.setId("R1");
    auto mon1{std::make_shared<AtomicMonitorHydra>(formulaA)};
    auto mon2{std::make_shared<AtomicMonitorHydra>(formulaB)};
    auto mon{UntilMonitorHydra(formula, mon1, mon2, 0, 4)};
    EXPECT_NO_THROW(mon.reset());
}