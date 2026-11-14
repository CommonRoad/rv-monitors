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
    hydraTestUtils::executeHydraEval({*testCase.formula}, {testCase.expectedResult}, testCase.trace);
}

TEST(TestHydra, formulaTest) {
    std::vector<std::string> excludedTestCases{"F49_1"}; // TODO add Weak next support
    size_t failures{0};
    auto testDataBase{TestDataBase()};
    for (const auto &testCase : testDataBase.getDatabaseEntries(supportedFormulaTypes.at(AlgorithmType::hydra_mtl))) {
        if (std::any_of(excludedTestCases.begin(), excludedTestCases.end(),
                        [testCase](const std::string &name) { return name == testCase.formula->getId(); }))
            continue;
        std::shared_ptr<Trace> tra{testCase.trace};
        failures += hydraTestUtils::executeHydraEval({*testCase.formula}, {testCase.expectedResult}, tra);
    }
    std::cout << "Hydra Formula Test: " << std::to_string(failures) << " failing formulas.";
}

TEST(TestHydra, testToString) {
    Formula formula = Parser::parse("G(P(c))");
    formula.setId("R1");
    auto monitor{Hydra({formula})};
    EXPECT_EQ(monitor.to_string(), "G(P(c))");
}

TEST(TestHydra, testNoValidId) {
    Formula formula = Parser::parse("G(P[0, 1](c))");
    EXPECT_THROW(auto monitor{Hydra({formula})}, std::logic_error);
}

TEST(TestHydra, testGetFormula) {
    Formula formula = Parser::parse("G(P[0, 1](c))");
    formula.setId("R1");
    auto monitor{Hydra({formula})};
    EXPECT_EQ(monitor.getFormulas().size(), 1);
    EXPECT_EQ(monitor.getFormulas().at(0).getId(), "R1");
    EXPECT_EQ(monitor.getFormulas().at(0).to_string(), "G(P[0, 1](c))");
}
