#include "monitors/expression_tree/unary/first_order/quote_quantifier.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/predicate_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/quote_monitor.h"
#include "monitors/trace/trace.h"
#include "test_or_monitor.h"

TEST(TestQuoteMonitor, testQuoteMonitorStandard) {
    Trace trace = Utils::createTrace({}, {{}, {}, {}, {}, {}}, std::vector<size_t>{0, 1, 2, 3, 4});
    Formula formula = Parser::parse("Q[0.2, 0.7] x in D. higher_than_time_step(x)");
    Formula formulaPredicate = Parser::parse("higher_than_time_step(x)");
    formula.setId("R1");
    DomainCollection domainCollection;
    domainCollection.addDomain({"D", {"1", "2", "3", "4"}});
    FunctionCollection functionCollection;
    functionCollection.addFunction({"higher_than_time_step", 1,
                                    [](int argc, char *args[]) -> bool {
                                        size_t timeStep = strtol(args[0], nullptr, 10);
                                        size_t val = strtol(args[1], nullptr, 10);
                                        return val > timeStep;
                                    },
                                    true});
    auto mon1{std::make_shared<PredicateMonitor>(formulaPredicate, 2, functionCollection, false)};
    mon1->registerParent(1);
    std::map<std::string, std::string> variables{std::make_pair("x", "D")};
    QuoteQuantifier *quantifier = dynamic_cast<QuoteQuantifier *>(formula.getExpr().get());
    auto mon{QuoteMonitor(formula, mon1, 1, variables, domainCollection, quantifier->getQuoteInterval().getMin(),
                          quantifier->getQuoteInterval().getMax(), false, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.step(0, 0).value, false);
    EXPECT_EQ(mon.step(0, 1).value, false);
    EXPECT_EQ(mon.step(0, 2).value, true);
    EXPECT_EQ(mon.step(0, 3).value, true);
    EXPECT_EQ(mon.step(0, 4).value, false);
    EXPECT_EQ(mon.step(0, 5).value, false);
}

TEST(TestQuoteMonitor, testWrongIntervalOrder) {
    Trace trace = Utils::createTrace({}, {{}, {}, {}, {}}, std::vector<size_t>{0, 1, 2, 3});
    Formula formula = Parser::parse("Q[0.5, 0.3] x in D. higher_than_zero(x)");
    Formula formulaPredicate = Parser::parse("higher_than_zero(x)");
    formula.setId("R1");
    DomainCollection domainCollection;
    domainCollection.addDomain({"D", {"2", "4", "8"}});
    FunctionCollection functionCollection;
    functionCollection.addFunction(
        {"higher_than_zero", 1, [](int argc, char *args[]) -> bool { return strtol(args[0], nullptr, 10) > 0; }});
    auto mon1{std::make_shared<PredicateMonitor>(formulaPredicate, 2, functionCollection, false)};
    mon1->registerParent(1);
    std::map<std::string, std::string> variables{std::make_pair("x", "D")};
    QuoteQuantifier *quantifier = dynamic_cast<QuoteQuantifier *>(formula.getExpr().get());
    auto mon{QuoteMonitor(formula, mon1, 1, variables, domainCollection, quantifier->getQuoteInterval().getMin(),
                          quantifier->getQuoteInterval().getMax(), false, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    for (size_t i = 0; i < 5; i++)
        EXPECT_EQ(mon.step(0, i).value, false);
}

TEST(TestQuoteMonitor, testQuoteMonitorToString) {
    Formula formula = Parser::parse("Q[0.22, 0.65] x, y in D. p(x, '20', y)");
    Formula formulaPredicate = Parser::parse("p(x, '20', y)");
    formula.setId("R1");
    FunctionCollection functionCollection;
    auto mon1{std::make_shared<PredicateMonitor>(formula, 1, functionCollection, false)};
    DomainCollection domainCollection;
    QuoteQuantifier *quantifier = dynamic_cast<QuoteQuantifier *>(formula.getExpr().get());
    auto mon{QuoteMonitor(formula, mon1, 0, {}, domainCollection, quantifier->getQuoteInterval().getMin(),
                          quantifier->getQuoteInterval().getMax(), false, false)};
    EXPECT_EQ(mon.to_string(), "Q[0.220000, 0.650000] x in D, y in D. p(x, '20', y)");
}
