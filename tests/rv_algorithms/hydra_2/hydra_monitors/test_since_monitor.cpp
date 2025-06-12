#include "../hydra_test_utils.h"
#include "monitors/collections/domain.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/atomic_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/constant_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/since_monitor.h"
#include "monitors/utils/utils.h"
#include "test_or_monitor.h"

TEST(TestSinceMonitor, testSinceMonitorEvaluation) {
    const std::shared_ptr<Trace> trace =
        Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"b"}, {}, {"a", "b"}, {"a"}, {"a", "b"}, {"a"}, {"b"}},
                           std::vector<size_t>{0, 1, 2, 3, 4, 5, 6, 20, 30});
    Formula formula = Parser::parse("a S[0, 4] b");
    Formula formulaA = Parser::parse("a");
    Formula formulaB = Parser::parse("b");
    formula.setId("R1");
    auto operatorInterval{IntervalTemporalOperator(0, 4)};
    auto mon1{std::make_shared<AtomicMonitor>(formulaA, 2)};
    mon1->registerParent(1);
    auto mon2{std::make_shared<AtomicMonitor>(formulaB, 3)};
    mon2->registerParent(1);
    auto mon{SinceMonitor(formula, mon1, mon2, operatorInterval, 1)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.step(0, 0).value, false);
    EXPECT_EQ(mon.step(0, 1).value, false);
    EXPECT_EQ(mon.step(0, 2).value, true);
    EXPECT_EQ(mon.step(0, 3).value, false);
    EXPECT_EQ(mon.step(0, 4).value, true);
    EXPECT_EQ(mon.step(0, 5).value, true);
    EXPECT_EQ(mon.step(0, 6).value, true);
    EXPECT_EQ(mon.step(0, 7).value, false);
    EXPECT_EQ(mon.step(0, 8).value, true);
}

TEST(TestSinceMonitor, testOnce1) {
    const std::shared_ptr<Trace> trace =
        Utils::createTrace({"a"}, {{"a"}, {"a"}, {"b"}, {}, {"a", "b"}, {"a"}, {"a", "b"}, {"a"}, {"b"}},
                           std::vector<size_t>{0, 1, 2, 3, 4, 5, 6, 20, 30});
    Formula formula = Parser::parse("(true) S[0, 2](a)"); // corresponds to O[0, 2](a)
    Formula formulaA = Parser::parse("true");
    Formula formulaB = Parser::parse("a");
    formula.setId("R1");
    auto operatorInterval{IntervalTemporalOperator(0, 2)};
    auto mon1{std::make_shared<ConstantMonitor>(formulaA, 2)};
    mon1->registerParent(1);
    auto mon2{std::make_shared<AtomicMonitor>(formulaB, 3)};
    mon2->registerParent(1);
    auto mon{SinceMonitor(formula, mon1, mon2, operatorInterval, 1)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.step(0, 0).value, true);
    EXPECT_EQ(mon.step(0, 1).value, true);
    EXPECT_EQ(mon.step(0, 2).value, true);
    EXPECT_EQ(mon.step(0, 3).value, true);
    EXPECT_EQ(mon.step(0, 4).value, true);
    EXPECT_EQ(mon.step(0, 5).value, true);
    EXPECT_EQ(mon.step(0, 6).value, true);
    EXPECT_EQ(mon.step(0, 7).value, true);
    EXPECT_EQ(mon.step(0, 8).value, false);
}

TEST(TestSinceMonitor, testOnce2) {
    const std::shared_ptr<Trace> trace =
        Utils::createTrace({"a"}, {{"a"}, {"a"}, {"b"}, {}, {"b"}, {}, {"a", "b"}, {"a"}, {"b"}},
                           std::vector<size_t>{0, 1, 2, 3, 4, 5, 6, 20, 30});
    Formula formula = Parser::parse("(true) S[0, 2](a)"); // corresponds to O[0, 2](a)
    Formula formulaA = Parser::parse("true");
    Formula formulaB = Parser::parse("a");
    formula.setId("R1");
    auto operatorInterval{IntervalTemporalOperator(0, 2)};
    auto mon1{std::make_shared<ConstantMonitor>(formulaA, 2)};
    mon1->registerParent(1);
    auto mon2{std::make_shared<AtomicMonitor>(formulaB, 3)};
    mon2->registerParent(1);
    auto mon{SinceMonitor(formula, mon1, mon2, operatorInterval, 1)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.step(0, 0).value, true);
    EXPECT_EQ(mon.step(0, 1).value, true);
    EXPECT_EQ(mon.step(0, 2).value, true);
    EXPECT_EQ(mon.step(0, 3).value, true);
    EXPECT_EQ(mon.step(0, 4).value, false);
    EXPECT_EQ(mon.step(0, 5).value, false);
    EXPECT_EQ(mon.step(0, 6).value, true);
    EXPECT_EQ(mon.step(0, 7).value, true);
    EXPECT_EQ(mon.step(0, 8).value, false);
}

