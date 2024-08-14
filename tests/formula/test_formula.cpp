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
    formulaTypes.emplace_back(FormulaType::futureOnly, true);
    formulaTypes.emplace_back(FormulaType::pastOnly, true);
    formulaTypes.emplace_back(FormulaType::safetyGeneral, true);
    formulaTypes.emplace_back(FormulaType::pastSafety, false);
    formulaTypes.emplace_back(FormulaType::futurePastCombined, true);
    formulaTypes.emplace_back(FormulaType::safetyGeneral, false);
    formulaTypes.emplace_back(FormulaType::futureSafety, false);
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

TEST(TestFormula, formulaDomainExtraction) {
    Formula formula{
        Parser::parse("in_same_lane('x_ego') & A dir1 in DDIR . same_dir('x_ego', dir1) & in_same_lane('x_ego')")};
    auto variables{formula.getDomains()};
    EXPECT_EQ(variables.size(), 1);
    EXPECT_EQ(variables.at(0).getName(), "dir1");
    EXPECT_EQ(variables.at(0).getDomain().to_string(), "DDIR");

    Formula formula2{Parser::parse(
        "in_same_lane('x_ego') & A dir1, dir2 in DDIR . same_dir('x_ego', dir1, dir2) & in_same_lane('x_ego')")};
    auto variables2{formula2.getDomains()};
    EXPECT_EQ(variables2.size(), 2);
    EXPECT_EQ(variables2.at(0).getName(), "dir1");
    EXPECT_EQ(variables2.at(1).getName(), "dir2");
    EXPECT_EQ(variables2.at(0).getDomain().to_string(), "DDIR");
}
