//
// Created by Evald Nexhipi on 20.10.21.
//
#include "test_formula_generator.h"
#include "monitors/generators/formula_generator/formula_generator.h"

TEST(TestFormulaGenerator, tfg) {

    FormulaGenerator fg;
    fg.rseed();

    auto a1 = std::make_shared<Atomic>("a1");
    auto a2 = std::make_shared<Atomic>("a2");
    auto p1 = std::make_shared<Predicate>("p1", "na", std::vector<std::string>{"params1", "params2"});

    fg.AddAtomic(a1, 1.0);
    fg.AddAtomic(a2, 1.0);
    fg.AddPredicate(p1, 3.0);
    fg.AddOperator(OP_NOT, 1.0);
    fg.AddOperator(OP_AND, 2.0);
    fg.AddOperator(OP_OR, 2.0);
    fg.AddOperator(OP_XOR, 2.0);
    fg.AddOperator(OP_IF, 2.0);
    fg.AddOperator(OP_IFF, 2.0);
    fg.AddOperator(OP_SINCE, 2.0);
    fg.AddOperatorWithInterval(OP_FUTURE, 2.0, 0,  3, 10, 30);
    fg.AddOperator(OP_GLOBALLY, 2.0);

    auto f =  fg.GenerateFormula(7);
    std::cout << *f << std::endl;

    EXPECT_TRUE(true);
}
