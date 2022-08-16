#include "monitors/expression_tree/unary/first_order/counting_quantifier.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/counting_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/predicate_monitor.h"
#include "monitors/trace/trace.h"
#include "test_or_monitor.h"

TEST(TestCountingMonitor, testCountingMonitorStandard) {
    Trace trace = Utils::createTrace({}, {{}, {}, {}, {}, {}}, std::vector<size_t>{0, 1, 2, 3, 4});
    Formula formula = Parser::parse("C[0.2, 0.7] x in D. higherThanStep(x)");
    Formula formulaPredicate = Parser::parse("higherThanStep(x)");
    formula.setId("R1");
    DomainCollection domainCollection;
    domainCollection.addDomain({"D", {"1", "2", "3", "4"}});
    FunctionCollection functionCollection;
    functionCollection.addFunction({"higherThanStep", 1,
                                    [](int argc, char *args[]) -> bool {
                                        size_t timeStep = strtol(args[0], nullptr, 10);
                                        size_t val = strtol(args[1], nullptr, 10);
                                        return val > timeStep;
                                    }, true
    });
    auto mon1{std::make_shared<PredicateMonitor>(formulaPredicate, 2, functionCollection, false)};
    mon1->registerParent(1);
    std::map<std::string, std::string> variables{std::make_pair("x", "D")};
    CountingQuantifier *quantifier = dynamic_cast<CountingQuantifier *>(formula.getExpr().get());
    auto mon{CountingMonitor(formula, mon1, 1, variables, domainCollection,
                             quantifier->getProbInterval()->getMin(), quantifier->getProbInterval()->getMax(), false, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.step(0, 0).b, false);
    EXPECT_EQ(mon.step(0, 1).b, false);
    EXPECT_EQ(mon.step(0, 2).b, true);
    EXPECT_EQ(mon.step(0, 3).b, true);
    EXPECT_EQ(mon.step(0, 4).b, false);
    EXPECT_EQ(mon.step(0, 5).b, false);
}

TEST(TestCountingMonitor, testWrongIntervalOrder) {
    Trace trace = Utils::createTrace({}, {{}, {}, {}, {}}, std::vector<size_t>{0, 1, 2, 3});
    Formula formula = Parser::parse("C[0.5, 0.3] x in D. higherThanZero(x)");
    Formula formulaPredicate = Parser::parse("higherThanZero(x)");
    formula.setId("R1");
    DomainCollection domainCollection;
    domainCollection.addDomain({"D", {"2", "4", "8"}});
    FunctionCollection functionCollection;
    functionCollection.addFunction({"higherThanZero", 1,
                                    [](int argc, char *args[]) -> bool {
                                        return strtol(args[0], nullptr, 10) > 0;
                                    }
    });
    auto mon1{std::make_shared<PredicateMonitor>(formulaPredicate, 2, functionCollection, false)};
    mon1->registerParent(1);
    std::map<std::string, std::string> variables{std::make_pair("x", "D")};
    CountingQuantifier *quantifier = dynamic_cast<CountingQuantifier *>(formula.getExpr().get());
    auto mon{CountingMonitor(formula, mon1, 1, variables, domainCollection,
                             quantifier->getProbInterval()->getMin(), quantifier->getProbInterval()->getMax(), false, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    for (size_t i = 0; i < 5; i++)
        EXPECT_EQ(mon.step(0, i).b, false);
}

TEST(TestCountingMonitor, testCountingMonitorToString) {
    Formula formula = Parser::parse("C[0.22, 0.65] x, y in D. p(x, '20', y)");
    Formula formulaPredicate = Parser::parse("p(x, '20', y)");
    formula.setId("R1");
    FunctionCollection functionCollection;
    auto mon1{std::make_shared<PredicateMonitor>(formula, 1, functionCollection, false)};
    DomainCollection domainCollection;
    CountingQuantifier *quantifier = dynamic_cast<CountingQuantifier *>(formula.getExpr().get());
    auto mon{CountingMonitor(formula, mon1, 0, {}, domainCollection,
                             quantifier->getProbInterval()->getMin(), quantifier->getProbInterval()->getMax(), false, false)};
    EXPECT_EQ(mon.to_string(), "C[0.220000, 0.650000] x in D, y in D. p(x, '20', y)");
}