TEST(TestSinceMonitor, testSinceMonitorEvaluationWithOffset) {
    const std::shared_ptr<Trace> trace =
        Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"b"}, {}, {"a", "b"}, {"a"}, {"a", "b"}, {"a"}, {"b"}},
                           std::vector<size_t>{0, 1, 2, 3, 4, 5, 6, 20, 30});
    Formula formula = Parser::parse("a S[2, 4] b");
    Formula formulaA = Parser::parse("a");
    Formula formulaB = Parser::parse("b");
    formula.setId("R1");
    auto operatorInterval{IntervalTemporalOperator(2, 4)};
    auto mon1{std::make_shared<AtomicMonitor>(formulaA, 2)};
    mon1->registerParent(1);
    auto mon2{std::make_shared<AtomicMonitor>(formulaB, 3)};
    mon2->registerParent(1);
    auto mon{SinceMonitor(formula, mon1, mon2, operatorInterval, 1)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.step(0, 0).value, false);
    EXPECT_EQ(mon.step(0, 1).value, false);
    EXPECT_EQ(mon.step(0, 2).value, false);
    EXPECT_EQ(mon.step(0, 3).value, false);
    EXPECT_EQ(mon.step(0, 4).value, false);
    EXPECT_EQ(mon.step(0, 5).value, false);
    EXPECT_EQ(mon.step(0, 6).value, true);
    EXPECT_EQ(mon.step(0, 7).value, false);
    EXPECT_EQ(mon.step(0, 8).value, false);
}

TEST(TestSinceMonitor, testToString) {
    Formula formula = Parser::parse("a S[0, 4]  b");
    Formula formulaA = Parser::parse("a");
    Formula formulaB = Parser::parse("b");
    formula.setId("R1");
    auto operatorInterval{IntervalTemporalOperator(0, 4)};
    auto mon1{std::make_shared<AtomicMonitor>(formulaA, 2)};
    mon1->registerParent(1);
    auto mon2{std::make_shared<AtomicMonitor>(formulaB, 3)};
    mon2->registerParent(1);
    auto mon{SinceMonitor(formula, mon1, mon2, operatorInterval, 1)};
    EXPECT_EQ(mon.to_string(), "(a S[0, 4] b)");

    formula = Parser::parse("a S[0, inf]  b");
    formulaA = Parser::parse("a");
    formulaB = Parser::parse("b");
    formula.setId("R1");
    operatorInterval = IntervalTemporalOperator(0);
    mon1 = std::make_shared<AtomicMonitor>(formulaA, 2);
    mon1->registerParent(1);
    mon2 = std::make_shared<AtomicMonitor>(formulaB, 3);
    mon2->registerParent(1);
    mon = SinceMonitor(formula, mon1, mon2, operatorInterval, 1);
    EXPECT_EQ(mon.to_string(), "(a S[0, inf] b)");

    formula = Parser::parse("a S b");
    formulaA = Parser::parse("a");
    formulaB = Parser::parse("b");
    formula.setId("R1");
    operatorInterval = IntervalTemporalOperator();
    mon1 = std::make_shared<AtomicMonitor>(formulaA, 2);
    mon1->registerParent(1);
    mon2 = std::make_shared<AtomicMonitor>(formulaB, 3);
    mon2->registerParent(1);
    mon = SinceMonitor(formula, mon1, mon2, operatorInterval, 1);
    EXPECT_EQ(mon.to_string(), "(a S b)");
}

TEST(TestSinceMonitor, testReset) {
    Formula formula = Parser::parse("a S[0, 4]  b");
    Formula formulaA = Parser::parse("a");
    Formula formulaB = Parser::parse("b");
    formula.setId("R1");
    auto operatorInterval{IntervalTemporalOperator(0, 4)};
    auto mon1{std::make_shared<AtomicMonitor>(formulaA, 2)};
    mon1->registerParent(1);
    auto mon2{std::make_shared<AtomicMonitor>(formulaB, 3)};
    mon2->registerParent(1);
    auto mon{SinceMonitor(formula, mon1, mon2, operatorInterval, 1)};
    EXPECT_NO_THROW(mon.reset());
}

std::vector<std::any> domain2(size_t stepNumber, std::vector<std::vector<std::any>> possibleSets) {
    switch (stepNumber) {
    case 0:
        return {};
    case 1:
        return {};
    case 2:
        return possibleSets[2];
    default:
        return {};
    }
}

bool checkValidityDomain2(size_t stepNumber) {
    switch (stepNumber) {
    case 0:
        return false;
    case 1:
        return false;
    case 2:
        return true;
    default:
        return false;
    }
}

