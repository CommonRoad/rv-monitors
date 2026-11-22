#include "test_hydra.h"
#include "../test_database.h"
#include "hydra_test_utils.h"
#include "monitors/collections/domain.h"
#include "monitors/expression_tree/unary/temporal/globally.h"
#include "monitors/parser/parser.h"

TEST(TestHydra2, testFormulaType) {
    Formula formulaOne = Parser::parse("G(a -> F(b))", "R1");
    Formula formulaTwo = Parser::parse("H(a -> F(b))", "R1");
    Formula formulaThree = Parser::parse("G(a -> F(b) & O(c))", "R1");
    Formula formulaFour = Parser::parse("a U b", "R1");
    EXPECT_NO_THROW(Hydra2({formulaOne}));
    EXPECT_THROW(Hydra2({formulaTwo}), std::logic_error);
    EXPECT_NO_THROW(Hydra2({formulaThree}));
    EXPECT_THROW(Hydra2({formulaFour}), std::logic_error);
}

TEST(TestHydra2, specificFormulaTest) {
    auto testDataBase{TestDataBase()};
    auto testCase = testDataBase.getTestSample("F97_1");
    hydraTestUtils::executeHydraEval(testCase.trace, {*testCase.formula}, {testCase.expectedResult}, testCase.domains);
}

TEST(TestHydra2, formulaTest) {
    std::vector<std::string> excludedTestCases{"F49_1"}; // TODO add Weak next support
    size_t failures{0};
    auto testDataBase{TestDataBase()};
    auto numEval{0};
    for (const auto &testCase : testDataBase.getDatabaseEntries(supportedFormulaTypes.at(AlgorithmType::hydra_2_mtl))) {
        if (std::any_of(excludedTestCases.begin(), excludedTestCases.end(),
                        [testCase](const std::string &name) { return name == testCase.formula->getId(); }))
            continue;
        failures += hydraTestUtils::executeHydraEval(testCase.trace, {*testCase.formula}, {testCase.expectedResult},
                                                     testCase.domains);
        numEval++;
    }
    std::cout << "Hydra Formula Test: " << std::to_string(failures) << "/" << std::to_string(numEval)
              << " failing formulas.";
}

TEST(TestHydra2, formulaTestEarlyTermination) {
    std::vector<std::string> excludedTestCases{"F49_1"};
    size_t failures{0};
    auto testDataBase{TestDataBase()};
    auto testCases{testDataBase.getDatabaseEntries(supportedFormulaTypes.at(AlgorithmType::hydra_2_mtl))};
    for (const auto &testCase : testCases) {
        if (std::any_of(excludedTestCases.begin(), excludedTestCases.end(),
                        [testCase](const std::string &name) { return name == testCase.formula->getId(); }))
            continue;
        auto monitor{Hydra2({*testCase.formula}, false, false, false, globally_early_termination, testCase.domains)};
        auto trace{testCase.trace};
        EXPECT_TRUE(TestDataBase::mergeResult(monitor.evaluate(trace).at(testCase.formula->getId())) ==
                    TestDataBase::mergeResult(testCase.expectedResult))
            << "Hydra Formula Test Early Termination " << testCase.formula->getId() << " failed.";
        if (!(TestDataBase::mergeResult(monitor.evaluate(trace).at(testCase.formula->getId())) ==
              TestDataBase::mergeResult(testCase.expectedResult)))
            failures++;
    }
    std::cout << "Hydra Formula Test Early Termination: " << std::to_string(failures) << "/"
              << std::to_string(testCases.size()) << " failing formulas.";
}

TEST(TestHydra2, formulaTestInitialStepEvaluationWithGlobally) {
    std::vector<std::string> excludedTestCases{"F1_1", "F49_1"}; // F1_1 does not provide a trace
    size_t failures{0};
    auto testDataBase{TestDataBase()};
    auto testCases{testDataBase.getDatabaseEntries(supportedFormulaTypes.at(hydra_2_mtl))};
    for (const auto &testCase : testCases) {
        if (std::any_of(excludedTestCases.begin(), excludedTestCases.end(),
                        [testCase](const std::string &name) { return name == testCase.formula->getId(); })) {
            continue;
        }
        auto monitor{Hydra2({*testCase.formula}, false, false, false, initial_step, testCase.domains)};
        auto trace{testCase.trace};
        auto actual = monitor.evaluate(trace).at(testCase.formula->getId());
        auto expected = TestDataBase::mergeResult(testCase.expectedResult).value;
        EXPECT_EQ(actual.at(0).value, expected)
            << "Hydra Formula Test Initial Step Evaluation with Globally " << testCase.formula->getId() << " failed.";
        if (actual.at(0).value != expected) {
            failures++;
        }
    }
    std::cout << "Hydra Formula Test Initial Step Evaluation with Globally: " << std::to_string(failures) << "/"
              << std::to_string(testCases.size()) << " failing formulas.";
}

