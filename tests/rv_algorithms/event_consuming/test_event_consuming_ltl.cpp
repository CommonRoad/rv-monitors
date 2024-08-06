#include "test_event_consuming_ltl.h"
#include "../test_database.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/ltl_event_consuming/event_consuming.h"
#include "monitors/trace/trace.h"

TEST(TestEventConsumingLTL, testFormulaType) {
    Formula formulaOne = Parser::parse("G(a -> F(b))");
    Formula formulaTwo = Parser::parse("H(a -> F(b))");
    Formula formulaThree = Parser::parse("G(a -> F(b) & O(c))");
    EXPECT_NO_THROW(EventConsumingLTL({formulaOne}, false));
    EXPECT_THROW(EventConsumingLTL({formulaTwo}, false), std::logic_error);
    EXPECT_THROW(EventConsumingLTL({formulaThree}, false), std::logic_error);
}

TEST(TestEventConsumingLTL, specificFormulaTest) {
    auto testDataBase{TestDataBase()};
    auto testCase = testDataBase.getTestSample("F11_1");
    auto monitor{EventConsumingLTL({*testCase.formula}, true)};
    auto trace{testCase.trace};
    EXPECT_TRUE(TestDataBase::mergeResult(monitor.evaluate(trace).at(testCase.formula->getId())) ==
                TestDataBase::mergeResult(testCase.expectedResult))
        << "EventConsumingLTL formula test " << testCase.formula->getId() << " failed.";
}

TEST(TestEventConsumingLTL, formulaTest) {
    size_t failures{0};
    auto testDataBase{TestDataBase()};
    auto testCases{testDataBase.getDatabaseEntries(supportedFormulaTypes.at(AlgorithmType::event_consuming_ltl))};
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
        auto monitor{EventConsumingLTL({*testCase.formula}, pastEvaluation)};
        auto trace{testCase.trace};
        EXPECT_TRUE(TestDataBase::mergeResult(monitor.evaluate(trace).at(testCase.formula->getId())) ==
                    TestDataBase::mergeResult(testCase.expectedResult))
            << "EventConsumingLTL formula test " << testCase.formula->getId() << " failed.";
        if (!(TestDataBase::mergeResult(monitor.evaluate(trace).at(testCase.formula->getId())) ==
              TestDataBase::mergeResult(testCase.expectedResult)))
            failures++;
    }
    std::cout << "EventConsumingLTL Formula Test: " << std::to_string(failures) << "/"
              << std::to_string(testCases.size()) << " failing formulas.";
}
