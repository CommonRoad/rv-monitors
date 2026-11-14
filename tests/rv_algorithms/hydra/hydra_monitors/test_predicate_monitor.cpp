#include "../../hydra_2/hydra_monitors/test_predicate_monitor.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra/hydra_monitors/predicate_monitor.h"
#include "test_constant_monitor.h"

std::shared_ptr<Trace> createTrace() {
    std::vector<Event> eventList;
    const std::map<std::string, std::function<bool(size_t)>> atomicsAndFunctions{{"a(dummy)", a}};
    for (size_t timeStepIdx{0}; timeStepIdx < 4; ++timeStepIdx) {
        event_map_t event;
        for (auto const &[atomic, valueFetcher] : atomicsAndFunctions) {
            auto valueFetcherBound = std::bind(valueFetcher, timeStepIdx);
            event[atomic] = EventValue<bool>(valueFetcherBound);
        }
        eventList.emplace_back(event, timeStepIdx, timeStepIdx == 4 - 1);
        event.clear();
    }
    return std::make_shared<Trace>(Trace(eventList));
}

TEST(TestPredicateMonitorHydra, testPredicateEvaluation) {
    auto trace{createTrace()};
    Formula formula = Parser::parse("a(dummy)");
    formula.setId("R1");
    auto mon{PredicateMonitorHydra(formula)};
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, false);
    EXPECT_EQ(mon.concreteStep().value, true);
    EXPECT_EQ(mon.concreteStep().value, false);
    EXPECT_EQ(mon.concreteStep().value, false);
}

TEST(TestPredicateMonitorHydra, testPredicateMonitorToString) {
    Formula formula = Parser::parse("a(dummy)");
    formula.setId("R1");
    auto mon{PredicateMonitorHydra(formula)};
    EXPECT_EQ(mon.to_string(), "a(dummy)");
}