TEST(TestHydra2, formulaTestInitialStepEvaluationWithoutGlobally) {
    std::vector<std::string> excludedTestCases{"F1_1", "F49_1"}; // F1_1 does not provide a trace
    size_t failures{0};
    auto testDataBase{TestDataBase()};
    auto testCases{testDataBase.getDatabaseEntries(supportedFormulaTypes.at(AlgorithmType::hydra_2_mtl))};
    for (const auto &testCase : testCases) {
        if (std::any_of(excludedTestCases.begin(), excludedTestCases.end(),
                        [testCase](const std::string &name) { return name == testCase.formula->getId(); })) {
            continue;
        }
        auto globally = std::dynamic_pointer_cast<Globally>(testCase.formula->getExpr());
        if (!globally) {
            // This should never happen, since we only select formulas supported by the Hydra monitor, i.e., ones that
            // start with globally
            ASSERT_TRUE(false) << "Hydra Formula Test Initial Step Evaluation without Globally "
                               << testCase.formula->getId() << ": Formula does not start with globally.";
        }
        auto monitor{Hydra2({Formula(globally->getLeft(), testCase.formula->getId())}, false, false, false,
                            FormulaEvaluationMode::initial_step, testCase.domains)};
        auto trace{testCase.trace};
        auto actual = monitor.evaluate(trace).at(testCase.formula->getId());

        auto expected = std::find_if(testCase.expectedResult.begin(), testCase.expectedResult.end(),
                                     [](const auto &verdict) { return verdict.timeStamp == 0; });
        EXPECT_TRUE(expected != testCase.expectedResult.end())
            << "Hydra Formula Test Initial Step Evaluation without Globally " << testCase.formula->getId()
            << ": No expected verdict for initial step.";
        EXPECT_EQ(actual.at(0).value, expected->value) << "Hydra Formula Test Initial Step Evaluation without Globally "
                                                       << testCase.formula->getId() << " failed.";
        if (actual.at(0).value != expected->value) {
            failures++;
        }
    }
    std::cout << "Hydra Formula Test Initial Step Evaluation without Globally: " << std::to_string(failures) << "/"
              << std::to_string(testCases.size()) << " failing formulas.";
}

TEST(TestHydra2, testToString) {
    Formula formula = Parser::parse("G(P(c))");
    formula.setId("R1");
    auto monitor{Hydra2({formula}, false, false, false)};
    EXPECT_EQ(monitor.to_string(), "G(P(c))");
}

TEST(TestHydra2, testNoValidId) {
    Formula formula = Parser::parse("G(P[0, 1](c))");
    EXPECT_THROW(auto monitor{Hydra2({formula}, false, false, false)}, std::logic_error);
}

TEST(TestHydra2, testGetFormula) {
    Formula formula = Parser::parse("G(P[0, 1](c))");
    formula.setId("R1");
    auto monitor{Hydra2({formula}, false, false, false)};
    EXPECT_EQ(monitor.getFormulas().size(), 1);
    EXPECT_EQ(monitor.getFormulas().at(0).getId(), "R1");
    EXPECT_EQ(monitor.getFormulas().at(0).to_string(), "G(P[0, 1](c))");
}

TEST(TestHydra2, testCombinedSinceUntilFormulas) {
    const std::shared_ptr<Trace> trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}},
                                                            std::vector<size_t>{0, 0, 2, 4, 5, 10});
    std::vector<Formula> formulas = {Parser::parse("G((a)S[0, 4](b))"), Parser::parse("G((a)U[0, 4](b))"),
                                     Parser::parse("G(((a)U[0, 4](b)) and a)"),
                                     Parser::parse("G((a)S[0, 4](b) or (a)U[0, 4](b))")};
    formulas[0].setId("R1");
    formulas[1].setId("R2");
    formulas[2].setId("R3");
    formulas[3].setId("R4");
    std::vector<std::vector<Verdict>> expSolutions{
        {Verdict{0, false}, Verdict{0, false}, Verdict{2, false}, Verdict{4, true}, Verdict{5, true},
         Verdict{10, true}},
        {Verdict{0, true}, Verdict{0, true}, Verdict{2, true}, Verdict{4, true}, Verdict{5, false}, Verdict{10, true}},
        {Verdict{0, true}, Verdict{0, true}, Verdict{2, true}, Verdict{4, true}, Verdict{5, false}, Verdict{10, false}},
        {Verdict{0, true}, Verdict{0, true}, Verdict{2, true}, Verdict{4, true}, Verdict{5, true}, Verdict{10, true}}};
    hydraTestUtils::executeHydraEval(trace, formulas, expSolutions);
}

