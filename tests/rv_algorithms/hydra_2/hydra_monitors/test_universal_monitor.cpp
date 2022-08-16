#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/atomic_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/predicate_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/universal_monitor.h"
#include "monitors/trace/trace.h"
#include "test_or_monitor.h"

TEST(TestUniversalMonitor, testUniversalMonitorStandard) {
    Trace trace = Utils::createTrace({}, {{}, {}, {}, {}}, std::vector<size_t>{0, 1, 2, 3});
    Formula formula = Parser::parse("A x in D0, y in D1. oneHigher(x, y)");
    Formula formulaPredicate = Parser::parse("oneHigher(x, y)");
    formula.setId("R1");
    std::vector<Domain> domains{{"D0", {"2", "3", "4", "5", "6"}}, {"D1", {"3", "4", "5"}}};
    DomainCollection domainCollection{domains};
    FunctionCollection functionCollection;
    functionCollection.addFunction({"oneHigher", 2,
                                    [](int argc, char *args[]) -> bool {
                                        size_t timeStep = strtol(args[0], nullptr, 10);
                                        size_t fstVal = strtol(args[1], nullptr, 10);
                                        size_t sndVal = strtol(args[2], nullptr, 10);
                                        return fstVal > timeStep or sndVal > timeStep;
                                    }, true
    });
    auto mon1{std::make_shared<PredicateMonitor>(formulaPredicate, 2, functionCollection, false)};
    mon1->registerParent(1);
    std::map<std::string, std::string> variables{std::make_pair("x", "D0"), std::make_pair("y", "D1")};
    auto mon{UniversalMonitor(formula, mon1, 1, variables, domainCollection, false, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.step(0, 0).b, true);
    EXPECT_EQ(mon.step(0, 1).b, true);
    EXPECT_EQ(mon.step(0, 2).b, true);
    EXPECT_EQ(mon.step(0, 3).b, false);
    EXPECT_EQ(mon.step(0, 4).b, false);
}

TEST(TestUniversalMonitor, testUniversalMonitorVariableScope) {
    Trace trace = Utils::createTrace({}, {{}}, std::vector<size_t>{0});
    Formula formula = Parser::parse("A x in D0. A x in D1. isNegative(x)");
    Formula formulaUniversal = Parser::parse("A x in D1. isNegative(x)");
    Formula formulaPredicate = Parser::parse("isNegative(x)");
    formula.setId("R1");
    std::vector<Domain> domains{{"D0", {"1", "2", "3"}}, {"D1", {"-1", "-2", "-3"}}};
    DomainCollection domainCollection{domains};
    FunctionCollection functionCollection;
    functionCollection.addFunction({"isNegative", 1,
                           [](int argc, char *args[]) -> bool {
                               return strtol(args[0], nullptr, 10) < 0;
                           }
    });
    auto mon2{std::make_shared<PredicateMonitor>(formulaPredicate, 3, functionCollection, false)};
    mon2->registerParent(2);
    std::map<std::string, std::string> variables2{std::make_pair("x", "D1")};
    auto mon1{std::make_shared<UniversalMonitor>(formulaUniversal, mon2, 2, variables2, domainCollection, false, false)};
    mon1->registerParent(1);
    std::map<std::string, std::string> variables1{std::make_pair("x", "D0")};
    auto mon{UniversalMonitor(formula, mon1, 1, variables1, domainCollection, false, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.step(0, 0).b, true);
    EXPECT_EQ(mon.step(0, 1).b, false);
}

TEST(TestUniversalMonitor, testUniversalMonitorVariableAndConstValues) {
    Trace trace = Utils::createTrace({}, {{}, {}, {}, {}, {}}, std::vector<size_t>{0, 1, 2, 3, 4});
    Formula formula = Parser::parse("A x in D. unequal(x, '4')");
    Formula formulaPredicate = Parser::parse("unequal(x, '4')");
    formula.setId("R1");
    DomainCollection domainCollection;
    domainCollection.addDomain({"D", {"1", "2", "3"}});
    FunctionCollection functionCollection;
    functionCollection.addFunction({"unequal", 2,
                                    [](int argc, char *args[]) -> bool {
                                        size_t timeStep = strtol(args[0], nullptr, 10);
                                        size_t val0 = strtol(args[1], nullptr, 10);
                                        size_t val1 = strtol(args[2], nullptr, 10);
                                        return val0 + timeStep != val1;
                                    }, true
    });
    auto mon1{std::make_shared<PredicateMonitor>(formulaPredicate, 2, functionCollection, false)};
    mon1->registerParent(1);
    std::map<std::string, std::string> variables{std::make_pair("x", "D")};
    auto mon{UniversalMonitor(formula, mon1, 1, variables, domainCollection, false, true)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.step(0, 0).b, true);
    EXPECT_EQ(mon.step(0, 1).b, false);
    EXPECT_EQ(mon.step(0, 2).b, false);
    EXPECT_EQ(mon.step(0, 3).b, false);
    EXPECT_EQ(mon.step(0, 4).b, true);
}

TEST(TestUniversalMonitor, testUniversalMonitorAtomic) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a", "b", "c"}, {"a", "c"}, {}, {"a", "b", "c"}, {"b"}}, std::vector<size_t>{0, 1, 2, 3, 4});
    Formula formula = Parser::parse("A x in D. x");
    Formula formulaAtomic = Parser::parse("x");
    formula.setId("R1");
    DomainCollection domainCollection;
    domainCollection.addDomain({"D", {"a", "b", "c"}});
    FunctionCollection functionCollection;
    auto mon1{std::make_shared<AtomicMonitor>(formulaAtomic, 2, false)};
    mon1->registerParent(1);
    std::map<std::string, std::string> variables{std::make_pair("x", "D")};
    auto mon{UniversalMonitor(formula, mon1, 1, variables, domainCollection, false, true)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.step(0, 0).b, true);
    EXPECT_EQ(mon.step(0, 1).b, false);
    EXPECT_EQ(mon.step(0, 2).b, false);
    EXPECT_EQ(mon.step(0, 3).b, true);
    EXPECT_EQ(mon.step(0, 4).b, false);
}

TEST(TestUniversalMonitor, testUniversalMonitorToString) {
    Formula formula = Parser::parse("A x, y in D. p(x, y, '100')");
    Formula formulaPredicate = Parser::parse("p(x, y, '100')");
    formula.setId("R1");
    FunctionCollection functionCollection;
    auto mon1{std::make_shared<PredicateMonitor>(formula, 1, functionCollection, false)};
    DomainCollection domainCollection;
    auto mon{UniversalMonitor(formula, mon1, 0, {}, domainCollection, false, false)};
    EXPECT_EQ(mon.to_string(), "A x in D, y in D. p(x, y, '100')");
}

