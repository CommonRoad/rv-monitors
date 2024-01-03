//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//

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
    auto testCase = testDataBase.getTestSample("F94_1");
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
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}},
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

TEST(TestHydra2, testExistentialStepLooping) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a", "b", "c"}, {}, {"b"}, {"a", "c"}});
    Formula formula = Parser::parse("G(C<=2 x in D. x)");
    formula.setId("R1");
    std::vector<Formula> formulas = {formula};
    std::vector<std::vector<Verdict>> expSolutions{
        {Verdict{0, false}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true}}};
    DomainStorage dom1{"D", {"a", "b", "c"}};
    std::vector<std::shared_ptr<DomainStorage>> domains{std::make_shared<DomainStorage>(dom1)};
    hydraTestUtils::executeHydraEval(trace, formulas, expSolutions, domains);
}

std::vector<std::any> createStringDomain(size_t stepNumber) { return {"a", "b", "c"}; }

TEST(TestHydra2, testExistentialStepLoopingDynamicDomain) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a", "b", "c"}, {}, {"b"}, {"a", "c"}});
    Formula formula = Parser::parse("G(C<=2 x in D. x)");
    formula.setId("R1");
    std::vector<Formula> formulas = {formula};
    std::vector<std::vector<Verdict>> expSolutions{
        {Verdict{0, false}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true}}};
    DomainStorage dom1{"D", createStringDomain};
    std::vector<std::shared_ptr<DomainStorage>> domains{std::make_shared<DomainStorage>(dom1)};
    hydraTestUtils::executeHydraEval(trace, formulas, expSolutions, domains);
}

TEST(TestHydra2, testInvalidPredicateCost) {
    Formula formula = Parser::parse("G(P[0, 1](c))", "R1");
#ifdef NDEBUG
    GTEST_SKIP() << "assertions are disabled";
#endif
    EXPECT_DEATH(Hydra2({formula}, false, false, false, false, {}, {{"test", {1, 2}}}),
                 "Invalid predicate costs/probabilities");
}