TEST(TestHydra2, testToTree) {
    std::vector<Formula> formulas = {Parser::parse("G((a)S[0, 4](b))"), Parser::parse("G((a)U[0, 4](b))"),
                                     Parser::parse("G(((a)U[0, 4](b)) and a)"),
                                     Parser::parse("G((a)S[0, 4](b) or (a)U[0, 4](b))")};
    formulas[0].setId("R1");
    formulas[1].setId("R2");
    formulas[2].setId("R3");
    formulas[3].setId("R4");
    std::vector<size_t> expectedRootIDs{0, 3, 4, 5};
    auto mon{Hydra2(formulas, true, true, true, globally)};
    auto tree{mon.to_tree()};
    ASSERT_EQ(tree.getRootIDs().size(), 4) << "Vectors x and y are of unequal length";
    for (int i = 0; i < tree.getRootIDs().size(); ++i) {
        EXPECT_EQ(tree.getRootIDs()[i], expectedRootIDs[i]) << "Vectors x and y differ at index " << i;
    }
    EXPECT_EQ(tree.getNodeTypes().at(0), "S[0, 4]");
    EXPECT_EQ(tree.getTree().at(0).size(), 2);
    EXPECT_EQ(tree.getTree().at(0).at(0), 2);
    EXPECT_EQ(tree.getTree().at(0).at(1), 1);
    EXPECT_EQ(tree.getNodeTypes().at(1), "b");
    EXPECT_EQ(tree.getTree().at(1).size(), 0);
    EXPECT_EQ(tree.getNodeTypes().at(2), "a");
    EXPECT_EQ(tree.getNodeTypes().at(3), "U[0, 4]");
    EXPECT_EQ(tree.getNodeTypes().at(4), "&");
    EXPECT_EQ(tree.getTree().at(4).size(), 2);
    EXPECT_EQ(tree.getNodeTypes().at(5), "|");
}

TEST(TestHydra2, testCombinedExistentialQuantifier) {
    const std::shared_ptr<Trace> trace = Utils::createTrace(
        {"a", "b", "d"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}}, std::vector<size_t>{0, 0, 2, 4, 5, 10});
    std::vector<Formula> formulas = {Parser::parse("G(E x in D. x)"), Parser::parse("G(E x in D. (P(x) & x))")};
    formulas[0].setId("R1");
    formulas[1].setId("R2");

    std::vector<std::shared_ptr<DomainStorage>> domains{{std::make_shared<DomainStorage>(
        DomainStorage("D", {"a", "b", "d"}, [](const std::any &val) { return std::any_cast<const char *>(val); }))}};

    auto monitor1{Hydra2({formulas[0]}, true, true, true, globally)};
    auto monitor2{Hydra2({formulas[1]}, true, true, true, globally)};
    auto res1{monitor1.evaluate(trace, domains)};
    auto res2{monitor2.evaluate(trace, domains)};

    std::vector<std::vector<Verdict>> expSolutions{res1.at("R1"), res2.at("R2")};

    hydraTestUtils::executeHydraEval(trace, formulas, expSolutions, domains);
}

TEST(TestHydra2, testCombinedUniversalQuantifier) {
    const std::shared_ptr<Trace> trace = Utils::createTrace(
        {"a", "b", "d"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}}, std::vector<size_t>{0, 0, 2, 4, 5, 10});
    std::vector<Formula> formulas = {Parser::parse("G(A x in D. x)"), Parser::parse("G(A x in D. (P(x) & x))")};
    formulas[0].setId("R1");
    formulas[1].setId("R2");

    std::vector<std::shared_ptr<DomainStorage>> domains{{std::make_shared<DomainStorage>(
        DomainStorage("D", {"a", "b", "d"}, [](const std::any &val) { return std::any_cast<const char *>(val); }))}};

    auto monitor1{Hydra2({formulas[0]}, true, true, true, globally)};
    auto monitor2{Hydra2({formulas[1]}, true, true, true, globally)};
    auto res1{monitor1.evaluate(trace, domains)};
    auto res2{monitor2.evaluate(trace, domains)};

    std::vector<std::vector<Verdict>> expSolutions{res1.at("R1"), res2.at("R2")};

    hydraTestUtils::executeHydraEval(trace, formulas, expSolutions, domains);
}

