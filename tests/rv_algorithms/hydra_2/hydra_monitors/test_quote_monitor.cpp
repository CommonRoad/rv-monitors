#include "../hydra_test_utils.h"
#include "monitors/expression_tree/unary/first_order/quote_quantifier.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/predicate_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/quote_monitor.h"
#include "monitors/trace/trace.h"
#include "test_or_monitor.h"

TEST(TestQuoteMonitor, testQuoteMonitorStandard) {
    Trace trace = hydraTestUtils::createTraceWithVariable(
        6, std::make_shared<Function<bool, size_t, size_t>>(
               Function<bool, size_t, size_t>("higher_than_time_step", {"timeStep", "val"},
                                              hydraTestUtils::higher_than_time_step, hydraTestUtils::twoSizeTArgs)));
    Formula formula = Parser::parse("Q[0.2, 0.7] x in D. higher_than_time_step(x)");
    Formula formulaPredicate = Parser::parse("higher_than_time_step(x)");
    formula.setId("R1");
    DomainStorage dom1{"D", {1ul, 2ul, 3ul, 4ul}};

    auto mon1{std::make_shared<PredicateMonitor>(formulaPredicate, 2, false)};
    mon1->registerParent(1);
    std::map<std::string, std::shared_ptr<DomainStorage>> variableValues{{"x", std::make_shared<DomainStorage>(dom1)}};
    QuoteQuantifier *quantifier = dynamic_cast<QuoteQuantifier *>(formula.getExpr().get());
    auto mon{QuoteMonitor(formula, mon1, 1, variableValues, quantifier->getQuoteInterval().getMin(),
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
    Trace trace = hydraTestUtils::createTraceWithVariable(
        6,
        std::make_shared<Function<bool, size_t, size_t>>(Function<bool, size_t, size_t>(
            "higher_than_zero", {"timeStep", "val"}, hydraTestUtils::higher_than_zero, hydraTestUtils::twoSizeTArgs)));
    Formula formula = Parser::parse("Q[0.5, 0.3] x in D. higher_than_zero(x)");
    Formula formulaPredicate = Parser::parse("higher_than_zero(x)");
    formula.setId("R1");
    DomainStorage dom1{"D", {2ul, 4ul, 8ul}};

    auto mon1{std::make_shared<PredicateMonitor>(formulaPredicate, 2, false)};
    mon1->registerParent(1);
    std::map<std::string, std::shared_ptr<DomainStorage>> variableValues{{"x", std::make_shared<DomainStorage>(dom1)}};
    QuoteQuantifier *quantifier = dynamic_cast<QuoteQuantifier *>(formula.getExpr().get());
    auto mon{QuoteMonitor(formula, mon1, 1, variableValues, quantifier->getQuoteInterval().getMin(),
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

    auto mon1{std::make_shared<PredicateMonitor>(formulaPredicate, 1, false)};
    std::map<std::string, std::shared_ptr<DomainStorage>> variableValues;
    QuoteQuantifier *quantifier = dynamic_cast<QuoteQuantifier *>(formula.getExpr().get());
    auto mon{QuoteMonitor(formula, mon1, 0, variableValues, quantifier->getQuoteInterval().getMin(),
                          quantifier->getQuoteInterval().getMax(), false, false)};
    EXPECT_EQ(mon.to_string(), "Q[0.220000, 0.650000] x in D, y in D. p(x, '20', y)");
}
