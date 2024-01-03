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
    auto testCase = testDataBase.getTestSample("F11_1");
    auto monitor{MtFsm({*testCase.formula}, true)};
    auto trace{testCase.trace};
    EXPECT_TRUE(TestDataBase::mergeResult(monitor.evaluate(trace).at(testCase.formula->getId())) ==
                TestDataBase::mergeResult(testCase.expectedResult))
        << "MtFsm formula test " << testCase.formula->getId() << " failed.";
}

TEST(TestMT_FSM, formulaTest) {
    size_t failures{0};
    auto testDataBase{TestDataBase()};
    auto testCases{testDataBase.getDatabaseEntries(supportedFormulaTypes.at(AlgorithmType::mt_fsm))};
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
        auto monitor{MtFsm({*testCase.formula}, pastEvaluation)};
        auto trace{testCase.trace};
        EXPECT_TRUE(TestDataBase::mergeResult(monitor.evaluate(trace).at(testCase.formula->getId())) ==
                    TestDataBase::mergeResult(testCase.expectedResult))
            << "MtFsm formula test " << testCase.formula->getId() << " failed.";
        if (!(TestDataBase::mergeResult(monitor.evaluate(trace).at(testCase.formula->getId())) ==
              TestDataBase::mergeResult(testCase.expectedResult)))
            failures++;
    }
    std::cout << "MtFsm Formula Test: " << std::to_string(failures) << "/" << std::to_string(testCases.size())
              << " failing formulas.";
}