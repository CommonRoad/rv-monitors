#include "../hydra_test_utils.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/existential_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/predicate_monitor.h"
#include "monitors/trace/trace.h"
#include "test_or_monitor.h"

TEST(TestExistentialMonitor, testExistentialMonitorStandard) {
    Trace trace = hydraTestUtils::createTraceWithVariable(
        4, std::make_shared<Function<bool, size_t, size_t>>(
               Function<bool, size_t, size_t>("equal_to_time_step", {"timeStep", "value"},
                                              hydraTestUtils::equal_to_time_step, hydraTestUtils::twoSizeTArgs)));
    Formula formula = Parser::parse("E x in D. equal_to_time_step(x)");
    Formula formulaPredicate = Parser::parse("equal_to_time_step(x)");
    formula.setId("R1");
    DomainStorage dom{"D", {0ul, 1ul, 3ul, 4ul}};

    auto mon1{std::make_shared<PredicateMonitor>(formulaPredicate, 2, false)};
    mon1->registerParent(1);
    std::map<std::string, std::shared_ptr<DomainStorage>> variableValues{{"x", std::make_shared<DomainStorage>(dom)}};
    auto mon{ExistentialMonitor(formula, mon1, 1, variableValues, false, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.step(0, 0).value, true);
    EXPECT_EQ(mon.step(0, 1).value, true);
    EXPECT_EQ(mon.step(0, 2).value, false);
    EXPECT_EQ(mon.step(0, 3).value, true);
    EXPECT_EQ(mon.step(0, 4).value, false);
}

TEST(TestExistentialMonitor, testExistentialMonitorToString) {
    Formula formula = Parser::parse("E x, y in D0, z in D1. p('120', '280', x, y, z)");
    Formula formulaPredicate = Parser::parse("p('120', '280', x, y, z)");
    formula.setId("R1");
    DomainStorage dom1{"D0", std::vector<std::any>{}};
    DomainStorage dom2{"D1", std::vector<std::any>{}};

    auto mon1{std::make_shared<PredicateMonitor>(formulaPredicate, 1, false)};
    std::map<std::string, std::shared_ptr<DomainStorage>> variableValues{{"x", std::make_shared<DomainStorage>(dom1)},
                                                                         {"y", std::make_shared<DomainStorage>(dom1)},
                                                                         {"z", std::make_shared<DomainStorage>(dom2)}};
    auto mon{ExistentialMonitor(formula, mon1, 0, variableValues, false, false)};
    EXPECT_EQ(mon.to_string(), "E x in D0, y in D0, z in D1. p('120', '280', x, y, z)");
}
