//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "test_rv_monitors.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/ltl_event_consuming/event_consuming.h"

void TestRVMonitors::SetUp() {
    trace = std::make_unique<Trace>(Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"b"}}));
    formula = std::make_unique<Formula>(Parser::parse("G(b -> (X(a)))"));
}

bool TestRVMonitors::executeRVMonitor(AlgorithmType algorithm) {
    Trace tr{*trace};
    Formula fm{*formula};
    auto monitor{RVMonitor(algorithm, fm, false, false)};
    std::cout << algorithm;

    return monitor.verify(tr).at(0).b;
}

TEST_F(TestRVMonitors, EventConsumingLTL) {
    testing::internal::CaptureStdout();
    EXPECT_FALSE(executeRVMonitor(AlgorithmType::event_consuming_ltl));
    auto consoleOutputAlgorithmType{testing::internal::GetCapturedStdout()};
    EXPECT_EQ(consoleOutputAlgorithmType, "EventConsuming-LTL");
}

TEST_F(TestRVMonitors, EventConsumingMTL) {
    testing::internal::CaptureStdout();
    EXPECT_FALSE(executeRVMonitor(AlgorithmType::event_consuming_mtl));
    auto consoleOutputAlgorithmType{testing::internal::GetCapturedStdout()};
    EXPECT_EQ(consoleOutputAlgorithmType, "EventConsuming-MTL");
}

TEST_F(TestRVMonitors, BTT_FSM_LTL) {
    testing::internal::CaptureStdout();
    EXPECT_FALSE(executeRVMonitor(AlgorithmType::btt_fsm));
    auto consoleOutputAlgorithmType{testing::internal::GetCapturedStdout()};
    EXPECT_EQ(consoleOutputAlgorithmType, "BTT-FSM*");
}

TEST_F(TestRVMonitors, MT_FSM_LTL) {
    testing::internal::CaptureStdout();
    EXPECT_FALSE(executeRVMonitor(AlgorithmType::mt_fsm));
    auto consoleOutputAlgorithmType{testing::internal::GetCapturedStdout()};
    EXPECT_EQ(consoleOutputAlgorithmType, "MT-FSM*");
}

TEST_F(TestRVMonitors, DynamicProgrammingLTL) {
    testing::internal::CaptureStdout();
    EXPECT_FALSE(executeRVMonitor(AlgorithmType::dynamic_programming_ltl));
    auto consoleOutputAlgorithmType{testing::internal::GetCapturedStdout()};
    EXPECT_EQ(consoleOutputAlgorithmType, "DynamicProgramming");
}

TEST_F(TestRVMonitors, StringToAlgorithm) {
    EXPECT_EQ(stringToAlgorithm("dynamic-programming-ltl"), AlgorithmType::dynamic_programming_ltl);
    EXPECT_EQ(stringToAlgorithm("event-consuming-ltl"), AlgorithmType::event_consuming_ltl);
    EXPECT_EQ(stringToAlgorithm("event-consuming-mtl"), AlgorithmType::event_consuming_mtl);
    EXPECT_EQ(stringToAlgorithm("btt-fsm*"), AlgorithmType::btt_fsm);
    EXPECT_EQ(stringToAlgorithm("mt-fsm*"), AlgorithmType::mt_fsm);
    EXPECT_THROW(stringToAlgorithm("test"), std::runtime_error);
}

TEST_F(TestRVMonitors, PerformanceMeasurement) {
    Trace tr{*trace};
    Formula fm{*formula};
    auto monitor{RVMonitor(AlgorithmType::dynamic_programming_ltl, fm, false, true)};

    monitor.verify(tr);

    EXPECT_EQ(monitor.getExecutionStatistic().getTotalTraceLength(), 3);
    EXPECT_EQ(monitor.getExecutionStatistic().getNumExecutions(), 1);
    EXPECT_GT(monitor.getExecutionStatistic().getMinTime(), 0.0);
    EXPECT_GT(monitor.getExecutionStatistic().getMaxTime(), 0.0);
    EXPECT_GT(monitor.getExecutionStatistic().getTotalTime(), 0.0);
}
