#include "../hydra_test_utils.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/atomic_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/predicate_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/universal_monitor.h"
#include "monitors/trace/trace.h"
#include "test_or_monitor.h"

bool one_higher(size_t timeStep, size_t fstVal, size_t sndVal) { return fstVal > timeStep or sndVal > timeStep; }

std::tuple<size_t, size_t, size_t> threeSizeTArgs(std::vector<DomainValue> values) {
    return {std::any_cast<size_t>(values.at(0).getValue()), std::any_cast<size_t>(values.at(1).getValue()),
            std::any_cast<size_t>(values.at(2).getValue())};
}

std::tuple<size_t, size_t, std::string> twoSizeTOneString(std::vector<DomainValue> values) {
    return {std::any_cast<size_t>(values.at(0).getValue()), std::any_cast<size_t>(values.at(1).getValue()),
            std::any_cast<std::string>(values.at(2).getValue())};
}

bool is_negative(size_t timeStep, int val) { return val < 0; }

bool name_is_hello(size_t timeStep, const std::string &val) { return val == "hello"; }

std::tuple<size_t, std::string> name_is_helloArgs(std::vector<DomainValue> values) {
    return {std::any_cast<size_t>(values.at(0).getValue()), std::any_cast<const char *>(values.at(1).getValue())};
}

std::tuple<size_t, int> is_negativeArgs(std::vector<DomainValue> values) {
    return {std::any_cast<size_t>(values.at(0).getValue()), std::any_cast<int>(values.at(1).getValue())};
}

bool unequal(size_t timeStep, size_t val0, const std::string &val1) { return val0 + timeStep != std::stoi(val1); }

std::vector<std::any> createDomain2(size_t stepNumber) { return {1ul, 2ul, 3ul, 4ul, 5ul, 6ul}; }

std::vector<std::any> createDomain3(size_t stepNumber) { return {1ul, 2ul, 3ul}; }

std::vector<std::any> createDomainFunc(const std::string &name) {
    if (name == "a")
        return {"hello"};
    else if (name == "b")
        return {"hello"};
    else
        return {"hello"};
}

TEST(TestUniversalMonitor, testUniversalMonitorStandard) {

    const std::shared_ptr<Trace> trace = hydraTestUtils::createTraceWithVariable(
        4, {std::make_shared<Function<bool, size_t, size_t, size_t>>(Function<bool, size_t, size_t, size_t>(
               "one_higher", {"timeStep", "fstVal", "sndVal"}, one_higher, threeSizeTArgs))});
    Formula formula = Parser::parse("A x in D0, y in D1. one_higher(x, y)");
    Formula formulaPredicate = Parser::parse("one_higher(x, y)");
    formula.setId("R1");
    DomainStorage dom1{"D0", {1ul, 2ul, 3ul, 4ul, 5ul, 6ul}, [](const std::any &val) {
                           return std::to_string(std::any_cast<size_t>(val));
                       }};
    DomainStorage dom2{
        "D1", {3ul, 4ul, 5ul}, [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); }};

    auto mon1{std::make_shared<PredicateMonitor>(formulaPredicate, 2, false)};
    mon1->registerParent(1);
    std::map<std::string, std::shared_ptr<DomainStorage>> variableValues{{"x", std::make_shared<DomainStorage>(dom1)},
                                                                         {"y", std::make_shared<DomainStorage>(dom2)}};
    auto mon{UniversalMonitor(formula, mon1, 1, variableValues, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.step(0, 0).value, true);
    EXPECT_EQ(mon.step(0, 1).value, true);
    EXPECT_EQ(mon.step(0, 2).value, true);
    EXPECT_EQ(mon.step(0, 3).value, false);
    EXPECT_EQ(mon.step(0, 4).value, false); // trace length == 4 -> false

    std::vector<std::vector<DomainValue>> expectedDomainValues{
        {{"x", 1ul, true, [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); }},
         {"y", 3ul, true, [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); }}},
        {{"x", 2ul, true, [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); }},
         {"y", 3ul, true, [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); }}},
        {{"x", 3ul, true, [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); }},
         {"y", 3ul, true, [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); }}},
    };
    auto domRes{mon.getViolatingDomainValues()};
    EXPECT_EQ(std::any_cast<size_t>(expectedDomainValues.at(0).at(0).getValue()),
              std::any_cast<size_t>(domRes.at(3).at(0).at(0).getValue()));
    EXPECT_EQ(std::any_cast<size_t>(expectedDomainValues.at(0).at(1).getValue()),
              std::any_cast<size_t>(domRes.at(3).at(0).at(1).getValue()));
    EXPECT_EQ(std::any_cast<size_t>(expectedDomainValues.at(1).at(0).getValue()),
              std::any_cast<size_t>(domRes.at(3).at(1).at(0).getValue()));
    EXPECT_EQ(std::any_cast<size_t>(expectedDomainValues.at(1).at(1).getValue()),
              std::any_cast<size_t>(domRes.at(3).at(1).at(1).getValue()));
    EXPECT_EQ(std::any_cast<size_t>(expectedDomainValues.at(2).at(0).getValue()),
              std::any_cast<size_t>(domRes.at(3).at(2).at(0).getValue()));
    EXPECT_EQ(std::any_cast<size_t>(expectedDomainValues.at(2).at(1).getValue()),
              std::any_cast<size_t>(domRes.at(3).at(2).at(1).getValue()));
}

