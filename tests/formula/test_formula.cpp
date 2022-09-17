//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2022 - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//

#include "test_formula.h"
#include "monitors/parser/parser.h"
#include <iostream>

TEST(TestFormula, testCheckCompatibility) {
    std::vector<std::unique_ptr<Formula>> formulas;
    formulas.push_back(std::make_unique<Formula>(Parser::parse("O[0, 1](c)")));
    formulas.push_back(std::make_unique<Formula>(Parser::parse("X[0, 1](c)")));
    formulas.push_back(std::make_unique<Formula>(Parser::parse("G(a -> F[0, 6](b & O[0, 6](c)))")));
    formulas.push_back(std::make_unique<Formula>(Parser::parse("G(a -> F(b & O(c)))")));
    formulas.push_back(std::make_unique<Formula>(Parser::parse("G((a -> O(c)))")));
    formulas.push_back(std::make_unique<Formula>(Parser::parse("G(F(c))")));
    std::vector<MonitoringProperties> formulaTypes;
    formulaTypes.emplace_back(TemporalFormulaType::futureOnly, true);
    formulaTypes.emplace_back(TemporalFormulaType::pastOnly, true);
    formulaTypes.emplace_back(TemporalFormulaType::safetyGeneral, true);
    formulaTypes.emplace_back(TemporalFormulaType::pastSafety, false);
    formulaTypes.emplace_back(TemporalFormulaType::futurePastCombined, true);
    formulaTypes.emplace_back(TemporalFormulaType::safetyGeneral, false);
    formulaTypes.emplace_back(TemporalFormulaType::futureSafety, false);
    std::vector<bool> expectedResult{false, true,  false, false, true,  false, false, true,  false, false, false,
                                     true,  false, false, false, false, true,  false, true,  false, false, false,
                                     false, true,  false, true,  true,  false, false, false, true,  true,  true,
                                     true,  false, true,  false, true,  false, true,  true,  true};
    size_t idx{0};
    for (const auto &formula : formulas)
        for (const auto &formulaType : formulaTypes) {
            EXPECT_EQ(formula->checkCompatibility(formulaType), expectedResult.at(idx))
                << "Error at idx: " + std::to_string(idx);
            ++idx;
        }
}