TEST(TestHydra2, testCombinedUniversalQuantifierDynamicDomain) {
    const std::shared_ptr<Trace> trace = hydraTestUtils::createTraceWithVariable(
        4, {std::make_shared<Function<bool, size_t, size_t>>(
                Function<bool, size_t, size_t>("divisible_by_time_step", {"timeStep", "val"},
                                               hydraTestUtils::divisible_by_time_step, hydraTestUtils::twoSizeTArgs)),
            std::make_shared<Function<bool, size_t, size_t>>(
                Function<bool, size_t, size_t>("higher_than_time_step", {"timeStep", "val"},
                                               hydraTestUtils::higher_than_time_step, hydraTestUtils::twoSizeTArgs))});
    std::vector<Formula> formulas = {
        Parser::parse("A x in D. (X(divisible_by_time_step(x)) U higher_than_time_step(x))"),
        Parser::parse("A x in D. (divisible_by_time_step(x) -> higher_than_time_step(x))")};
    formulas[0].setId("R1");
    formulas[1].setId("R2");

    std::vector<std::shared_ptr<DomainStorage>> domains{{std::make_shared<DomainStorage>(DomainStorage(
        "D", {2ul, 3ul, 4ul}, [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); }))}};

    auto monitor1{Hydra2({formulas[0]}, true, true, true, globally)};
    auto monitor2{Hydra2({formulas[1]}, true, true, true, globally)};
    auto res1{monitor1.evaluate(trace, domains)};
    auto res2{monitor2.evaluate(trace, domains)};

    std::vector<std::vector<Verdict>> expSolutions{res1.at("R1"), res2.at("R2")};

    hydraTestUtils::executeHydraEval(trace, formulas, expSolutions, domains);
}

TEST(TestHydra2, testFutureOnceLazy) {
    const std::shared_ptr<Trace> trace = hydraTestUtils::createTraceWithVariable(
        6, {std::make_shared<Function<bool, size_t, std::string>>(Function<bool, size_t, std::string>(
               "test_domain", {"timeStep", "val"}, hydraTestUtils::test_domain, hydraTestUtils::sizeTStringArgs))});
    std::vector formulas = {Parser::parse("E x in D. (O(test_domain(x)) & F(test_domain(x)))")};
    formulas[0].setId("R1");

    auto functionDom{std::make_shared<Function<std::vector<std::any>, size_t>>(Function<std::vector<std::any>, size_t>(
        "testname", {"stepNumber"}, hydraTestUtils::createStringDomain3, hydraTestUtils::createDomain3Arguments))};
    DomainStorage dom1{"D", functionDom, [](const std::any &val) { return std::any_cast<std::string>(val); },
                       [](size_t, const std::any &) { return true; }};
    std::vector domains{std::make_shared<DomainStorage>(dom1)};

    auto monitor1{Hydra2({formulas[0]}, true, true, true, FormulaEvaluationMode::globally)};
    const auto res1{monitor1.evaluate(trace, domains)};
    const std::vector<std::vector<Verdict>> expSolutions{
        {{0, true}, {1, true}, {2, true}, {3, false}, {4, false}, {5, true}}};

    hydraTestUtils::executeHydraEval(trace, formulas, expSolutions, domains);
}

TEST(TestHydra2, testExistentialStepLooping) {
    const std::shared_ptr<Trace> trace = Utils::createTrace({"a", "b", "c"}, {{"a", "b", "c"}, {}, {"b"}, {"a", "c"}});
    Formula formula = Parser::parse("G(C<=2 x in D. x)");
    formula.setId("R1");
    const std::vector<Formula> formulas = {formula};
    std::vector<std::vector<Verdict>> expSolutions{
        {Verdict{0, false}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true}}};
    DomainStorage dom1{"D", {"a", "b", "c"}, [](const std::any &val) { return std::any_cast<const char *>(val); }};
    std::vector<std::shared_ptr<DomainStorage>> domains{std::make_shared<DomainStorage>(dom1)};
    hydraTestUtils::executeHydraEval(trace, formulas, expSolutions, domains);
}

std::vector<std::any> createStringDomain(size_t stepNumber) { return {"a", "b", "c"}; }

std::tuple<size_t> createDomain2Arguments(const std::vector<DomainValue> &args) {
    return {std::any_cast<size_t>(args.at(0).getValue())};
}

TEST(TestHydra2, testExistentialStepLoopingDynamicDomain) {
    const std::shared_ptr<Trace> trace = Utils::createTrace({"a", "b", "c"}, {{"a", "b", "c"}, {}, {"b"}, {"a", "c"}});
    Formula formula = Parser::parse("G(C<=2 x in D. x)");
    formula.setId("R1");
    std::vector<Formula> formulas = {formula};
    std::vector<std::vector<Verdict>> expSolutions{
        {Verdict{0, false}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true}}};
    auto functionDom{std::make_shared<Function<std::vector<std::any>, size_t>>(Function<std::vector<std::any>, size_t>(
        "testname", {"stepNumber"}, createStringDomain, createDomain2Arguments))};
    DomainStorage dom1{"D", functionDom, [](const std::any &) { return "name"; },
                       [](size_t, const std::any &) { return true; }};
    std::vector<std::shared_ptr<DomainStorage>> domains{std::make_shared<DomainStorage>(dom1)};
    hydraTestUtils::executeHydraEval(trace, formulas, expSolutions, domains);
}

std::vector<std::any> domain(size_t stepNumber, std::vector<std::vector<std::any>> possibleSets) {
    switch (stepNumber) {
    case 0:
        return possibleSets[0];
    case 1:
        return possibleSets[1];
    case 2:
        return possibleSets[2];
    case 3:
        return possibleSets[3];
    default:
        return {};
    }
}

