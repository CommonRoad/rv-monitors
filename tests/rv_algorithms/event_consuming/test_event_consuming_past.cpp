#include "test_event_consuming_past.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/ltl_event_consuming/event_consuming.h"
#include "monitors/trace/trace.h"
#include "monitors/utils/utils.h"

TEST(TestECPast, violatedEnd) {
    Trace trace =
        Utils::createTrace({"green", "yellow", "red"}, {{"green"}, {"red"}, {"yellow"}, {"green"}, {"red"}, {"green"}});
    Formula formula = Parser::parse("G(green -> P(yellow))");
    auto eventConsuming{EventConsumingLTL(formula, true)};
    EXPECT_FALSE(eventConsuming.evaluate(trace).at(0).b);
}

TEST(TestECPast, violatedBeginning) {
    Trace trace = Utils::createTrace({"green", "yellow", "red"},
                                     {{"red"}, {"green"}, {"red"}, {"yellow"}, {"red"}, {"green"}, {"yellow"}});
    Formula formula = Parser::parse("G(green -> P(yellow))");
    auto eventConsuming{EventConsumingLTL(formula, true)};
    EXPECT_FALSE(eventConsuming.evaluate(trace).at(0).b);
}

TEST(TestECPast, satisfied) {
    Trace trace =
        Utils::createTrace({"green", "yellow", "red"},
                           {{"yellow"}, {"green"}, {"red"}, {"yellow"}, {"green"}, {"red"}, {"yellow"}, {"green"}});
    Formula formula = Parser::parse("G(green -> P(yellow))");
    auto eventConsuming{EventConsumingLTL(formula, true)};
    EXPECT_TRUE((eventConsuming.evaluate(trace).at(0).b));
}