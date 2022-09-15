#include "test_equality_operator_monitor.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/equality_operator_monitor.h"
#include "monitors/trace/trace.h"
#include "test_or_monitor.h"

TEST(TestEqualityOperatorMonitor, testEqualityOpBothVariables) {
    Trace trace = Utils::createTrace(std::set<std::string>(), {{}, {}, {}});
    Formula formula = Parser::parse("x == y");
    formula.setId("R1");
    auto mon{EqualityOperatorMonitor(formula, 1, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    mon.setVariableValues({std::make_pair("x", Value("7")), std::make_pair("y", Value("7"))});
    EXPECT_EQ(mon.step(0, 0).value, true);
    mon.setVariableValues({std::make_pair("x", Value("7")), std::make_pair("y", Value("9"))});
    EXPECT_EQ(mon.step(0, 1).value, false);
    mon.setVariableValues({std::make_pair("x", Value("9")), std::make_pair("y", Value("9"))});
    EXPECT_EQ(mon.step(0, 2).value, true);
    EXPECT_EQ(mon.step(0, 3).value, false);
}

TEST(TestEqualityOperatorMonitor, testEqualityOpVariableConst) {
    Trace trace = Utils::createTrace(std::set<std::string>(), {{}, {}, {}});
    Formula formula = Parser::parse("x == '10'");
    formula.setId("R1");
    auto mon{EqualityOperatorMonitor(formula, 1, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    mon.setVariableValues({std::make_pair("x", Value("10"))});
    EXPECT_EQ(mon.step(0, 0).value, true);
    mon.setVariableValues({std::make_pair("x", Value("10"))});
    EXPECT_EQ(mon.step(0, 1).value, true);
    mon.setVariableValues({std::make_pair("x", Value("3"))});
    EXPECT_EQ(mon.step(0, 2).value, false);
    EXPECT_EQ(mon.step(0, 3).value, false);
}

TEST(TestEqualityOperatorMonitor, testEqualityOpBothConsts) {
    Trace trace = Utils::createTrace(std::set<std::string>(), {{}, {}, {}});
    Formula formula = Parser::parse("'10' == '10'");
    formula.setId("R1");
    auto mon{EqualityOperatorMonitor(formula, 1, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.step(0, 0).value, true);
    EXPECT_EQ(mon.step(0, 1).value, true);
    EXPECT_EQ(mon.step(0, 2).value, true);
    EXPECT_EQ(mon.step(0, 3).value, false);
}

TEST(TestEqualityOperatorMonitor, testEqualityOpMonitorToString) {
    Formula formula = Parser::parse("'9' == x");
    formula.setId("R1");
    auto mon{EqualityOperatorMonitor(formula, 0, false)};
    EXPECT_EQ(mon.to_string(), "'9' == x");
}