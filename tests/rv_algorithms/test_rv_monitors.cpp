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
    trace = std::make_unique<Trace>(Utils::createTrace({"a", "b", "c"}, {{"b"}, {"a"}, {"b"}}));
    formula = std::make_unique<Formula>(Parser::parse("G(b -> (X(a)))"));
    formula->setId("R1");
}

std::vector<Verdict> TestRVMonitors::executeRVMonitor(AlgorithmType algorithm) {
    Trace tra{*trace};
    Formula form{*formula};
    auto monitor{RVMonitor({form, MonitorConfiguration(algorithm, false)})};
    std::cout << algorithm;

    return monitor.verify(tra)[form.getId()];
}

TEST_F(TestRVMonitors, EventConsumingLTL) {
    testing::internal::CaptureStdout();
    EXPECT_FALSE(executeRVMonitor(AlgorithmType::event_consuming_ltl).at(0).value);
    auto consoleOutputAlgorithmType{testing::internal::GetCapturedStdout()};
    EXPECT_EQ(consoleOutputAlgorithmType, "EventConsuming-LTL");
}

TEST_F(TestRVMonitors, HydraMTL) {
    testing::internal::CaptureStdout();
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, false}};
    auto solution{executeRVMonitor(AlgorithmType::hydra_2_mtl)};
    for (size_t i{0}; i < expSolution.size(); ++i)
        EXPECT_TRUE(expSolution[i] == solution[i]) << "Mismatch at time step: " << i;
    auto consoleOutputAlgorithmType{testing::internal::GetCapturedStdout()};
    EXPECT_EQ(consoleOutputAlgorithmType, "Hydra_2-MTL");
}

TEST_F(TestRVMonitors, BTT_FSM_LTL) {
    testing::internal::CaptureStdout();
    EXPECT_FALSE(executeRVMonitor(AlgorithmType::btt_fsm).at(0).value);
    auto consoleOutputAlgorithmType{testing::internal::GetCapturedStdout()};
    EXPECT_EQ(consoleOutputAlgorithmType, "BTT-FSM*");
}

TEST_F(TestRVMonitors, MT_FSM_LTL) {
    testing::internal::CaptureStdout();
    EXPECT_FALSE(executeRVMonitor(AlgorithmType::mt_fsm).at(0).value);
    auto consoleOutputAlgorithmType{testing::internal::GetCapturedStdout()};
    EXPECT_EQ(consoleOutputAlgorithmType, "MT-FSM*");
}

TEST_F(TestRVMonitors, DynamicProgrammingLTL) {
    testing::internal::CaptureStdout();
    EXPECT_FALSE(executeRVMonitor(AlgorithmType::dynamic_programming_ltl).at(0).value);
    auto consoleOutputAlgorithmType{testing::internal::GetCapturedStdout()};
    EXPECT_EQ(consoleOutputAlgorithmType, "DynamicProgramming");
}

TEST_F(TestRVMonitors, StringToAlgorithm) {
    EXPECT_EQ(stringToAlgorithm("dynamic-programming-ltl"), AlgorithmType::dynamic_programming_ltl);
    EXPECT_EQ(stringToAlgorithm("event-consuming-ltl"), AlgorithmType::event_consuming_ltl);
    EXPECT_EQ(stringToAlgorithm("hydra_2-mtl"), AlgorithmType::hydra_2_mtl);
    EXPECT_EQ(stringToAlgorithm("hydra-mtl"), AlgorithmType::hydra_mtl);
    EXPECT_EQ(stringToAlgorithm("btt-fsm*"), AlgorithmType::btt_fsm);
    EXPECT_EQ(stringToAlgorithm("mt-fsm*"), AlgorithmType::mt_fsm);
    EXPECT_EQ(stringToAlgorithm("event-consuming-mtl"), AlgorithmType::event_consuming_mtl);
    EXPECT_THROW(stringToAlgorithm("test"), std::runtime_error);
}

TEST_F(TestRVMonitors, PerformanceMeasurement) {
    Trace tra{*trace};
    Formula form{*formula};
    auto monitor{RVMonitor({form, MonitorConfiguration(AlgorithmType::hydra_2_mtl, true)})};
    monitor.verify(tra);

    EXPECT_EQ(monitor.getExecutionStatistic().getTotalTraceLength(), 3);
    EXPECT_EQ(monitor.getExecutionStatistic().getNumExecutions(), 1);
    EXPECT_GT(monitor.getExecutionStatistic().getMinTime(), 0.0);
    EXPECT_GT(monitor.getExecutionStatistic().getMaxTime(), 0.0);
    EXPECT_GT(monitor.getExecutionStatistic().getTotalTime(), 0.0);
}

void copyMonitor(Trace &tra, AlgorithmType alg) {
    Formula form{Formula(Parser::parse("G(!(a | b) & !c)"))};
    form.setId("R_U1");
    auto conf1{MonitorConfiguration(alg, false)};
    auto mon1{RVMonitor({form, conf1})};
    auto mon2{mon1};
    mon2.verify(tra);
    RVMonitor monitor = mon1;
    monitor.verify(tra);
}

TEST_F(TestRVMonitors, CopyAndAssignmentConstructor) {
    Trace tra{*trace};
    copyMonitor(tra, AlgorithmType::hydra_2_mtl);
    copyMonitor(tra, AlgorithmType::hydra_mtl);
    copyMonitor(tra, AlgorithmType::event_consuming_ltl);
    copyMonitor(tra, AlgorithmType::event_consuming_mtl);
    copyMonitor(tra, AlgorithmType::dynamic_programming_ltl);
    copyMonitor(tra, AlgorithmType::btt_fsm);
    copyMonitor(tra, AlgorithmType::mt_fsm);
}