TEST(TestHydra2, testExistenceVariableDomainLazyPredicate) {
    const std::shared_ptr<Trace> trace = hydraTestUtils::createTraceWithVariable(
        4,
        {std::make_shared<Function<bool, size_t, size_t, size_t>>(Function<bool, size_t, size_t, std::size_t>(
             "equal_to_time_step_with_unused_arg", {"timeStep", "randomValue", "value"},
             hydraTestUtils::equal_to_time_step_with_unused_arg, hydraTestUtils::threeSizeTArgs)),
         std::make_shared<Function<bool, size_t, size_t>>(Function<bool, size_t, size_t>(
             "equals_three", {"timeStep", "val"}, hydraTestUtils::equals_three, hydraTestUtils::twoSizeTArgs))},
        ConstantCollection({ConstantValue{
            "x_ego", 0ul, [](const std::any &val) { return std::to_string(std::any_cast<std::size_t>(val)); }}}));
    Formula formula =
        Parser::parse("E x_o in D0. equal_to_time_step_with_unused_arg('x_ego', x_o) & equals_three(x_o)");
    formula.setId("R1");
    std::vector<Formula> formulas = {formula};
    std::vector<std::vector<Verdict>> expSolutions{
        {Verdict{0, false}, Verdict{1, false}, Verdict{2, false}, Verdict{3, true}}};
    std::vector<std::vector<std::any>> possibleSets{{1ul, 2ul, 3ul, 4ul}, {1ul, 2ul, 3ul}, {0ul}, {3ul}};
    auto functionDom{std::make_shared<Function<std::vector<std::any>, size_t>>(Function<std::vector<std::any>, size_t>(
        "testname", {"stepNumber"}, [possibleSets](size_t stepNumber) { return domain(stepNumber, possibleSets); },
        createDomain2Arguments))};
    DomainStorage dom1{"D0", functionDom,
                       [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); },
                       [](size_t, const std::any &) { return true; }};
    std::vector<std::shared_ptr<DomainStorage>> domains{std::make_shared<DomainStorage>(dom1)};
    hydraTestUtils::executeHydraEval(trace, formulas, expSolutions, domains);
}

TEST(TestHydra2, testLazyEvaluationComplexPredicate) {
    auto traceAtomic{Utils::createTrace({"a"}, {{}, {}, {"a"}})};
    const std::vector<Event> eventList{traceAtomic->getEvent(0), traceAtomic->getEvent(1), traceAtomic->getEvent(2)};
    const std::shared_ptr<Trace> trace = hydraTestUtils::createTraceWithVariable(
        4,
        {std::make_shared<Function<bool, size_t, size_t>>(
            Function<bool, size_t, size_t>("raise_if_step_less_than_two", {"timeStep", "randomValue"},
                                           hydraTestUtils::raiseIfStepLessThanTwo, hydraTestUtils::twoSizeTArgs))},
        ConstantCollection({ConstantValue{
            "2", 2ul, [](const std::any &val) { return std::to_string(std::any_cast<std::size_t>(val)); }}}),
        eventList);

    Formula formula = Parser::parse("G(a & raise_if_step_less_than_two('2'))");
    formula.setId("R1");
    std::vector<Formula> formulas = {formula};
    std::vector<std::vector<Verdict>> expSolutions{{Verdict{0, false}, Verdict{1, false}, Verdict{2, true}}};

    auto monitor{Hydra2(formulas, true, true, true, globally)};
    hydraTestUtils::evalHydra(formulas, expSolutions, monitor, trace, "ttt", {});
}

