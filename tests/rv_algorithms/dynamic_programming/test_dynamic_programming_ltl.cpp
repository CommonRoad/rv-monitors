#include "test_dynamic_programming_ltl.h"
#include "../test_database.h"
#include "monitors/expression_tree/binary/temporal/since.h"
#include "monitors/expression_tree/unary/temporal/historically.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/dynamic_programming/dynamic.h"
#include "monitors/trace/trace.h"

TEST(TestDynamicProgrammingLTL, testFormulaType) {
    Formula formulaOne = Parser::parse("G(a -> F(b))");
    Formula formulaTwo = Parser::parse("H(a -> F(b))");
    Formula formulaThree = Parser::parse("G(a -> F(b) & O(c))");
    EXPECT_NO_THROW(DynamicProgrammingLTL({formulaOne}));
    EXPECT_THROW(DynamicProgrammingLTL({formulaTwo}), std::logic_error);
    EXPECT_THROW(DynamicProgrammingLTL({formulaThree}), std::logic_error);
}

TEST(TestDynamicProgrammingLTL, specificFormulaTest) {
    auto testDataBase{TestDataBase()};
    auto testCase = testDataBase.getTestSample("F11_1");
    auto monitor{DynamicProgrammingLTL({*testCase.formula}, true)};
    auto trace{testCase.trace};
    EXPECT_TRUE(TestDataBase::mergeResult(monitor.evaluate(trace).at(testCase.formula->getId())) ==
                TestDataBase::mergeResult(testCase.expectedResult))
        << "DynamicProgrammingLTL formula test " << testCase.formula->getId() << " failed.";
}

TEST(TestDynamicProgrammingLTL, formulaTest) {
    size_t failures{0};
    auto testDataBase{TestDataBase()};
    auto testCases{testDataBase.getDatabaseEntries(supportedFormulaTypes.at(AlgorithmType::dynamic_programming_ltl))};
    for (const auto &testCase : testCases) {
        if (testCase.formula->includeTemporalOperator('S') or testCase.formula->includeTemporalOperator('H'))
            continue;
        bool pastEvaluation;
        if (testCase.formula->checkCompatibility(FormulaType::pastSafety))
            pastEvaluation = true;
        else if (testCase.formula->checkCompatibility(FormulaType::futureSafety))
            pastEvaluation = false;
        else
            continue;
        auto monitor{DynamicProgrammingLTL({*testCase.formula}, pastEvaluation)};
        auto trace{testCase.trace};
        EXPECT_TRUE(TestDataBase::mergeResult(monitor.evaluate(trace).at(testCase.formula->getId())) ==
                    TestDataBase::mergeResult(testCase.expectedResult))
            << "DynamicProgrammingLTL formula test " << testCase.formula->getId() << " failed.";
        if (!(TestDataBase::mergeResult(monitor.evaluate(trace).at(testCase.formula->getId())) ==
              TestDataBase::mergeResult(testCase.expectedResult)))
            failures++;
    }
    std::cout << "DynamicProgrammingLTL Formula Test: " << std::to_string(failures) << "/"
              << std::to_string(testCases.size()) << " failing formulas.";
}
