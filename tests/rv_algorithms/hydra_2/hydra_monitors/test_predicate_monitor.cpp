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
    default:
        return false;
    }
}

std::tuple<size_t> aArgs(std::vector<DomainValue> values) { return {std::any_cast<size_t>(values.at(0).getValue())}; }

const std::shared_ptr<Trace> createTraceWithVariable() {
    std::vector<std::shared_ptr<IFunction<bool>>> test{
        {std::make_shared<Function<bool, size_t>>(Function<bool, size_t>("a", {"size_t"}, a, aArgs))}};
    std::vector<Event> eventList;
    for (size_t timeStepIdx{0}; timeStepIdx < 4; ++timeStepIdx) {
        eventList.emplace_back(timeStepIdx, timeStepIdx == 4 - 1);
    }
    return std::make_shared<Trace>(Trace(eventList, test));
}

TEST(TestPredicateMonitor, testPredicateEvaluation) {
    auto trace{createTraceWithVariable()};
    Formula formula = Parser::parse("a('dummy')");
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

TEST(TestPredicateMonitor, testPredicateEvaluationWithArguments) {
    auto trace{createTraceWithVariable()};
    Formula formula = Parser::parse("a(dummy)");
    formula.setId("R1");
    auto mon{PredicateMonitor(formula, 1, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    size_t val{0};
    mon.addDomainValues({DomainValue("dummy", val, true, [](const std::any &) { return "name"; })});
    EXPECT_EQ(mon.concreteStep(0, 0).value, true);
    mon.reset();
    mon.addDomainValues({DomainValue("dummy", val, true, [](const std::any &) { return "name"; })});
    EXPECT_EQ(mon.step(0, 0).value, true);
    mon.reset();

    val = 1;
    mon.addDomainValues({DomainValue("dummy", val, true, [](const std::any &) { return "name"; })});
    EXPECT_EQ(mon.concreteStep(0, 1).value, false);
    mon.reset();
    mon.addDomainValues({DomainValue("dummy", val, true, [](const std::any &) { return "name"; })});
    EXPECT_EQ(mon.step(0, 1).value, false);
    mon.reset();

    val = 2;
    mon.addDomainValues({DomainValue("dummy", val, true, [](const std::any &) { return "name"; })});
    EXPECT_EQ(mon.concreteStep(0, 2).value, true);
    mon.reset();
    mon.addDomainValues({DomainValue("dummy", val, true, [](const std::any &) { return "name"; })});
    EXPECT_EQ(mon.step(0, 2).value, true);
    mon.reset();

    val = 3;
    mon.addDomainValues({DomainValue("dummy", val, true, [](const std::any &) { return "name"; })});
    EXPECT_EQ(mon.concreteStep(0, 3).value, false);
    mon.reset();
    mon.addDomainValues({DomainValue("dummy", val, true, [](const std::any &) { return "name"; })});
    EXPECT_EQ(mon.step(0, 3).value, false);
    mon.reset();

    val = 4;
    mon.addDomainValues({DomainValue("dummy", val, true, [](const std::any &) { return "name"; })});
    EXPECT_EQ(mon.concreteStep(0, 4).value, false);
    mon.reset();
    mon.addDomainValues({DomainValue("dummy", val, true, [](const std::any &) { return "name"; })});
    EXPECT_EQ(mon.step(0, 4).value, false);
    mon.reset();
}