TEST(TestHydra2, testHierachicalEval) {
    const std::shared_ptr<Trace> trace = hydraTestUtils::createTraceWithVariable(
        4,
        {std::make_shared<Function<bool, size_t, size_t, size_t>>(Function<bool, size_t, size_t, std::size_t>(
             "equal_to_time_step_with_unused_arg", {"timeStep", "randomValue", "value"},
             hydraTestUtils::equal_to_time_step_with_unused_arg, hydraTestUtils::threeSizeTArgs)),
         std::make_shared<Function<bool, size_t, size_t>>(Function<bool, size_t, size_t>(
             "equals_three", {"timeStep", "val"}, hydraTestUtils::equals_three, hydraTestUtils::twoSizeTArgs))},
        ConstantCollection({ConstantValue{
            "x_ego", 0ul, [](const std::any &val) { return std::to_string(std::any_cast<std::size_t>(val)); }}}));
    Formula formula1 =
        Parser::parse("E x_o in D0. equal_to_time_step_with_unused_arg('x_ego', x_o) & equals_three(x_o)");
    formula1.setId("R1");
    Formula formula2 = Parser::parse("A x_o in D0. equals_three(x_o)");
    formula2.setId("R2");
    std::vector<Formula> formulas = {formula1, formula2};
    std::vector<std::vector<Verdict>> expSolutions{
        {Verdict{0, false}, Verdict{1, false}, Verdict{2, false}, Verdict{3, true}}};
    std::vector<std::vector<std::any>> possibleSets{{1ul, 2ul, 3ul, 4ul}, {1ul, 2ul, 3ul}, {0ul}, {3ul}};
    auto functionDom{std::make_shared<Function<std::vector<std::any>, size_t>>(Function<std::vector<std::any>, size_t>(
        "testname", {"stepNumber"}, [possibleSets](size_t stepNumber) { return domain(stepNumber, possibleSets); },
        createDomain2Arguments))};
    DomainStorage dom1{"D0", functionDom,
                       [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); },
                       [](size_t, const std::any &) { return true; }};
    std::vector<std::shared_ptr<DomainStorage>> domains{std::make_shared<DomainStorage>(dom1)};
    auto monitor{Hydra2(formulas, true, true, true, globally)};
    auto solution{monitor.evaluate(trace, domains, {"R1"})};
    EXPECT_EQ(solution["R1"].at(0).value, expSolutions.at(0).at(0).value);
    EXPECT_EQ(solution["R1"].at(1).value, expSolutions.at(0).at(1).value);
    EXPECT_EQ(solution["R1"].at(2).value, expSolutions.at(0).at(2).value);
    EXPECT_EQ(solution["R1"].at(3).value, expSolutions.at(0).at(3).value);
    auto solution2{monitor.evaluate({}, {}, {"R2"})};
    EXPECT_EQ(solution2["R2"].at(0).value, false);
    EXPECT_EQ(solution2["R2"].at(1).value, false);
    EXPECT_EQ(solution2["R2"].at(2).value, false);
    EXPECT_EQ(solution2["R2"].at(3).value, true);

    auto monitor2{Hydra2(formulas, true, true, true, globally)};
    auto solution21{monitor2.evaluate(trace, domains, {"R1"})};
    EXPECT_EQ(solution21["R1"].at(0).value, expSolutions.at(0).at(0).value);
    EXPECT_EQ(solution21["R1"].at(1).value, expSolutions.at(0).at(1).value);
    EXPECT_EQ(solution21["R1"].at(2).value, expSolutions.at(0).at(2).value);
    EXPECT_EQ(solution21["R1"].at(3).value, expSolutions.at(0).at(3).value);
}

TEST(TestHydra2, testInvalidPredicateCost) {
    Formula formula = Parser::parse("G(P[0, 1](c))", "R1");
#ifdef NDEBUG
    GTEST_SKIP() << "assertions are disabled";
#endif
    EXPECT_DEATH(Hydra2({formula}, false, false, false, globally, {}, {{"test", {1, 2}}}),
                 "Invalid predicate costs/probabilities");
}

namespace {
constexpr size_t kLongTraceLength{200};

bool atomA(const size_t i) { return i % 3 == 0 or i % 7 == 1; }
bool atomB(const size_t i) { return i % 5 == 1; }
bool atomC(const size_t i) { return i % 2 == 0 or i % 11 == 3; }

// Equidistant trace of length kLongTraceLength over the atoms a, b, and c.
std::shared_ptr<Trace> longTrace() {
    std::vector<std::vector<std::string>> events;
    std::vector<size_t> timeStamps;
    for (size_t i{0}; i < kLongTraceLength; ++i) {
        std::vector<std::string> event;
        if (atomA(i))
            event.emplace_back("a");
        if (atomB(i))
            event.emplace_back("b");
        if (atomC(i))
            event.emplace_back("c");
        events.push_back(event);
        timeStamps.push_back(i);
    }
    return Utils::createTrace({"a", "b", "c"}, events, timeStamps);
}
} // namespace

// Verdicts are only stored if they can be requested several times and are removed once no parent can request them
// anymore, so the number of stored verdicts does not grow with the trace length.
TEST(TestHydra2, testStoredVerdictsAreBounded) {
    const auto trace{longTrace()};
    for (const bool connectedEval : {false, true})
        for (const bool reordering : {false, true})
            for (const bool shortCircuiting : {false, true}) {
                // no temporal operator and no shared sub-formula: every verdict is requested once
                auto propositional{Hydra2({Parser::parse("G(a & (b | c))", "R1")}, connectedEval, reordering,
                                          shortCircuiting, globally)};
                propositional.evaluate(trace);
                EXPECT_EQ(propositional.getNumStoredVerdicts(), 0);

                // the children of since and until are requested repeatedly; only the window of since (4 steps) and
                // the current step of until are kept
                auto temporal{Hydra2(
                    {Parser::parse("G((a S[0, 3] b) | (c U[1, 4] a))", "R1"), Parser::parse("G(P(a) -> X(b))", "R2")},
                    connectedEval, reordering, shortCircuiting, globally)};
                temporal.evaluate(trace);
                EXPECT_LE(temporal.getNumStoredVerdicts(), 20);
            }
}

