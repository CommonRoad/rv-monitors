#include "test_constant_monitor.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/constant_monitor.h"
#include "monitors/utils/utils.h"

TEST(TestConstantMonitor, testSingleConstant) {
    const std::shared_ptr<Trace> trace = Utils::createTrace({"a"}, {{"a"}, {}}, std::vector<size_t>{0, 1});
    Formula formula = Parser::parse("true");
    formula.setId("R1");
    auto mon{ConstantMonitor(formula, 1, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep(0, 0).value, true);
    EXPECT_EQ(mon.step(0, 0).value, true);
    EXPECT_EQ(mon.step(0, 4).value, false);
}

TEST(TestConstantMonitor, testConstantToString) {
    const std::shared_ptr<Trace> trace = Utils::createTrace({"a"}, {{"a"}, {}}, std::vector<size_t>{0, 1});
    Formula formula = Parser::parse("true");
    formula.setId("R1");
    auto mon{ConstantMonitor(formula, 0, false)};
    EXPECT_EQ(mon.to_string(), "true");
    formula = Parser::parse("false");
    mon = ConstantMonitor(formula, 0, false);
    EXPECT_EQ(mon.to_string(), "false");
}
