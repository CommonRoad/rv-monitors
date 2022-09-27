//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2022 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//

#include "test_predicate_monitor.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/predicate_monitor.h"
#include "monitors/utils/utils.h"
#include "test_constant_monitor.h"

bool a(size_t dummy) {
    switch (dummy) {
    case 0:
        return true;
    case 1:
        return false;
    case 2:
        return true;
    case 3:
        return false;
    default:
        return false;
    }
}

Trace createTrace() {
    std::vector<Event> eventList;
    const std::map<std::string, std::function<bool(size_t)>> atomicsAndFunctions{{"a(dummy)", a}};
    for (size_t timeStepIdx{0}; timeStepIdx < 4; ++timeStepIdx) {
        event_map_t event;
        for (auto const &[atomic, valueFetcher] : atomicsAndFunctions) {
            auto valueFetcherBound = std::bind(valueFetcher, timeStepIdx);
            event[atomic] = lazy_bool(valueFetcherBound);
        }
        eventList.emplace_back(event, timeStepIdx, timeStepIdx == 4 - 1);
        event.clear();
    }
    return Trace(eventList);
}

TEST(TestPredicateMonitor, testPredicateEvaluation) {
    auto trace{createTrace()};
    Formula formula = Parser::parse("a(dummy)");
    formula.setId("R1");
    auto mon{PredicateMonitor(formula, 1, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep(0, 0).value, true);
    EXPECT_EQ(mon.step(0, 0).value, true);
    EXPECT_EQ(mon.concreteStep(0, 1).value, false);
    EXPECT_EQ(mon.step(0, 1).value, false);
    EXPECT_EQ(mon.concreteStep(0, 2).value, true);
    EXPECT_EQ(mon.step(0, 2).value, true);
    EXPECT_EQ(mon.concreteStep(0, 3).value, false);
    EXPECT_EQ(mon.step(0, 3).value, false);
    EXPECT_EQ(mon.concreteStep(0, 4).value, false);
    EXPECT_EQ(mon.step(0, 4).value, false);
}

TEST(TestPredicateMonitor, testPredicateMonitorToString) {
    Formula formula = Parser::parse("a(dummy)");
    formula.setId("R1");
    auto mon{PredicateMonitor(formula, 0, false)};
    EXPECT_EQ(mon.to_string(), "a(dummy)");
}
