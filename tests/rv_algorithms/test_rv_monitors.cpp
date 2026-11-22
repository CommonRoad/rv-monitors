#include "test_rv_monitors.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra_2/hydra.h"
#include "monitors/rv_algorithms/ltl_event_consuming/event_consuming.h"

void TestRVMonitors::SetUp() {
    trace = Utils::createTrace({"a", "b", "c"}, {{"b"}, {"a"}, {"b"}});
    formula = std::make_unique<Formula>(Parser::parse("G(b -> (X(a)))"));
    formula->setId("R1");
}

std::vector<Verdict> TestRVMonitors::executeRVMonitor(const AlgorithmType algorithm) {
    Formula form{*formula};
    auto monitor{RVMonitor({form, MonitorConfiguration(algorithm, false)})};
    std::cout << algorithm;

    return monitor.verify(trace)[form.getId()];
}

TEST_F(TestRVMonitors, EventConsumingLTL) {
    testing::internal::CaptureStdout();
    EXPECT_FALSE(executeRVMonitor(AlgorithmType::event_consuming_ltl).at(0).value);
    const auto consoleOutputAlgorithmType{testing::internal::GetCapturedStdout()};
    EXPECT_EQ(consoleOutputAlgorithmType, "EventConsuming-LTL");
}

TEST_F(TestRVMonitors, HydraMTL) {
    testing::internal::CaptureStdout();
    const std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, false}};
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
    Formula form{*formula};
    auto monitor{RVMonitor({form, MonitorConfiguration(AlgorithmType::hydra_2_mtl, true)})};

    monitor.verify(trace);

    EXPECT_EQ(monitor.getExecutionStatistic().getTraceLengths().at(0), 3);
    EXPECT_EQ(monitor.getExecutionStatistic().getNumExecutions(), 1);
    EXPECT_GT(monitor.getExecutionStatistic().getCompTimes().at(0), 0.0);
    EXPECT_GT(monitor.getExecutionStatistic().getTotalTime(), 0.0);
}

void copyMonitor(const std::shared_ptr<Trace> &tra, AlgorithmType alg) {
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
    copyMonitor(trace, AlgorithmType::hydra_2_mtl);
    copyMonitor(trace, AlgorithmType::hydra_mtl);
    copyMonitor(trace, AlgorithmType::event_consuming_ltl);
    copyMonitor(trace, AlgorithmType::event_consuming_mtl);
    copyMonitor(trace, AlgorithmType::dynamic_programming_ltl);
    copyMonitor(trace, AlgorithmType::btt_fsm);
    copyMonitor(trace, AlgorithmType::mt_fsm);
}

TEST_F(TestRVMonitors, Hydra2Statistics) {
    const auto formula{Parser::parse("G(a | (b S c))", "R1")};
    const auto trace{Utils::createTrace({"a", "b", "c"}, {{"b", "c"}, {"b"}, {"a", "b"}, {"b"}}, {0, 1, 2, 3})};
    auto hydra2{RVMonitor(formula, MonitorConfiguration(AlgorithmType::hydra_2_mtl, false))};
    EXPECT_EQ(hydra2.getNumEvaluations(), 0U);
    hydra2.verify(trace);
    EXPECT_GT(hydra2.getNumEvaluations(), 0U);

    // evaluating the same trace again resets the monitor, so the counts are not doubled
    const auto evaluations{hydra2.getNumEvaluations()};
    const auto stored{hydra2.getNumStoredVerdicts()};
    hydra2.verify(trace);
    EXPECT_EQ(hydra2.getNumEvaluations(), evaluations);
    EXPECT_EQ(hydra2.getNumStoredVerdicts(), stored);

    // matches the underlying algorithm
    auto direct{Hydra2({formula}, true, true, true, globally)};
    direct.evaluate(trace);
    EXPECT_EQ(hydra2.getNumEvaluations(), direct.getNumEvaluations());
    EXPECT_EQ(hydra2.getNumStoredVerdicts(), direct.getNumStoredVerdicts());

    // other algorithms do not collect statistics
    auto dynamic{RVMonitor(*this->formula, MonitorConfiguration(AlgorithmType::dynamic_programming_ltl, false))};
    dynamic.verify(this->trace);
    EXPECT_EQ(dynamic.getNumEvaluations(), 0U);
    EXPECT_EQ(dynamic.getNumStoredVerdicts(), 0U);
}
