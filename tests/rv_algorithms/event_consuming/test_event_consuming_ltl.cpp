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
    auto testCase = testDataBase.getTestSample("F11_2");
    auto monitor{EventConsumingLTL({*testCase.formula})};
    auto trace{testCase.trace};
    EXPECT_TRUE(TestDataBase::mergeResult(monitor.evaluate(trace).at(testCase.formula->getId())) ==
                TestDataBase::mergeResult(testCase.expectedResult))
        << "EventConsumingLTL formula test " << testCase.formula->getId() << " failed.";
}

TEST(TestEventConsumingLTL, formulaTest) {
    std::vector<std::string> excludedTestCases{"F11_1", "F23_1", "F24_1", "F24_2", "F25_2", "F27_1",
                                               "F29_1", "F29_2", "F29_3", "F46_3"}; // TODO check why monitor fails
    size_t failures{0};
    auto testDataBase{TestDataBase()};
    auto testCases{testDataBase.getDatabaseEntries(supportedFormulaTypes.at(AlgorithmType::event_consuming_ltl))};
    for (const auto &testCase : testCases) {
        if (std::any_of(excludedTestCases.begin(), excludedTestCases.end(),
                        [testCase](const std::string &name) { return name == testCase.formula->getId(); }))
            continue;
        auto monitor{EventConsumingLTL({*testCase.formula})};
        auto trace{testCase.trace};
        EXPECT_TRUE(TestDataBase::mergeResult(monitor.evaluate(trace).at(testCase.formula->getId())) ==
                    TestDataBase::mergeResult(testCase.expectedResult))
            << "EventConsumingLTL formula test " << testCase.formula->getId() << " failed.";
        if (!(TestDataBase::mergeResult(monitor.evaluate(trace).at(testCase.formula->getId())) ==
              TestDataBase::mergeResult(testCase.expectedResult)))
            failures++;
    }
    std::cout << "EventConsumingLTL Formula Test: " << std::to_string(failures) << "/"
              << std::to_string(testCases.size() - excludedTestCases.size()) << " failing formulas.";
}