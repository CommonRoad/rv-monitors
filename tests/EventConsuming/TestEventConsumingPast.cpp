#include "TestEventConsumingPast.h"
#include "parser/parser.h"
#include "rv_algorithms/EventConsuming/EventConsuming.h"
#include "trace/trace.h"
#include "utils/utils.h"

TEST(TestECPast, violatedEnd) {
    Trace trace =
        Utils::createTrace({"green", "yellow", "red"}, {{"green"}, {"red"}, {"yellow"}, {"green"}, {"red"}, {"green"}});
    Formula formula = Parser::parse("G(green -> P(yellow))");
    EXPECT_EQ(EventConsuming::eventConsumingPast(formula, trace).to_string(), "false");
}

TEST(TestECPast, violatedBeginning) {
    Trace trace = Utils::createTrace({"green", "yellow", "red"},
                                     {{"red"}, {"green"}, {"red"}, {"yellow"}, {"red"}, {"green"}, {"yellow"}});
    Formula formula = Parser::parse("G(green -> P(yellow))");
    EXPECT_EQ(EventConsuming::eventConsumingPast(formula, trace).to_string(), "false");
}

TEST(TestECPast, satisfied) {
    Trace trace =
        Utils::createTrace({"green", "yellow", "red"},
                           {{"yellow"}, {"green"}, {"red"}, {"yellow"}, {"green"}, {"red"}, {"yellow"}, {"green"}});
    Formula formula = Parser::parse("G(green -> P(yellow))");
    EXPECT_EQ(EventConsuming::eventConsumingPast(formula, trace).to_string(), "true");
}