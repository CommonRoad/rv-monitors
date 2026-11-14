#include "test_constant_monitor.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra/hydra_monitors/constant_monitor.h"
#include "monitors/utils/utils.h"

TEST(TestConstantMonitorHydra, testSingleConstant) {
    const std::shared_ptr<Trace> trace = Utils::createTrace({"a"}, {{"a"}, {}}, std::vector<size_t>{0, 1});
    Formula formula = Parser::parse("true");
    formula.setId("R1");
    auto mon{ConstantMonitorHydra(formula)};
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, true);
}

TEST(TestConstantMonitorHydra, testConstantToString) {
    const std::shared_ptr<Trace> trace = Utils::createTrace({"a"}, {{"a"}, {}}, std::vector<size_t>{0, 1});
    Formula formula = Parser::parse("true");
    formula.setId("R1");
    auto mon{ConstantMonitorHydra(formula)};
    EXPECT_EQ(mon.to_string(), "true");
    formula = Parser::parse("false");
    mon = ConstantMonitorHydra(formula);
    EXPECT_EQ(mon.to_string(), "false");
}
