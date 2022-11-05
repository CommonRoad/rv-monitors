#include "test_counting_monitor.h"
#include "monitors/expression_tree/unary/first_order/counting_quantifier.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/atomic_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/counting_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/predicate_monitor.h"
#include "monitors/trace/trace.h"

TEST(TestCountingMonitor, testCountingMonitorStandard) {
    Trace trace = Utils::createTrace({"a", "b", "c", "d"}, {{"a", "b", "c"}, {}, {"a", "b"}, {"b", "c"}, {"a", "d"}});
    Formula formula = Parser::parse("C>=2 x in D. x");
    Formula formulaAtomic = Parser::parse("x");
    formula.setId("R1");
    DomainCollection domainCollection;
    domainCollection.addDomain({"D", {"a", "b", "c"}});
    auto mon1{std::make_shared<AtomicMonitor>(formulaAtomic, 2, false)};
    mon1->registerParent(1);
    std::map<std::string, std::string> variables{std::make_pair("x", "D")};
    CountingQuantifier *quantifier = dynamic_cast<CountingQuantifier *>(formula.getExpr().get());
    auto mon{CountingMonitor(formula, mon1, 1, variables, domainCollection, quantifier->getOperator(),
                             quantifier->getNum(), false, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.step(0, 0).value, true);
    EXPECT_EQ(mon.step(0, 1).value, false);
    EXPECT_EQ(mon.step(0, 2).value, true);
    EXPECT_EQ(mon.step(0, 3).value, true);
    EXPECT_EQ(mon.step(0, 4).value, false);
    EXPECT_EQ(mon.step(0, 5).value, false);
}

TEST(TestCountingMonitor, testCountingMonitorToString) {
    Formula formula = Parser::parse("C=4 x, y, z in D. p(x, y, z)");
    Formula formulaPredicate = Parser::parse("p(x, y, z)");
    formula.setId("R1");
    FunctionCollection functionCollection;
    auto mon1{std::make_shared<PredicateMonitor>(formula, 1, functionCollection, false)};
    DomainCollection domainCollection;
    CountingQuantifier *quantifier = dynamic_cast<CountingQuantifier *>(formula.getExpr().get());
    auto mon{CountingMonitor(formula, mon1, 0, {}, domainCollection, quantifier->getOperator(), quantifier->getNum(),
                             false, false)};
    EXPECT_EQ(mon.to_string(), "C=4 x in D, y in D, z in D. p(x, y, z)");
}