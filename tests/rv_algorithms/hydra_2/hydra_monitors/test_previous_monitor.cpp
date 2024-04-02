#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/atomic_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/previous_monitor.h"
#include "monitors/utils/utils.h"
#include "test_or_monitor.h"

TEST(TestPreviousMonitor, testPreviousMonitorEvaluationStandard) {
    Trace trace = Utils::createTrace({"a"}, {{"a"}, {}, {"a"}, {"a"}}, std::vector<size_t>{0, 1, 2, 3});
    Formula formula = Parser::parse("P[0, 4](a)");
    Formula formulaA = Parser::parse("a");
    formula.setId("R1");
    auto operatorInterval{IntervalTemporalOperator(0, 4)};
    auto mon1{std::make_shared<AtomicMonitor>(formulaA, 2, true)};
    mon1->registerParent(1);
    auto mon{PreviousMonitor(formula, mon1, operatorInterval, 1, true)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep(0, 0).value, false);
    EXPECT_EQ(mon.concreteStep(0, 1).value, true);
    EXPECT_EQ(mon.concreteStep(0, 2).value, false);
    EXPECT_EQ(mon.concreteStep(0, 3).value, true);
    EXPECT_EQ(mon.concreteStep(0, 4).value, false);
}

TEST(TestPreviousMonitor, testPreviousMonitorEvaluationInterval) {
    Trace trace = Utils::createTrace({"a"}, {{"a"}, {}, {"a"}, {"a"}}, std::vector<size_t>{0, 1, 2, 10});
    Formula formula = Parser::parse("P[0, 4](a)");
    Formula formulaA = Parser::parse("a");
    formula.setId("R1");
    auto operatorInterval{IntervalTemporalOperator(0, 4)};
    auto mon1{std::make_shared<AtomicMonitor>(formulaA, 2, false)};
    mon1->registerParent(1);
    auto mon{PreviousMonitor(formula, mon1, operatorInterval, 1, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep(0, 0).value, false);
    EXPECT_EQ(mon.concreteStep(0, 1).value, true);
    EXPECT_EQ(mon.concreteStep(0, 2).value, false);
    EXPECT_EQ(mon.concreteStep(0, 3).value, false);
    EXPECT_EQ(mon.concreteStep(0, 4).value, false);
}

TEST(TestPreviousMonitor, testToString) {
    Formula formula = Parser::parse("P[0, 4](a)");
    Formula formulaA = Parser::parse("a");
    formula.setId("R1");
    auto operatorInterval{IntervalTemporalOperator(0, 4)};
    auto mon1{std::make_shared<AtomicMonitor>(formulaA, 2, false)};
    mon1->registerParent(1);
    auto mon{PreviousMonitor(formula, mon1, operatorInterval, 1, false)};
    EXPECT_EQ(mon.to_string(), "P[0, 4](a)");

    formula = Parser::parse("P[0, inf](a)");
    formulaA = Parser::parse("a");
    formula.setId("R1");
    operatorInterval = IntervalTemporalOperator(0);
    mon1 = std::make_shared<AtomicMonitor>(formulaA, 2, false);
    mon1->registerParent(1);
    mon = PreviousMonitor(formula, mon1, operatorInterval, 1, false);
    EXPECT_EQ(mon.to_string(), "P[0, inf](a)");

    formula = Parser::parse("P(a)");
    formulaA = Parser::parse("a");
    formula.setId("R1");
    operatorInterval = IntervalTemporalOperator();
    mon1 = std::make_shared<AtomicMonitor>(formulaA, 2, false);
    mon1->registerParent(1);
    mon = PreviousMonitor(formula, mon1, operatorInterval, 1, false);
    EXPECT_EQ(mon.to_string(), "P(a)");
}

TEST(TestPreviousMonitor, testReset) {
    Formula formula = Parser::parse("P[0, 4](a)");
    Formula formulaA = Parser::parse("a");
    formula.setId("R1");
    auto operatorInterval{IntervalTemporalOperator(0, 4)};
    auto mon1{std::make_shared<AtomicMonitor>(formulaA, 2, false)};
    mon1->registerParent(1);
    auto mon{PreviousMonitor(formula, mon1, IntervalTemporalOperator(0, 4), 1, false)};
    EXPECT_NO_THROW(mon.reset());
}