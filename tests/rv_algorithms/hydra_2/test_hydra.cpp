//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//

#include "test_hydra.h"
#include "../test_database.h"
#include "hydra_test_utils.h"
#include "monitors/parser/parser.h"

TEST(TestHydra, testFormulaType) {
    Formula formulaOne = Parser::parse("G(a -> F(b))", "R1");
    Formula formulaTwo = Parser::parse("H(a -> F(b))", "R1");
    Formula formulaThree = Parser::parse("G(a -> F(b) & O(c))", "R1");
    Formula formulaFour = Parser::parse("a U b", "R1");
    EXPECT_NO_THROW(Hydra({formulaOne}));
    EXPECT_THROW(Hydra({formulaTwo}), std::logic_error);
    EXPECT_NO_THROW(Hydra({formulaThree}));
    EXPECT_THROW(Hydra({formulaFour}), std::logic_error);
}

TEST(TestHydra, specificFormulaTest) {
    auto testDataBase{TestDataBase()};
    auto testCase = testDataBase.getTestSample("F11_1");
    hydraTestUtils::executeHydraEval(testCase.trace, {*testCase.formula}, {testCase.expectedResult});
}

TEST(TestHydra, formulaTest) {
    std::vector<std::string> excludedTestCases{"F49_1"}; // TODO add Weak next support
    size_t failures{0};
    auto testDataBase{TestDataBase()};
    for (const auto &testCase : testDataBase.getDatabaseEntries(supportedFormulaTypes.at(AlgorithmType::hydra_mtl))) {
        if (std::any_of(excludedTestCases.begin(), excludedTestCases.end(),
                        [testCase](const std::string &name) { return name == testCase.formula->getId(); }))
            continue;
        failures += hydraTestUtils::executeHydraEval(testCase.trace, {*testCase.formula}, {testCase.expectedResult}, testCase.domainCollection, testCase.functionCollection);
    }
    std::cout << "Hydra Formula Test: " << std::to_string(failures - excludedTestCases.size()) << " failing formulas.";
}

TEST(TestHydra, testToString) {
    Formula formula = Parser::parse("G(P(c))");
    formula.setId("R1");
    auto monitor{Hydra({formula}, {}, {}, false, false, false)};
    EXPECT_EQ(monitor.to_string(), "G(P(c))");
}

TEST(TestHydra, testNoValidId) {
    Formula formula = Parser::parse("G(P[0, 1](c))");
    EXPECT_THROW(auto monitor{Hydra({formula}, {}, {}, false, false, false)}, std::logic_error);
}

TEST(TestHydra, testGetFormula) {
    Formula formula = Parser::parse("G(P[0, 1](c))");
    formula.setId("R1");
    auto monitor{Hydra({formula}, {}, {}, false, false, false)};
    EXPECT_EQ(monitor.getFormulas().size(), 1);
    EXPECT_EQ(monitor.getFormulas().at(0).getId(), "R1");
    EXPECT_EQ(monitor.getFormulas().at(0).to_string(), "G(P[0, 1](c))");
}

TEST(TestHydra, testCombinedSinceUntilFormulas) {
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

TEST(TestHydra, testExistentialStepLooping) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a", "b", "c"}, {}, {"b"}, {"a", "c"}});
    Formula formula = Parser::parse("G(C<=2 x in D. x)");
    formula.setId("R1");
    std::vector<Formula> formulas = {formula};
    std::vector<std::vector<Verdict>> expSolutions{{Verdict{0, false}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true}}};
    DomainCollection domainCollection = {Domain{"D", {"a", "b", "c"}}};
    hydraTestUtils::executeHydraEval(trace, formulas, expSolutions, domainCollection);
}