TEST(TestUniversalMonitor, testUniversalMonitorStandardDynamicDomain) {
    const std::shared_ptr<Trace> trace = hydraTestUtils::createTraceWithVariable(
        4, {std::make_shared<Function<bool, size_t, size_t, size_t>>(Function<bool, size_t, size_t, size_t>(
               "one_higher", {"timeStep", "fstVal", "sndVal"}, one_higher, threeSizeTArgs))});
    Formula formula = Parser::parse("A x in D0, y in D1. one_higher(x, y)");
    Formula formulaPredicate = Parser::parse("one_higher(x, y)");
    formula.setId("R1");
    auto functionDom{std::make_shared<Function<std::vector<std::any>, size_t>>(Function<std::vector<std::any>, size_t>(
        "testname", {"stepNumber"}, createDomain2, [](const std::vector<DomainValue> &args) {
            return std::tuple<size_t>{std::any_cast<size_t>(args.at(0).getValue())};
        }))};
    DomainStorage dom1{"D0", functionDom,
                       [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); },
                       [](size_t, const std::any &) { return true; }};
    DomainStorage dom2{
        "D1", {3ul, 4ul, 5ul}, [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); }};

    auto mon1{std::make_shared<PredicateMonitor>(formulaPredicate, 2, false)};
    mon1->registerParent(1);
    std::map<std::string, std::shared_ptr<DomainStorage>> variableValues{{"x", std::make_shared<DomainStorage>(dom1)},
                                                                         {"y", std::make_shared<DomainStorage>(dom2)}};
    auto mon{UniversalMonitor(formula, mon1, 1, variableValues, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.step(0, 0).value, true);
    EXPECT_EQ(mon.step(0, 1).value, true);
    EXPECT_EQ(mon.step(0, 2).value, true);
    EXPECT_EQ(mon.step(0, 3).value, false);
    EXPECT_EQ(mon.step(0, 4).value, false);
}

TEST(TestUniversalMonitor, testUniversalMonitorVariableScope) {
    const std::shared_ptr<Trace> trace = hydraTestUtils::createTraceWithVariable(
        4, {std::make_shared<Function<bool, size_t, int>>(
               Function<bool, size_t, int>("is_negative", {"timeStep", "val"}, is_negative, is_negativeArgs))});
    Formula formula = Parser::parse("A x in D0. A x in D1. is_negative(x)");
    Formula formulaUniversal = Parser::parse("A x in D1. is_negative(x)");
    Formula formulaPredicate = Parser::parse("is_negative(x)");
    formula.setId("R1");
    DomainStorage dom1{
        "D0", {1ul, 2ul, 3ul}, [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); }};
    DomainStorage dom2{"D1", {-4, -5, -6}, [](const std::any &val) { return std::to_string(std::any_cast<int>(val)); }};

    auto mon2{std::make_shared<PredicateMonitor>(formulaPredicate, 3, false)};
    mon2->registerParent(2);
    std::map<std::string, std::shared_ptr<DomainStorage>> variableValues1{{"x", std::make_shared<DomainStorage>(dom1)}};
    std::map<std::string, std::shared_ptr<DomainStorage>> variableValues2{{"x", std::make_shared<DomainStorage>(dom2)}};

    auto mon1{std::make_shared<UniversalMonitor>(formulaUniversal, mon2, 2, variableValues1, false)};
    mon1->registerParent(1);
    auto mon{UniversalMonitor(formula, mon1, 1, variableValues2, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.step(0, 0).value, true);
    EXPECT_EQ(mon.step(0, 1).value, true);
}

