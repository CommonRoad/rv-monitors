//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "test_rv_monitors.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/event_consuming/event_consuming.h"

void TestRVMonitors::SetUp() {
    trace = std::make_unique<Trace>(Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"b"}}));
    formula = std::make_unique<Formula>(Parser::parse("G(b -> (X(a)))"));
}

bool TestRVMonitors::executeRVMonitor(AlgorithmType algorithm) {
    Trace tr{*trace};
    Formula fm{*formula};
    auto monitor{RVMonitor(algorithm, fm, false, false)};

    return monitor.verify(tr).at(0).b;
}

TEST_F(TestRVMonitors, EventConsumingLTL) { EXPECT_FALSE(executeRVMonitor(AlgorithmType::event_consuming_ltl)); }

TEST_F(TestRVMonitors, EventConsumingMTL) { EXPECT_FALSE(executeRVMonitor(AlgorithmType::event_consuming_mtl)); }

TEST_F(TestRVMonitors, BTT_FSM_LTL) { EXPECT_FALSE(executeRVMonitor(AlgorithmType::btt_fsm)); }

TEST_F(TestRVMonitors, MT_FSM_LTL) { EXPECT_FALSE(executeRVMonitor(AlgorithmType::mt_fsm)); }

TEST_F(TestRVMonitors, DynamicProgrammingLTL) {
    EXPECT_FALSE(executeRVMonitor(AlgorithmType::dynamic_programming_ltl));
}
