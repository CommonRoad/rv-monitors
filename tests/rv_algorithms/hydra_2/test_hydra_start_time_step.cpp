#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra_2/hydra.h"
#include "monitors/rv_algorithms/rv_monitor.h"
#include "monitors/utils/utils.h"

#include <gtest/gtest.h>

// Evaluation from a start time step: events before it (e.g., the history of an ego vehicle) are context for past
// operators only. They must not be reported, must not cause early termination, and must not be the evaluated step in
// initial_step mode.

namespace {

// time steps 0..5, a is violated at 1 (before the start time step 3) and at 4 (after it), b holds only at 1
std::shared_ptr<Trace> historyTrace() {
    return Utils::createTrace({"a", "b"}, {{"a"}, {"b"}, {"a"}, {"a"}, {}, {"a"}}, {0, 1, 2, 3, 4, 5});
}

std::vector<Hydra2> allConfigs(const std::string &formulaString, const FormulaEvaluationMode mode) {
    std::vector<Hydra2> monitors;
    for (const bool connected : {false, true})
        for (const bool reordering : {false, true})
            for (const bool shortCircuiting : {false, true}) {
                auto formula{Parser::parse(formulaString)};
                formula.setId("R1");
                monitors.emplace_back(std::vector<Formula>{formula}, connected, reordering, shortCircuiting, mode);
            }
    return monitors;
}

std::vector<Verdict> run(Hydra2 &monitor, const size_t start) {
    monitor.setEvaluationStartTimeStep(start);
    return monitor.evaluate(historyTrace()).at("R1");
}

} // namespace

TEST(TestHydra2StartTimeStep, earlyTerminationIgnoresViolationInHistory) {
    for (auto &monitor : allConfigs("G(a)", FormulaEvaluationMode::globally_early_termination)) {
        // without start time step, the violation at time step 1 terminates the evaluation
        const auto all{run(monitor, 0)};
        ASSERT_EQ(all.size(), 1U);
        EXPECT_FALSE(all.at(0).value);
        EXPECT_EQ(all.at(0).timeStamp.value(), 1U);
        // from time step 3 on, the violation at 4 has to be found
        const auto fromThree{run(monitor, 3)};
        ASSERT_EQ(fromThree.size(), 1U);
        EXPECT_FALSE(fromThree.at(0).value);
        EXPECT_EQ(fromThree.at(0).timeStamp.value(), 4U);
    }
}

TEST(TestHydra2StartTimeStep, earlyTerminationSatisfiedAfterViolationInHistory) {
    // from time step 5 on, a holds
    for (auto &monitor : allConfigs("G(a)", FormulaEvaluationMode::globally_early_termination)) {
        const auto verdicts{run(monitor, 5)};
        ASSERT_EQ(verdicts.size(), 1U);
        EXPECT_TRUE(verdicts.at(0).value);
    }
}

TEST(TestHydra2StartTimeStep, globallyReportsOnlyCurrentAndFutureSteps) {
    for (auto &monitor : allConfigs("G(a)", FormulaEvaluationMode::globally)) {
        const auto verdicts{run(monitor, 3)};
        ASSERT_EQ(verdicts.size(), 3U);
        EXPECT_EQ(verdicts.at(0).timeStamp.value(), 3U);
        EXPECT_TRUE(verdicts.at(0).value);
        EXPECT_EQ(verdicts.at(1).timeStamp.value(), 4U);
        EXPECT_FALSE(verdicts.at(1).value);
        EXPECT_EQ(verdicts.at(2).timeStamp.value(), 5U);
        EXPECT_TRUE(verdicts.at(2).value);
    }
}

TEST(TestHydra2StartTimeStep, historyRemainsContextForPastOperators) {
    // b holds only at time step 1, i.e., before the start time step; O(b) at the start time step has to see it
    for (const auto mode : {FormulaEvaluationMode::globally, FormulaEvaluationMode::globally_early_termination}) {
        for (auto &monitor : allConfigs("G(O(b))", mode)) {
            const auto verdicts{run(monitor, 3)};
            ASSERT_FALSE(verdicts.empty());
            for (const auto &verdict : verdicts)
                EXPECT_TRUE(verdict.value);
        }
    }
    // a bounded window that reaches into the history: O[0,2](b) at time step 3 sees time step 1
    for (auto &monitor : allConfigs("G(O[0, 2](b))", FormulaEvaluationMode::globally)) {
        const auto verdicts{run(monitor, 3)};
        ASSERT_EQ(verdicts.size(), 3U);
        EXPECT_TRUE(verdicts.at(0).value);  // 3: b at 1
        EXPECT_FALSE(verdicts.at(1).value); // 4: window {2,3,4}
        EXPECT_FALSE(verdicts.at(2).value); // 5
    }
}