// Pruning does not change the verdicts on traces that are longer than the intervals.
TEST(TestHydra2, testPrunedVerdictsOnLongTrace) {
    const auto trace{longTrace()};
    std::vector<Verdict> expected;
    for (size_t i{0}; i < kLongTraceLength; ++i) {
        // a S[0, 3] b: b at j with i - j <= 3 and a at all steps after j up to i
        bool since{false};
        for (size_t j{i + 1}; j-- > 0 and i - j <= 3;) {
            if (atomB(j)) {
                since = true;
                break;
            }
            if (!atomA(j))
                break;
        }
        // c U[1, 4] a: a at j with 1 <= j - i <= 4 and c at all steps from i before j
        bool until{false};
        for (size_t j{i}; j < kLongTraceLength and j - i <= 4; ++j) {
            if (j - i >= 1 and atomA(j)) {
                until = true;
                break;
            }
            if (!atomC(j))
                break;
        }
        expected.emplace_back(i, since or until);
    }
    std::vector<Formula> formulas{Parser::parse("G((a S[0, 3] b) | (c U[1, 4] a))", "R1"),
                                  Parser::parse("G(a S[0, 3] b)", "R2")};
    std::vector<Verdict> expectedSince;
    for (size_t i{0}; i < kLongTraceLength; ++i) {
        bool since{false};
        for (size_t j{i + 1}; j-- > 0 and i - j <= 3;) {
            if (atomB(j)) {
                since = true;
                break;
            }
            if (!atomA(j))
                break;
        }
        expectedSince.emplace_back(i, since);
    }
    hydraTestUtils::executeHydraEval(trace, formulas, {expected, expectedSince});
}

namespace {
// Asymmetric, time-dependent relation between two values.
bool asymmetricRelation(size_t timeStep, size_t x, size_t y) { return (7 * x + 13 * y + timeStep + x * y) % 5 < 2; }
} // namespace

// The clones of a sub-monitor are cached per variable and domain value. With connected evaluation, rel(x, y) is shared
// by both formulas, whose quantifiers bind x and y in different orders, so the formulas must not receive each other's
// clones (including their cached verdicts).
TEST(TestHydra2, testQuantifierClonesDistinguishVariables) {
    const std::shared_ptr<Trace> trace = hydraTestUtils::createTraceWithVariable(
        4, {std::make_shared<Function<bool, size_t, size_t, size_t>>(Function<bool, size_t, size_t, size_t>(
               "rel", {"timeStep", "x", "y"}, asymmetricRelation, hydraTestUtils::threeSizeTArgs))});
    const std::vector<std::shared_ptr<DomainStorage>> domains{std::make_shared<DomainStorage>(DomainStorage(
        "D", {0ul, 1ul, 2ul}, [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); }))};
    const auto verdicts = [](const std::vector<bool> &values) {
        std::vector<Verdict> result;
        for (size_t idx{0}; idx < values.size(); ++idx)
            result.emplace_back(idx, values[idx]);
        return result;
    };
    // formula R1, formula R2, expected verdicts of R1, expected verdicts of R2
    const std::vector<std::tuple<std::string, std::string, std::vector<bool>, std::vector<bool>>> cases{
        {"G(E x in D. A y in D. rel(x, y))",
         "G(E y in D. A x in D. rel(x, y))",
         {false, true, true, false},
         {false, false, false, false}},
        {"G(A x in D. E y in D. rel(x, y))",
         "G(A y in D. E x in D. rel(x, y))",
         {false, true, false, false},
         {true, true, true, false}},
        {"G(E x in D. C=1 y in D. rel(x, y))",
         "G(E y in D. C=1 x in D. rel(x, y))",
         {false, true, true, true},
         {true, true, true, true}},
        {"G(A x in D. C>=1 y in D. rel(x, y))",
         "G(C=1 y in D. A x in D. rel(x, y))",
         {false, true, false, false},
         {false, false, false, false}}};
    for (const auto &[first, second, expectedFirst, expectedSecond] : cases) {
        std::vector<Formula> formulas{Parser::parse(first), Parser::parse(second)};
        formulas[0].setId("R1");
        formulas[1].setId("R2");
        SCOPED_TRACE(first + " / " + second);
        hydraTestUtils::executeHydraEval(trace, formulas, {verdicts(expectedFirst), verdicts(expectedSecond)}, domains);
    }
}