TEST(TestUniversalMonitor, testUniversalMonitorDynamicDomainWithArgumentsTrue) {
    const std::shared_ptr<Trace> trace = hydraTestUtils::createTraceWithVariable(
        4, {std::make_shared<Function<bool, size_t, std::string>>(Function<bool, size_t, std::string>(
               "name_is_hello", {"stepNumber", "val"}, name_is_hello, name_is_helloArgs))});
    Formula formula = Parser::parse("A x in D0. A y in extract_names(x). name_is_hello(y)");
    Formula formulaUniversal = Parser::parse("A y in extract_names(x). name_is_hello(y)");
    Formula formulaPredicate = Parser::parse("name_is_hello(y)");
    formula.setId("R1");
    DomainStorage dom1{"D0", {"a", "b", "c"}, [](const std::any &val) { return std::any_cast<const char *>(val); }};
    auto functionDom{
        std::make_shared<Function<std::vector<std::any>, std::string>>(Function<std::vector<std::any>, std::string>(
            "testname", {"x"}, createDomainFunc, [](const std::vector<DomainValue> &args) {
                return std::tuple<std::string>{std::any_cast<const char *>(args.at(0).getValue())};
            }))};
    auto dom2 = DomainStorage(
        "extract_names", functionDom, [](const std::any &val) { return std::any_cast<const char *>(val); },
        [](size_t, const std::any &) { return true; });

    auto mon2{std::make_shared<PredicateMonitor>(formulaPredicate, 3, false)};
    mon2->registerParent(2);
    std::map<std::string, std::shared_ptr<DomainStorage>> variableValues1{{"x", std::make_shared<DomainStorage>(dom1)}};
    std::map<std::string, std::shared_ptr<DomainStorage>> variableValues2{{"y", std::make_shared<DomainStorage>(dom2)}};

    auto mon1{std::make_shared<UniversalMonitor>(formulaUniversal, mon2, 2, variableValues2, false)};
    mon1->registerParent(1);
    auto mon{UniversalMonitor(formula, mon1, 1, variableValues1, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    mon.addDomainValues({}); // triggers adding of domain values, e.g., to predicates
    EXPECT_EQ(mon.step(0, 0).value, true);
    EXPECT_EQ(mon.step(0, 1).value, true);
}

TEST(TestUniversalMonitor, testUniversalMonitorDynamicDomainWithArgumentsFalse) {
    const std::shared_ptr<Trace> trace = hydraTestUtils::createTraceWithVariable(
        4, {std::make_shared<Function<bool, size_t, std::string>>(Function<bool, size_t, std::string>(
               "name_is_hello", {"stepNumber", "val"}, name_is_hello, name_is_helloArgs))});
    Formula formula = Parser::parse("A x in D0. A y in extract_names(x). !name_is_hello(y)");
    Formula formulaUniversal = Parser::parse("A y in extract_names(x). name_is_hello(y)");
    Formula formulaPredicate = Parser::parse("name_is_hello(y)");
    formula.setId("R1");
    DomainStorage dom1{"D0", {"a", "b", "c"}, [](const std::any &val) { return std::any_cast<const char *>(val); }};
    auto functionDom{
        std::make_shared<Function<std::vector<std::any>, std::string>>(Function<std::vector<std::any>, std::string>(
            "testname", {"x"}, createDomainFunc, [](const std::vector<DomainValue> &args) {
                return std::tuple<std::string>{std::any_cast<const char *>(args.at(0).getValue())};
            }))};
    auto dom2 = DomainStorage(
        "extract_names", functionDom, [](const std::any &val) { return std::any_cast<const char *>(val); },
        [](size_t, const std::any &) { return true; });

    auto mon2{std::make_shared<PredicateMonitor>(formulaPredicate, 3, false)};
    mon2->registerParent(2);
    std::map<std::string, std::shared_ptr<DomainStorage>> variableValues1{{"x", std::make_shared<DomainStorage>(dom1)}};
    std::map<std::string, std::shared_ptr<DomainStorage>> variableValues2{{"y", std::make_shared<DomainStorage>(dom2)}};

    auto mon1{std::make_shared<UniversalMonitor>(formulaUniversal, mon2, 2, variableValues2, false)};
    mon1->registerParent(1);
    auto mon{UniversalMonitor(formula, mon1, 1, variableValues1, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    mon.addDomainValues({}); // triggers adding of domain values, e.g., to predicates
    EXPECT_EQ(mon.step(0, 0).value, true);
    EXPECT_EQ(mon.step(0, 1).value, true);
}

TEST(TestUniversalMonitor, testUniversalMonitorVariableScopeDynamicDomain) {
    const std::shared_ptr<Trace> trace = hydraTestUtils::createTraceWithVariable(
        4, {std::make_shared<Function<bool, size_t, int>>(
               Function<bool, size_t, int>("is_negative", {"timeStep", "val"}, is_negative, is_negativeArgs))});
    Formula formula = Parser::parse("A x in D0. A x in D1. is_negative(x)");
    Formula formulaUniversal = Parser::parse("A x in D1. is_negative(x)");
    Formula formulaPredicate = Parser::parse("is_negative(x)");
    formula.setId("R1");
    auto functionDom{std::make_shared<Function<std::vector<std::any>, size_t>>(Function<std::vector<std::any>, size_t>(
        "testname", {"stepNumber"}, createDomain2, [](const std::vector<DomainValue> &args) {
            return std::tuple<size_t>{std::any_cast<size_t>(args.at(0).getValue())};
        }))};
    DomainStorage dom1{"D0", functionDom,
                       [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); },
                       [](size_t, const std::any &) { return true; }};
    DomainStorage dom2{"D1", {-4, -5, -6}, [](const std::any &val) { return std::to_string(std::any_cast<int>(val)); }};

    auto mon2{std::make_shared<PredicateMonitor>(formulaPredicate, 3, false)};
    mon2->registerParent(2);
    std::map<std::string, std::shared_ptr<DomainStorage>> variableValues1{{"x", std::make_shared<DomainStorage>(dom1)}};
    std::map<std::string, std::shared_ptr<DomainStorage>> variableValues2{{"x", std::make_shared<DomainStorage>(dom2)}};

    auto mon1{std::make_shared<UniversalMonitor>(formulaUniversal, mon2, 2, variableValues1, false)};
    mon1->registerParent(1);
    auto mon{UniversalMonitor(formula, mon1, 1, variableValues2, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.step(0, 0).value, true);
    EXPECT_EQ(mon.step(0, 1).value, true);
}

TEST(TestUniversalMonitor, testUniversalMonitorVariableAndConstValues) {
    const std::shared_ptr<Trace> trace = hydraTestUtils::createTraceWithVariable(
        5,
        {std::make_shared<Function<bool, size_t, size_t, std::string>>(Function<bool, size_t, size_t, std::string>(
            "unequal", {"timeStep", "val0", "val1"}, unequal, twoSizeTOneString))},
        ConstantCollection({ConstantValue{std::string("4"), std::string("4"),
                                          [](const std::any &val) { return std::any_cast<std::string>(val); }}}));

    Formula formula = Parser::parse("A x in D. unequal(x, '4')");
    Formula formulaPredicate = Parser::parse("unequal(x, '4')");
    formula.setId("R1");
    DomainStorage dom1{
        "D", {1ul, 2ul, 3ul}, [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); }};

    auto mon1{std::make_shared<PredicateMonitor>(formulaPredicate, 2, false)};
    mon1->registerParent(1);
    std::map<std::string, std::shared_ptr<DomainStorage>> variableValues{{"x", std::make_shared<DomainStorage>(dom1)}};
    auto mon{UniversalMonitor(formula, mon1, 1, variableValues, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.step(0, 0).value, true);
    EXPECT_EQ(mon.step(0, 1).value, false);
    EXPECT_EQ(mon.step(0, 2).value, false);
    EXPECT_EQ(mon.step(0, 3).value, false);
    EXPECT_EQ(mon.step(0, 4).value, true);
}

TEST(TestUniversalMonitor, testUniversalMonitorAtomic) {
    const std::shared_ptr<Trace> trace = Utils::createTrace(
        {"a", "b", "c"}, {{"a", "b", "c"}, {"a", "c"}, {}, {"a", "b", "c"}, {"b"}}, std::vector<size_t>{0, 1, 2, 3, 4});
    Formula formula = Parser::parse("A x in D. x");
    Formula formulaAtomic = Parser::parse("x");
    formula.setId("R1");
    DomainStorage dom1{"D", {"a", "b", "c"}, [](const std::any &val) { return std::any_cast<const char *>(val); }};

    auto mon1{std::make_shared<AtomicMonitor>(formulaAtomic, 2)};
    mon1->registerParent(1);
    std::map<std::string, std::shared_ptr<DomainStorage>> variableValues{{"x", std::make_shared<DomainStorage>(dom1)}};
    auto mon{UniversalMonitor(formula, mon1, 1, variableValues, false)};
    mon.registerParent(0);
    mon.setTrace(trace);
    EXPECT_EQ(mon.step(0, 0).value, true);
    EXPECT_EQ(mon.step(0, 1).value, false);
    EXPECT_EQ(mon.step(0, 2).value, false);
    EXPECT_EQ(mon.step(0, 3).value, true);
    EXPECT_EQ(mon.step(0, 4).value, false);
}

TEST(TestUniversalMonitor, testUniversalMonitorToString) {
    Formula formula = Parser::parse("A x, y in D. p(x, y, '100')");
    Formula formulaPredicate = Parser::parse("p(x, y, '100')");
    formula.setId("R1");
    DomainStorage dom1{"D", std::vector<std::any>{},
                       [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); }};

    auto mon1{std::make_shared<PredicateMonitor>(formulaPredicate, 1, false)};
    std::map<std::string, std::shared_ptr<DomainStorage>> variableValues{{"x", std::make_shared<DomainStorage>(dom1)},
                                                                         {"y", std::make_shared<DomainStorage>(dom1)}};
    auto mon{UniversalMonitor(formula, mon1, 0, variableValues, false)};
    EXPECT_EQ(mon.to_string(), "A x in D, y in D. p(x, y, '100')");
}
