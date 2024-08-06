//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2022 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//

#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra/hydra_monitors/atomic_monitor.h"
#include "monitors/utils/utils.h"
#include "test_constant_monitor.h"

TEST(TestAtomicMonitorHydra, testAtomicEvaluation) {
    const std::shared_ptr<Trace> trace =
        Utils::createTrace({"a", "b"}, {{"a"}, {"b"}, {"a"}, {}}, std::vector<size_t>{0, 1, 2, 3});
    Formula formula = Parser::parse("a");
    formula.setId("R1");
    auto mon{AtomicMonitorHydra(formula)};
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, false);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, false);
    EXPECT_EQ(mon.concreteStep().value, false);
}

TEST(TestAtomicMonitorHydra, testToString) {
    const std::shared_ptr<Trace> trace =
        Utils::createTrace({"a", "b"}, {{"a"}, {"b"}, {"a"}, {}}, std::vector<size_t>{0, 1, 2, 3});
    Formula formula = Parser::parse("a");
    formula.setId("R1");
    auto mon{AtomicMonitorHydra(formula)};
    EXPECT_EQ(mon.to_string(), "a");
}
