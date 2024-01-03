//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//

#include "test_mtl_event_consuming.h"
#include "../test_database.h"
#include "monitors/rv_algorithms/mtl_event_consuming//mtl_event_consuming.h"
#include "monitors/trace/trace.h"

TEST(TestEventConsumingMTL, specificFormulaTest) {
    auto testDataBase{TestDataBase()};
    auto testCase = testDataBase.getTestSample("F11_1");
    auto monitor{MTLEventConsuming({*testCase.formula})};
    auto trace{testCase.trace};
    auto res{TestDataBase::mergeResult(monitor.evaluate(trace).at(testCase.formula->getId()))};
    auto exp{TestDataBase::mergeResult(testCase.expectedResult)};
    EXPECT_TRUE(res == exp) << "MTL-EventConsuming formula test " << testCase.formula->getId() << " failed.";
}

TEST(TestEventConsumingMTL, formulaTest) {
    size_t failures{0};
    auto testDataBase{TestDataBase()};
    auto testCases{testDataBase.getDatabaseEntries(supportedFormulaTypes.at(AlgorithmType::event_consuming_mtl))};
    for (const auto &testCase : testCases) {
        auto monitor{MTLEventConsuming({*testCase.formula})};
        auto trace{testCase.trace};
        EXPECT_TRUE(TestDataBase::mergeResult(monitor.evaluate(trace).at(testCase.formula->getId())) ==
                    TestDataBase::mergeResult(testCase.expectedResult))
            << "MTL-EventConsuming formula test " << testCase.formula->getId() << " failed.";
        if (!(TestDataBase::mergeResult(monitor.evaluate(trace).at(testCase.formula->getId())) ==
              TestDataBase::mergeResult(testCase.expectedResult)))
            failures++;
    }
    std::cout << "MTL Event-Consuming Formula Test: " << std::to_string(failures) << "/"
              << std::to_string(testCases.size()) << " failing formulas.";
}
