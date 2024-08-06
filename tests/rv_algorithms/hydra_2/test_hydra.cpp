#include "test_hydra.h"
#include "../test_database.h"
#include "hydra_test_utils.h"
#include "monitors/collections/domain.h"
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
        auto monitor{Hydra2({*testCase.formula}, false, false, false, true, testCase.domains)};
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
    auto mon{Hydra2(formulas, true, true, true, false)};
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

    auto monitor1{Hydra2({formulas[0]}, true, true, true, false)};
    auto monitor2{Hydra2({formulas[1]}, true, true, true, false)};
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

    auto monitor1{Hydra2({formulas[0]}, true, true, true, false)};
    auto monitor2{Hydra2({formulas[1]}, true, true, true, false)};
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

    auto monitor1{Hydra2({formulas[0]}, true, true, true, false)};
    auto monitor2{Hydra2({formulas[1]}, true, true, true, false)};
    auto res1{monitor1.evaluate(trace, domains)};
    auto res2{monitor2.evaluate(trace, domains)};

    std::vector<std::vector<Verdict>> expSolutions{res1.at("R1"), res2.at("R2")};

    hydraTestUtils::executeHydraEval(trace, formulas, expSolutions, domains);
}

TEST(TestHydra2, testExistentialStepLooping) {
    const std::shared_ptr<Trace> trace = Utils::createTrace({"a", "b", "c"}, {{"a", "b", "c"}, {}, {"b"}, {"a", "c"}});
    Formula formula = Parser::parse("G(C<=2 x in D. x)");
    formula.setId("R1");
    std::vector<Formula> formulas = {formula};
    std::vector<std::vector<Verdict>> expSolutions{
        {Verdict{0, false}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true}}};
    DomainStorage dom1{"D", {"a", "b", "c"}, [](const std::any &val) { return std::any_cast<const char *>(val); }};
    std::vector<std::shared_ptr<DomainStorage>> domains{std::make_shared<DomainStorage>(dom1)};
    hydraTestUtils::executeHydraEval(trace, formulas, expSolutions, domains);
}

std::vector<std::any> createStringDomain(size_t stepNumber) { return {"a", "b", "c"}; }

TEST(TestHydra2, testExistentialStepLoopingDynamicDomain) {
    const std::shared_ptr<Trace> trace = Utils::createTrace({"a", "b", "c"}, {{"a", "b", "c"}, {}, {"b"}, {"a", "c"}});
    Formula formula = Parser::parse("G(C<=2 x in D. x)");
    formula.setId("R1");
    std::vector<Formula> formulas = {formula};
    std::vector<std::vector<Verdict>> expSolutions{
        {Verdict{0, false}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true}}};
    DomainStorage dom1{"D", createStringDomain, [](const std::any &) { return "name"; },
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
    DomainStorage dom1{"D0", [possibleSets](size_t stepNumber) { return domain(stepNumber, possibleSets); },
                       [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); },
                       [](size_t, const std::any &) { return true; }};
    std::vector<std::shared_ptr<DomainStorage>> domains{std::make_shared<DomainStorage>(dom1)};
    hydraTestUtils::executeHydraEval(trace, formulas, expSolutions, domains);
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
    DomainStorage dom1{"D0", [possibleSets](size_t stepNumber) { return domain(stepNumber, possibleSets); },
                       [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); },
                       [](size_t, const std::any &) { return true; }};
    std::vector<std::shared_ptr<DomainStorage>> domains{std::make_shared<DomainStorage>(dom1)};
    auto monitor{Hydra2(formulas, true, true, true, false)};
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

    auto monitor2{Hydra2(formulas, true, true, true, false)};
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
    EXPECT_DEATH(Hydra2({formula}, false, false, false, false, {}, {{"test", {1, 2}}}),
                 "Invalid predicate costs/probabilities");
}
