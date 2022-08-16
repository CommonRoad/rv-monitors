//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2022 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//

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
    FunctionCollection functions;
    auto mon{PredicateMonitor(formula, 1, functions, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.concreteStep(0, 0).b, true);
    EXPECT_EQ(mon.step(0, 0).b, true);
    EXPECT_EQ(mon.concreteStep(0, 1).b, false);
    EXPECT_EQ(mon.step(0, 1).b, false);
    EXPECT_EQ(mon.concreteStep(0, 2).b, true);
    EXPECT_EQ(mon.step(0, 2).b, true);
    EXPECT_EQ(mon.concreteStep(0, 3).b, false);
    EXPECT_EQ(mon.step(0, 3).b, false);
    EXPECT_EQ(mon.concreteStep(0, 4).b, false);
    EXPECT_EQ(mon.step(0, 4).b, false);
}

TEST(TestPredicateMonitor, testPredicateEvalConstValue) {
    auto trace{createTrace()};
    Formula formula = Parser::parse("equal('100', '100')");
    formula.setId("R1");
    FunctionCollection functions;
    functions.addFunction({"equal", 2,
                           [](int argc, char *args[]) -> bool {
                               int val0 = strtol(args[0], nullptr, 10);
                               int val1 = strtol(args[1], nullptr, 10);
                               return val0 == val1;
                           }
    });
    auto mon{PredicateMonitor(formula, 1, functions, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    for (size_t i = 0; i < 4; i++)
        EXPECT_EQ(mon.step(0, i).b, true);
    EXPECT_EQ(mon.step(0, 4).b, false);

}

TEST(TestPredicateMonitor, testPredicateMonitorToString) {
    Formula formula = Parser::parse("a(dummy)");
    formula.setId("R1");
    FunctionCollection functions;
    auto mon{PredicateMonitor(formula, 0, functions, false)};
    EXPECT_EQ(mon.to_string(), "a(dummy)");
}