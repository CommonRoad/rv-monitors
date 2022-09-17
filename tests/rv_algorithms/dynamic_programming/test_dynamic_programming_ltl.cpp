#include "test_dynamic_programming_ltl.h"
#include "../test_database.h"
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
    auto testCase = testDataBase.getTestSample("F11_2");
    auto monitor{DynamicProgrammingLTL({*testCase.formula})};
    auto trace{testCase.trace};
    EXPECT_TRUE(TestDataBase::mergeResult(monitor.evaluate(trace).at(testCase.formula->getId())) ==
                TestDataBase::mergeResult(testCase.expectedResult))
        << "DynamicProgrammingLTL formula test " << testCase.formula->getId() << " failed.";
}

TEST(TestDynamicProgrammingLTL, formulaTest) {
    std::vector<std::string> excludedTestCases{
        "F11_3", "F23_1", "F24_1", "F24_2", "F25_2", "F27_1",
        "F29_1", "F29_2", "F29_3", "F42_1", "F46_3"}; // TODO check why monitor fails
    size_t failures{0};
    auto testDataBase{TestDataBase()};
    auto testCases{testDataBase.getDatabaseEntries(supportedFormulaTypes.at(AlgorithmType::dynamic_programming_ltl))};
    for (const auto &testCase : testCases) {
        if (std::any_of(excludedTestCases.begin(), excludedTestCases.end(),
                        [testCase](const std::string &name) { return name == testCase.formula->getId(); }))
            continue;
        auto monitor{DynamicProgrammingLTL({*testCase.formula})};
        auto trace{testCase.trace};
        EXPECT_TRUE(TestDataBase::mergeResult(monitor.evaluate(trace).at(testCase.formula->getId())) ==
                    TestDataBase::mergeResult(testCase.expectedResult))
            << "DynamicProgrammingLTL formula test " << testCase.formula->getId() << " failed.";
        if (!(TestDataBase::mergeResult(monitor.evaluate(trace).at(testCase.formula->getId())) ==
              TestDataBase::mergeResult(testCase.expectedResult)))
            failures++;
    }
    std::cout << "DynamicProgrammingLTL Formula Test: " << std::to_string(failures) << "/"
              << std::to_string(testCases.size() - excludedTestCases.size()) << " failing formulas.";
}
