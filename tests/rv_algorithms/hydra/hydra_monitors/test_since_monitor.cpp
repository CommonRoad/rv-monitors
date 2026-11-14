#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra/hydra_monitors/atomic_monitor.h"
#include "monitors/rv_algorithms/hydra/hydra_monitors/constant_monitor.h"
#include "monitors/rv_algorithms/hydra/hydra_monitors/since_monitor.h"
#include "monitors/utils/utils.h"
#include "test_or_monitor.h"

TEST(TestSinceMonitorHydra, testSinceMonitorEvaluation) {
    const std::shared_ptr<Trace> trace =
        Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"b"}, {}, {"a", "b"}, {"a"}, {"a", "b"}, {"a"}, {"b"}},
                           std::vector<size_t>{0, 1, 2, 3, 4, 5, 6, 20, 30});
    Formula formula = Parser::parse("a S[0, 4] b");
    Formula formulaA = Parser::parse("a");
    Formula formulaB = Parser::parse("b");
    formula.setId("R1");
    auto mon1{std::make_shared<AtomicMonitorHydra>(formulaA)};
    auto mon2{std::make_shared<AtomicMonitorHydra>(formulaB)};
    auto mon{SinceMonitorHydra(formula, mon1, mon2, IntervalTemporalOperator(0, 4))};
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep().value, false);
    EXPECT_EQ(mon.concreteStep().value, false);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, false);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, false);
    EXPECT_EQ(mon.concreteStep().value, true);
}

TEST(TestSinceMonitorHydra, testSinceMonitorEvaluationWithOffset) {
    const std::shared_ptr<Trace> trace =
        Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"b"}, {}, {"a", "b"}, {"a"}, {"a", "b"}, {"a"}, {"b"}},
                           std::vector<size_t>{0, 1, 2, 3, 4, 5, 6, 20, 30});
    Formula formula = Parser::parse("a S[2, 4] b");
    Formula formulaA = Parser::parse("a");
    Formula formulaB = Parser::parse("b");
    formula.setId("R1");
    auto mon1{std::make_shared<AtomicMonitorHydra>(formulaA)};
    auto mon2{std::make_shared<AtomicMonitorHydra>(formulaB)};
    auto mon{SinceMonitorHydra(formula, mon1, mon2, IntervalTemporalOperator(2, 4))};
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep().value, false);
    EXPECT_EQ(mon.concreteStep().value, false);
    EXPECT_EQ(mon.concreteStep().value, false);
    EXPECT_EQ(mon.concreteStep().value, false);
    EXPECT_EQ(mon.concreteStep().value, false);
    EXPECT_EQ(mon.concreteStep().value, false);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, false);
    EXPECT_EQ(mon.concreteStep().value, false);
}

TEST(TestSinceMonitorHydra, testOnce1) {
    const std::shared_ptr<Trace> trace =
        Utils::createTrace({"a"}, {{"a"}, {"a"}, {"b"}, {}, {"a", "b"}, {"a"}, {"a", "b"}, {"a"}, {"b"}},
                           std::vector<size_t>{0, 1, 2, 3, 4, 5, 6, 20, 30});
    Formula formula = Parser::parse("(true) S[0, 2](a)"); // corresponds to O[0, 2](a)
    Formula formulaA = Parser::parse("true");
    Formula formulaB = Parser::parse("a");
    formula.setId("R1");
    auto mon1{std::make_shared<ConstantMonitorHydra>(formulaA)};
    auto mon2{std::make_shared<AtomicMonitorHydra>(formulaB)};
    auto mon{SinceMonitorHydra(formula, mon1, mon2, IntervalTemporalOperator(0, 2))};
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, false);
}

TEST(TestSinceMonitorHydra, testOnce2) {
    const std::shared_ptr<Trace> trace =
        Utils::createTrace({"a"}, {{"a"}, {"a"}, {"b"}, {}, {"b"}, {}, {"a", "b"}, {"a"}, {"b"}},
                           std::vector<size_t>{0, 1, 2, 3, 4, 5, 6, 20, 30});
    Formula formula = Parser::parse("(true) S[0, 2](a)"); // corresponds to O[0, 2](a)
    Formula formulaA = Parser::parse("true");
    Formula formulaB = Parser::parse("a");
    formula.setId("R1");
    auto mon1{std::make_shared<ConstantMonitorHydra>(formulaA)};
    auto mon2{std::make_shared<AtomicMonitorHydra>(formulaB)};
    auto mon{SinceMonitorHydra(formula, mon1, mon2, IntervalTemporalOperator(0, 2))};
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, false);
    EXPECT_EQ(mon.concreteStep().value, false);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, false);
}

TEST(TestSinceMonitorHydra, testToString) {
    Formula formula = Parser::parse("a S[0, 4]  b");
    Formula formulaA = Parser::parse("a");
    Formula formulaB = Parser::parse("b");
    formula.setId("R1");
    auto mon1{std::make_shared<AtomicMonitorHydra>(formulaA)};
    auto mon2{std::make_shared<AtomicMonitorHydra>(formulaB)};
    auto mon{SinceMonitorHydra(formula, mon1, mon2, IntervalTemporalOperator(0, 4))};
    EXPECT_EQ(mon.to_string(), "(a S[0, 4] b)");
}

TEST(TestSinceMonitorHydra, testReset) {
    Formula formula = Parser::parse("a S[0, 4]  b");
    Formula formulaA = Parser::parse("a");
    Formula formulaB = Parser::parse("b");
    formula.setId("R1");
    auto mon1{std::make_shared<AtomicMonitorHydra>(formulaA)};
    auto mon2{std::make_shared<AtomicMonitorHydra>(formulaB)};
    auto mon{SinceMonitorHydra(formula, mon1, mon2, IntervalTemporalOperator(0, 4))};
    EXPECT_NO_THROW(mon.reset());
}
