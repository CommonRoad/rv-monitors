#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/predicate_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/existential_monitor.h"
#include "monitors/trace/trace.h"
#include "test_or_monitor.h"

TEST(TestExistentialMonitor, testExistentialMonitorStandard) {
    Trace trace = Utils::createTrace({}, {{}, {}, {}, {}}, std::vector<size_t>{0, 1, 2, 3});
    Formula formula = Parser::parse("E x in D. equal_to_time_step(x)");
    Formula formulaPredicate = Parser::parse("equal_to_time_step(x)");
    formula.setId("R1");
    DomainCollection domainCollection;
    domainCollection.addDomain({"D", {"0", "1", "3", "4"}});
    FunctionCollection functionCollection;
    functionCollection.addFunction({"equal_to_time_step", 1,
                                    [](int argc, char *args[]) -> bool {
                                        size_t timeStep = strtol(args[0], nullptr, 10);
                                        size_t val = strtol(args[1], nullptr, 10);
                                        return val == timeStep;
                                    }, true
    });
    auto mon1{std::make_shared<PredicateMonitor>(formulaPredicate, 2, functionCollection, false)};
    mon1->registerParent(1);
    std::map<std::string, std::string> variables{std::make_pair("x", "D")};
    auto mon{ExistentialMonitor(formula, mon1, 1, variables, domainCollection, false, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.step(0, 0).b, true);
    EXPECT_EQ(mon.step(0, 1).b, true);
    EXPECT_EQ(mon.step(0, 2).b, false);
    EXPECT_EQ(mon.step(0, 3).b, true);
    EXPECT_EQ(mon.step(0, 4).b, false);
}

TEST(TestExistentialMonitor, testExistentialMonitorToString) {
    Formula formula = Parser::parse("E x, y in D0, z in D1. p('120', '280', x, y, z)");
    Formula formulaPredicate = Parser::parse("p('120', '280', x, y, z)");
    formula.setId("R1");
    FunctionCollection functionCollection;
    auto mon1{std::make_shared<PredicateMonitor>(formula, 1, functionCollection, false)};
    DomainCollection domainCollection;
    auto mon{ExistentialMonitor(formula, mon1, 0, {}, domainCollection, false, false)};
    EXPECT_EQ(mon.to_string(), "E x in D0, y in D0, z in D1. p('120', '280', x, y, z)");
}
