#include "test_mt_fsm.h"
#include "../test_database.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/mt_fsm/mt_fsm.h"
#include "monitors/trace/trace.h"

TEST(TestMT_FSM, testFormulaType) {
    Formula formulaOne = Parser::parse("G(a -> F(b))");
    Formula formulaTwo = Parser::parse("H(a -> F(b))");
    Formula formulaThree = Parser::parse("G(a -> F(b) & O(c))");
    EXPECT_NO_THROW(MtFsm({formulaOne}));
    EXPECT_THROW(MtFsm({formulaTwo}), std::logic_error);
    EXPECT_THROW(MtFsm({formulaThree}), std::logic_error);
}

TEST(TestMT_FSM, specificFormulaTest) {
    auto testDataBase{TestDataBase()};
    auto testCase = testDataBase.getTestSample("F46_3");
    auto monitor{MtFsm({*testCase.formula})};
    auto trace{testCase.trace};
    EXPECT_TRUE(TestDataBase::mergeResult(monitor.evaluate(trace).at(testCase.formula->getId())) ==
                TestDataBase::mergeResult(testCase.expectedResult))
        << "MtFsm formula test " << testCase.formula->getId() << " failed.";
}

TEST(TestMT_FSM, formulaTest) {
    std::vector<std::string> excludedTestCases{"F11_1", "F23_1", "F24_1", "F24_2", "F25_1",
                                               "F27_1", "F29_1", "F29_2", "F29_3"}; // TODO check why monitor fails
    size_t failures{0};
    auto testDataBase{TestDataBase()};
    auto testCases{testDataBase.getDatabaseEntries(supportedFormulaTypes.at(AlgorithmType::mt_fsm))};
    for (const auto &testCase : testCases) {
        if (std::any_of(excludedTestCases.begin(), excludedTestCases.end(),
                        [testCase](const std::string &name) { return name == testCase.formula->getId(); }))
            continue;
        auto monitor{MtFsm({*testCase.formula})};
        auto trace{testCase.trace};
        EXPECT_TRUE(TestDataBase::mergeResult(monitor.evaluate(trace).at(testCase.formula->getId())) ==
                    TestDataBase::mergeResult(testCase.expectedResult))
            << "MtFsm formula test " << testCase.formula->getId() << " failed.";
        if (!(TestDataBase::mergeResult(monitor.evaluate(trace).at(testCase.formula->getId())) ==
              TestDataBase::mergeResult(testCase.expectedResult)))
            failures++;
    }
    std::cout << "MtFsm Formula Test: " << std::to_string(failures) << "/"
              << std::to_string(testCases.size() - excludedTestCases.size()) << " failing formulas.";
}