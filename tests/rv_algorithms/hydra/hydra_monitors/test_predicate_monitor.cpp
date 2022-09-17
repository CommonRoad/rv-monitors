//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2022 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//

#include "../../hydra_2/hydra_monitors/test_predicate_monitor.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra/hydra_monitors/predicate_monitor.h"
#include "monitors/utils/utils.h"
#include "test_constant_monitor.h"

TEST(TestPredicateMonitorHydra, testPredicateEvaluation) {
    auto trace{createTrace()};
    Formula formula = Parser::parse("a(dummy)");
    formula.setId("R1");
    auto mon{PredicateMonitorHydra(formula)};
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, false);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, false);
    EXPECT_EQ(mon.concreteStep().value, false);
}

TEST(TestPredicateMonitorHydra, testPredicateMonitorToString) {
    Formula formula = Parser::parse("a(dummy)");
    formula.setId("R1");
    auto mon{PredicateMonitorHydra(formula)};
    EXPECT_EQ(mon.to_string(), "a(dummy)");
}