TEST(TestHydra2StartTimeStep, initialStepEvaluatesStartTimeStep) {
    // a is violated at time step 4 only after the start; a at the start time step 3 holds, at 1 it does not
    for (auto &monitor : allConfigs("a", FormulaEvaluationMode::initial_step)) {
        const auto atStart{run(monitor, 3)};
        ASSERT_EQ(atStart.size(), 1U);
        EXPECT_TRUE(atStart.at(0).value);
        EXPECT_EQ(atStart.at(0).timeStamp.value(), 3U);
        const auto atHistory{run(monitor, 4)};
        ASSERT_EQ(atHistory.size(), 1U);
        EXPECT_FALSE(atHistory.at(0).value);
        EXPECT_EQ(atHistory.at(0).timeStamp.value(), 4U);
        const auto atZero{run(monitor, 0)};
        ASSERT_EQ(atZero.size(), 1U);
        EXPECT_EQ(atZero.at(0).timeStamp.value(), 0U);
    }
}

TEST(TestHydra2StartTimeStep, startTimeStepBetweenEvents) {
    // trace with step size 2 (events at 0, 2, 4): evaluation starts at the first event >= start time step
    auto trace{Utils::createTrace({"a"}, {{"a"}, {}, {"a"}}, {0, 2, 4})};
    auto formula{Parser::parse("G(a)")};
    formula.setId("R1");
    Hydra2 monitor({formula}, true, true, true, FormulaEvaluationMode::globally);
    monitor.setEvaluationStartTimeStep(1);
    const auto verdicts{monitor.evaluate(trace).at("R1")};
    ASSERT_EQ(verdicts.size(), 2U);
    EXPECT_EQ(verdicts.at(0).timeStamp.value(), 2U);
    EXPECT_EQ(verdicts.at(1).timeStamp.value(), 4U);
}

TEST(TestHydra2StartTimeStep, startAfterTraceEnd) {
    for (auto &monitor : allConfigs("G(a)", FormulaEvaluationMode::globally))
        EXPECT_TRUE(run(monitor, 10).empty());
    for (auto &monitor : allConfigs("G(a)", FormulaEvaluationMode::globally_early_termination)) {
        const auto verdicts{run(monitor, 10)};
        ASSERT_EQ(verdicts.size(), 1U);
        EXPECT_TRUE(verdicts.at(0).value);
    }
}

// Evaluating from a start time step gives the same verdicts as evaluating everything and dropping the history.
TEST(TestHydra2StartTimeStep, consistentWithFullEvaluation) {
    for (const std::string formula :
         {"G(a -> O[0, 2](b))", "G(b S a)", "G(P(a) | b)", "G(a U[0, 2] b)", "G(H[0, 1](a))"}) {
        for (size_t idx{0}; idx < 8; ++idx) {
            auto full{allConfigs(formula, FormulaEvaluationMode::globally).at(idx)};
            auto partial{allConfigs(formula, FormulaEvaluationMode::globally).at(idx)};
            const auto fullVerdicts{run(full, 0)};
            const auto partialVerdicts{run(partial, 3)};
            ASSERT_EQ(partialVerdicts.size(), 3U) << formula;
            for (size_t k{0}; k < partialVerdicts.size(); ++k) {
                EXPECT_EQ(partialVerdicts.at(k).value, fullVerdicts.at(k + 3).value) << formula << " at " << k + 3;
                EXPECT_EQ(partialVerdicts.at(k).timeStamp, fullVerdicts.at(k + 3).timeStamp) << formula;
            }
        }
    }
}

TEST(TestHydra2StartTimeStep, throughRVMonitor) {
    auto formula{Parser::parse("G(a)")};
    formula.setId("R1");
    MonitorConfiguration config;
    config.algorithm = hydra_2_mtl;
    config.formulaEvaluationMode = FormulaEvaluationMode::globally_early_termination;
    RVMonitor monitor({formula}, config);
    const auto verdicts{monitor.verify(historyTrace(), {}, {}, 3).at("R1")};
    ASSERT_EQ(verdicts.size(), 1U);
    EXPECT_FALSE(verdicts.at(0).value);
    EXPECT_EQ(verdicts.at(0).timeStamp.value(), 4U);
}

TEST(TestHydra2StartTimeStep, otherAlgorithmsRejectStartTimeStep) {
    auto formula{Parser::parse("G(a)")};
    formula.setId("R1");
    RVMonitor monitor({formula}, MonitorConfiguration(AlgorithmType::dynamic_programming_ltl, false));
    EXPECT_THROW(monitor.verify(historyTrace(), {}, {}, 3), std::invalid_argument);
    EXPECT_NO_THROW(monitor.verify(historyTrace(), {}, {}, 0));
}