// With lazy evaluation, the unbounded since is skipped as long as a holds and catches up on the skipped steps at the
// last step. It evaluates its children at most at the steps at which they are evaluated without lazy evaluation, so
// lazy evaluation computes fewer verdicts in total.
TEST(TestHydra2, testLazyEvaluationComputesNoAdditionalVerdicts) {
    constexpr size_t traceLength{50};
    std::vector<std::vector<std::string>> events;
    std::vector<size_t> timeStamps;
    for (size_t i{0}; i < traceLength; ++i) {
        std::vector<std::string> event{"b"};
        if (i + 1 < traceLength)
            event.emplace_back("a");
        if (i == 0)
            event.emplace_back("c");
        events.push_back(event);
        timeStamps.push_back(i);
    }
    const auto trace{Utils::createTrace({"a", "b", "c"}, events, timeStamps)};
    for (const bool connectedEval : {false, true}) {
        auto notLazy{Hydra2({Parser::parse("G(a | (b S c))", "R1")}, connectedEval, false, false, globally)};
        auto lazy{Hydra2({Parser::parse("G(a | (b S c))", "R1")}, connectedEval, false, true, globally)};
        const auto resultNotLazy{notLazy.evaluate(trace)};
        const auto resultLazy{lazy.evaluate(trace)};
        for (size_t i{0}; i < traceLength; ++i) {
            EXPECT_TRUE(resultNotLazy.at("R1").at(i).value);
            EXPECT_TRUE(resultLazy.at("R1").at(i).value);
        }
        EXPECT_EQ(notLazy.getNumEvaluations(), 249U);
        EXPECT_EQ(lazy.getNumEvaluations(), 199U);
    }
}

namespace {
bool alwaysFalseRelation(size_t, size_t, size_t) { return false; }
} // namespace

// Short-circuiting is only disabled for the top-level universal quantifier (it records every violating domain value).
// The nested universal quantifier stops at its first violating value, so it evaluates fewer predicates.
TEST(TestHydra2, testNestedUniversalQuantifierShortCircuits) {
    constexpr size_t traceLength{4};
    const std::shared_ptr<Trace> trace = hydraTestUtils::createTraceWithVariable(
        traceLength, {std::make_shared<Function<bool, size_t, size_t, size_t>>(Function<bool, size_t, size_t, size_t>(
                         "rel", {"timeStep", "x", "y"}, alwaysFalseRelation, hydraTestUtils::threeSizeTArgs))});
    const std::vector<std::shared_ptr<DomainStorage>> domains{std::make_shared<DomainStorage>(DomainStorage(
        "D", {0ul, 1ul, 2ul}, [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); }))};
    const auto formula{Parser::parse("A x in D. G(A y in D. rel(x, y))", "R1")};

    auto shortCircuit{Hydra2({formula}, true, false, true, globally, domains)};
    auto noShortCircuit{Hydra2({formula}, true, false, false, globally, domains)};
    const auto resultShortCircuit{shortCircuit.evaluate(trace)};
    const auto resultNoShortCircuit{noShortCircuit.evaluate(trace)};
    for (size_t i{0}; i < traceLength; ++i) {
        EXPECT_FALSE(resultShortCircuit.at("R1").at(i).value);
        EXPECT_FALSE(resultNoShortCircuit.at("R1").at(i).value);
    }
    // top-level quantifier: all three values of x are recorded as violating at every step
    for (const auto &mon : {&shortCircuit, &noShortCircuit}) {
        const auto violations{mon->getViolatingDomainValues().at("R1")};
        for (size_t i{0}; i < traceLength; ++i)
            EXPECT_EQ(violations.at(i).size(), 3U);
    }
    // nested quantifier: one instead of three evaluations of rel per value of x
    EXPECT_LT(shortCircuit.getNumEvaluations(), noShortCircuit.getNumEvaluations());
}

// A direct chain of universal quantifiers is one top-level quantifier over both variables: all violating
// combinations are recorded.
TEST(TestHydra2, testUniversalQuantifierChainRecordsViolations) {
    constexpr size_t traceLength{4};
    const std::shared_ptr<Trace> trace = hydraTestUtils::createTraceWithVariable(
        traceLength, {std::make_shared<Function<bool, size_t, size_t, size_t>>(Function<bool, size_t, size_t, size_t>(
                         "rel", {"timeStep", "x", "y"}, alwaysFalseRelation, hydraTestUtils::threeSizeTArgs))});
    const std::vector<std::shared_ptr<DomainStorage>> domains{std::make_shared<DomainStorage>(DomainStorage(
        "D", {0ul, 1ul, 2ul}, [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); }))};
    auto monitor{Hydra2({Parser::parse("A x in D. A y in D. rel(x, y)", "R1")}, true, false, true, globally, domains)};
    const auto result{monitor.evaluate(trace)};
    const auto violations{monitor.getViolatingDomainValues().at("R1")};
    for (size_t i{0}; i < traceLength; ++i) {
        EXPECT_FALSE(result.at("R1").at(i).value);
        EXPECT_EQ(violations.at(i).size(), 9U);
    }
}
