#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/and_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/constant_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/negation_monitor.h"
#include "monitors/utils/utils.h"
#include "test_or_monitor.h"

TEST(TestAndMonitor, testSingleAndMonitorFalseEvaluation) {
    const std::shared_ptr<Trace> trace = Utils::createTrace({"a"}, {{"a"}, {}}, std::vector<size_t>{0, 1});
    Formula formula = Parser::parse("false & true");
    Formula formulaFalse = Parser::parse("false");
    Formula formulaTrue = Parser::parse("true");
    formula.setId("R1");
    auto mon1{std::make_shared<ConstantMonitor>(formulaFalse, 2)};
    mon1->registerParent(1);
    auto mon2{std::make_shared<ConstantMonitor>(formulaTrue, 3)};
    mon2->registerParent(1);
    auto mon{AndMonitor(formula, {mon1, mon2}, 1, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep(0, 0).value, false);
    mon.reset();
    EXPECT_EQ(mon.step(0, 0).value, false);
    mon.reset();
    EXPECT_EQ(mon.step(0, 4).value, false);
}

TEST(TestAndMonitor, testSingleAndMonitorTrueEvaluation) {
    const std::shared_ptr<Trace> trace = Utils::createTrace({"a"}, {{"a"}, {}}, std::vector<size_t>{0, 1});
    Formula formula = Parser::parse("true & true");
    Formula formulaTrue = Parser::parse("true");
    formula.setId("R1");
    auto mon1{std::make_shared<ConstantMonitor>(formulaTrue, 2)};
    mon1->registerParent(1);
    auto mon2{std::make_shared<ConstantMonitor>(formulaTrue, 3)};
    mon2->registerParent(1);
    auto mon{AndMonitor(formula, {mon1, mon2}, 1, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep(0, 0).value, true);
    mon.reset();
    EXPECT_EQ(mon.step(0, 0).value, true);
}

TEST(TestAndMonitor, testSingleAndMonitorFalseEvaluationShortCircuiting) {
    const std::shared_ptr<Trace> trace = Utils::createTrace({"a"}, {{"a"}, {}}, std::vector<size_t>{0, 1});
    Formula formula = Parser::parse("false & true");
    Formula formulaFalse = Parser::parse("false");
    Formula formulaTrue = Parser::parse("true");
    formula.setId("R1");
    auto mon1{std::make_shared<ConstantMonitor>(formulaFalse, 2)};
    mon1->registerParent(1);
    auto mon2{std::make_shared<ConstantMonitor>(formulaTrue, 3)};
    mon2->registerParent(1);
    auto mon{AndMonitor(formula, {mon1, mon2}, 1, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep(0, 0).value, false);
    mon.reset();
    EXPECT_EQ(mon.step(0, 0).value, false);
    mon.reset();
    EXPECT_EQ(mon.step(0, 4).value, false);
}

TEST(TestAndMonitor, testSingleAndMonitorTrueEvaluationShortCircuiting) {
    const std::shared_ptr<Trace> trace = Utils::createTrace({"a"}, {{"a"}, {}}, std::vector<size_t>{0, 1});
    Formula formula = Parser::parse("true & true");
    Formula formulaTrue = Parser::parse("true");
    formula.setId("R1");
    auto mon1{std::make_shared<ConstantMonitor>(formulaTrue, 2)};
    mon1->registerParent(1);
    auto mon2{std::make_shared<ConstantMonitor>(formulaTrue, 3)};
    mon2->registerParent(1);
    auto mon{AndMonitor(formula, {mon1, mon2}, 1, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep(0, 0).value, true);
    mon.reset();
    EXPECT_EQ(mon.step(0, 0).value, true);
}

TEST(TestAndMonitor, testSingleAndMonitorShortCircuitingForLoop) {
    const std::shared_ptr<Trace> trace = Utils::createTrace({"a"}, {{"a"}, {}}, std::vector<size_t>{0, 1});
    Formula formula = Parser::parse("!(false) & false");
    Formula formulaFalse = Parser::parse("false");
    Formula formulaNegation = Parser::parse("!(false)");
    formula.setId("R1");
    auto mon1{
        std::make_shared<NegationMonitor>(formulaNegation, std::make_shared<ConstantMonitor>(formulaFalse, 4), 2)};
    mon1->registerParent(1);
    auto mon2{std::make_shared<ConstantMonitor>(formulaFalse, 3)};
    mon2->registerParent(1);
    auto mon{AndMonitor(formula, {mon1, mon2}, 1, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep(0, 0).value, false);
    mon.reset();
    EXPECT_EQ(mon.step(0, 0).value, false);
    mon.reset();
    EXPECT_EQ(mon.step(0, 4).value, false);
    mon.reset();
    EXPECT_EQ(mon.concreteStep(0, 4).value, false);
}

TEST(TestAndMonitor, testToString) {
    const std::shared_ptr<Trace> trace = Utils::createTrace({"a"}, {{"a"}, {}}, std::vector<size_t>{0, 1});
    Formula formula = Parser::parse("false & true");
    Formula formulaFalse = Parser::parse("false");
    Formula formulaTrue = Parser::parse("true");
    formula.setId("R1");
    auto mon1{std::make_shared<ConstantMonitor>(formulaFalse, 2)};
    mon1->registerParent(1);
    auto mon2{std::make_shared<ConstantMonitor>(formulaTrue, 3)};
    mon2->registerParent(1);
    auto mon{AndMonitor(formula, {mon1, mon2}, 1, false)};
    EXPECT_EQ(mon.to_string(), "(false & true)");
}

TEST(TestAndMonitor, testReset) {
    const std::shared_ptr<Trace> trace = Utils::createTrace({"a"}, {{"a"}, {}}, std::vector<size_t>{0, 1});
    Formula formula = Parser::parse("false & true");
    Formula formulaFalse = Parser::parse("false");
    Formula formulaTrue = Parser::parse("true");
    formula.setId("R1");
    auto mon1{std::make_shared<ConstantMonitor>(formulaFalse, 2)};
    mon1->registerParent(1);
    auto mon2{std::make_shared<ConstantMonitor>(formulaTrue, 3)};
    mon2->registerParent(1);
    auto mon{AndMonitor(formula, {mon1, mon2}, 1, false)};
    EXPECT_NO_THROW(mon.reset());
}