TEST(TestSinceMonitor, testLazyEvaluationComplexPredicateInfinteInterval) {
    auto traceAtomic{Utils::createTrace({"a"}, {{}, {}, {"a"}})};
    std::vector<Event> eventList{traceAtomic->getEvent(0), traceAtomic->getEvent(1), traceAtomic->getEvent(2)};
    const std::shared_ptr<Trace> trace = hydraTestUtils::createTraceWithVariable(
        4,
        {std::make_shared<Function<bool, size_t, size_t>>(
            Function<bool, size_t, size_t>("raise_if_step_less_than_two", {"timeStep", "randomValue"},
                                           hydraTestUtils::raiseIfStepLessThanTwo, hydraTestUtils::twoSizeTArgs))},
        ConstantCollection({ConstantValue{
            "2", 2ul, [](const std::any &val) { return std::to_string(std::any_cast<std::size_t>(val)); }}}),
        eventList);
    std::vector<std::vector<std::any>> possibleSets{{1ul, 2ul, 3ul, 4ul}, {1ul, 2ul, 3ul}, {0ul}, {3ul}};

    auto functionDom{std::make_shared<Function<std::vector<std::any>, size_t>>(Function<std::vector<std::any>, size_t>(
        "testname", {"stepNumber"}, [possibleSets](size_t stepNumber) { return domain2(stepNumber, possibleSets); },
        [](const std::vector<DomainValue> &args) {
            return std::tuple<size_t>{std::any_cast<size_t>(args.at(0).getValue())};
        }))};
    DomainStorage dom1{"D0", functionDom,
                       [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); },
                       [](size_t stepNumber, const std::any &) { return checkValidityDomain2(stepNumber); }};

    Formula formula = Parser::parse("G(a & (E x in D0. ((a) S (raise_if_step_less_than_two(x)))))");
    formula.setId("R1");
    std::vector<Formula> formulas = {formula};
    std::vector<std::vector<Verdict>> expSolutions{{Verdict{0, false}, Verdict{1, false}, Verdict{2, true}}};

    std::vector<std::shared_ptr<DomainStorage>> domains{std::make_shared<DomainStorage>(dom1)};
    auto monitor{Hydra2(formulas, true, true, true, globally, domains,
                        {{"a", {0, 1, 0}}, {"raise_if_step_less_than_two", {100, 0, 1000}}})};
    hydraTestUtils::evalHydra(formulas, expSolutions, monitor, trace, "ttt", {});
}

TEST(TestSinceMonitor, testLazyEvaluationComplexPredicateFiniteInterval) {
    auto traceAtomic{Utils::createTrace({"a"}, {{}, {}, {"a"}})};
    std::vector<Event> eventList{traceAtomic->getEvent(0), traceAtomic->getEvent(1), traceAtomic->getEvent(2)};
    const std::shared_ptr<Trace> trace = hydraTestUtils::createTraceWithVariable(
        4,
        {std::make_shared<Function<bool, size_t, size_t>>(
            Function<bool, size_t, size_t>("raise_if_step_less_than_two", {"timeStep", "randomValue"},
                                           hydraTestUtils::raiseIfStepLessThanTwo, hydraTestUtils::twoSizeTArgs))},
        ConstantCollection({ConstantValue{
            "2", 2ul, [](const std::any &val) { return std::to_string(std::any_cast<std::size_t>(val)); }}}),
        eventList);
    std::vector<std::vector<std::any>> possibleSets{{1ul, 2ul, 3ul, 4ul}, {1ul, 2ul, 3ul}, {0ul}, {3ul}};
    auto functionDom{std::make_shared<Function<std::vector<std::any>, size_t>>(Function<std::vector<std::any>, size_t>(
        "testname", {"stepNumber"}, [possibleSets](size_t stepNumber) { return domain2(stepNumber, possibleSets); },
        [](const std::vector<DomainValue> &args) {
            return std::tuple<size_t>{std::any_cast<size_t>(args.at(0).getValue())};
        }))};
    DomainStorage dom1{"D0", functionDom,
                       [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); },
                       [](size_t stepNumber, const std::any &) { return checkValidityDomain2(stepNumber); }};

    Formula formula = Parser::parse("G(a & (E x in D0. ((a) S[0, 1] (raise_if_step_less_than_two(x)))))");
    formula.setId("R1");
    std::vector<Formula> formulas = {formula};
    std::vector<std::vector<Verdict>> expSolutions{{Verdict{0, false}, Verdict{1, false}, Verdict{2, true}}};

    std::vector<std::shared_ptr<DomainStorage>> domains{std::make_shared<DomainStorage>(dom1)};
    auto monitor{Hydra2(formulas, true, true, true, FormulaEvaluationMode::globally, domains,
                        {{"a", {0, 1, 0}}, {"raise_if_step_less_than_two", {100, 0, 1000}}})};
    hydraTestUtils::evalHydra(formulas, expSolutions, monitor, trace, "ttt", {});
}
