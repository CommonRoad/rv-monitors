#include "test_or_monitor.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra/hydra_monitors/constant_monitor.h"
#include "monitors/rv_algorithms/hydra/hydra_monitors/or_monitor.h"
#include "monitors/utils/utils.h"

TEST(TestOrMonitorHydra, testSingleOrMonitor) {
    const std::shared_ptr<Trace> trace = Utils::createTrace({"a"}, {{"a"}, {}}, std::vector<size_t>{0, 1});
    Formula formula = Parser::parse("false | true");
    Formula formulaFalse = Parser::parse("false");
    Formula formulaTrue = Parser::parse("true");
    formula.setId("R1");
    auto mon1{std::make_shared<ConstantMonitorHydra>(formulaFalse)};
    auto mon2{std::make_shared<ConstantMonitorHydra>(formulaTrue)};
    auto mon{OrMonitorHydra(formula, {mon1, mon2})};
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, true);
}

TEST(TestOrMonitorHydra, testToString) {
    const std::shared_ptr<Trace> trace = Utils::createTrace({"a"}, {{"a"}, {}}, std::vector<size_t>{0, 1});
    Formula formula = Parser::parse("false | true");
    Formula formulaFalse = Parser::parse("false");
    Formula formulaTrue = Parser::parse("true");
    formula.setId("R1");
    auto mon1{std::make_shared<ConstantMonitorHydra>(formulaFalse)};
    auto mon2{std::make_shared<ConstantMonitorHydra>(formulaTrue)};
    auto mon{OrMonitorHydra(formula, {mon1, mon2})};
    EXPECT_EQ(mon.to_string(), "(false | true)");
}

TEST(TestOrMonitorHydra, testReset) {
    const std::shared_ptr<Trace> trace = Utils::createTrace({"a"}, {{"a"}, {}}, std::vector<size_t>{0, 1});
    Formula formula = Parser::parse("false | true");
    Formula formulaFalse = Parser::parse("false");
    Formula formulaTrue = Parser::parse("true");
    formula.setId("R1");
    auto mon1{std::make_shared<ConstantMonitorHydra>(formulaFalse)};
    auto mon2{std::make_shared<ConstantMonitorHydra>(formulaTrue)};
    auto mon{OrMonitorHydra(formula, {mon1, mon2})};
    EXPECT_NO_THROW(mon.reset());
}
