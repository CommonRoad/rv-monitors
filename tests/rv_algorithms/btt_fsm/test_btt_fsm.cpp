#include "test_btt_fsm.h"
#include "../test_database.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/btt_fsm/btt_fsm_utils_creation.h"
#include "monitors/trace/trace.h"

TEST(TestBTT_FSMEvaluation, testFormulaType) {
    Formula formulaOne = Parser::parse("G(a -> F(b))");
    Formula formulaTwo = Parser::parse("H(a -> F(b))");
    Formula formulaThree = Parser::parse("G(a -> F(b) & O(c))");
    Formula formulaFour = Parser::parse("G(a -> O(c))");
    EXPECT_NO_THROW(BttFsm({formulaOne}));
    EXPECT_THROW(BttFsm({formulaTwo}), std::logic_error);
    EXPECT_THROW(BttFsm({formulaThree}), std::logic_error);
    EXPECT_NO_THROW(BttFsm({formulaFour}));
}

TEST(TestBTT_FSMEvaluation, specificFormulaTest) {
    auto testDataBase{TestDataBase()};
    auto testCase = testDataBase.getTestSample("F46_3");
    auto monitor{BttFsm({*testCase.formula})};
    auto trace{testCase.trace};
    EXPECT_TRUE(TestDataBase::mergeResult(monitor.evaluate(trace).at(testCase.formula->getId())) ==
                TestDataBase::mergeResult(testCase.expectedResult))
        << "BTT formula test " << testCase.formula->getId() << " failed.";
}

TEST(TestBTT_FSMEvaluation, formulaTest) {
    std::vector<std::string> excludedTestCases{"F11_1", "F23_1", "F24_1", "F24_2", "F25_1",
                                               "F27_1", "F29_1", "F29_2", "F29_3"}; // TODO check why monitor fails
    size_t failures{0};
    auto testDataBase{TestDataBase()};
    auto testCases{testDataBase.getDatabaseEntries(supportedFormulaTypes.at(AlgorithmType::btt_fsm))};
    for (const auto &testCase : testCases) {
        if (std::any_of(excludedTestCases.begin(), excludedTestCases.end(),
                        [testCase](const std::string &name) { return name == testCase.formula->getId(); }))
            continue;
        auto monitor{BttFsm({*testCase.formula})};
        auto trace{testCase.trace};
        EXPECT_TRUE(TestDataBase::mergeResult(monitor.evaluate(trace).at(testCase.formula->getId())) ==
                    TestDataBase::mergeResult(testCase.expectedResult))
            << "BTT formula test " << testCase.formula->getId() << " failed.";
        if (!(TestDataBase::mergeResult(monitor.evaluate(trace).at(testCase.formula->getId())) ==
              TestDataBase::mergeResult(testCase.expectedResult)))
            failures++;
    }
    std::cout << "BTT Formula Test: " << std::to_string(failures) << "/"
              << std::to_string(testCases.size() - excludedTestCases.size()) << " failing formulas.";
}
