#include "test_formula_generator.h"
#include "monitors/expression_tree/atomic/predicate.h"
#include "monitors/utils/generators/formula_generator/formula_generator.h"

TEST(TestFormulaGenerator, generalTest) {

    FormulaGenerator formGe;
    FormulaGenerator::rSeed();

    auto ato1 = std::make_shared<Atomic>("a");
    auto ato2 = std::make_shared<Atomic>("b");
    auto pred1 =
        std::make_shared<Predicate>("p", "p", std::vector<std::string>{"p1", "p2"}, std::vector<std::string>{});

    formGe.addAtomic(ato1, 1.0);
    formGe.addAtomic(ato2, 1.0);
    formGe.addPredicate(pred1, 3.0);
    formGe.addOperator(notSymbol, 1.0);
    formGe.addOperator(andSymbol, 2.0);
    formGe.addOperator(orSymbol, 2.0);
    formGe.addOperator(xorSymbol, 2.0);
    formGe.addOperator(implicationSymbol, 2.0);
    formGe.addOperator(equivalenceSymbol, 2.0);
    formGe.addOperator(sinceSymbol, 2.0);
    formGe.addOperatorWithInterval(untilSymbol, 2.0, 0, 3, 5, 10);
    formGe.addOperator(globallySymbol, 2.0);

    // auto f = fg.generateFormula(7);
    auto formula = formGe.generateFormula(4);
    std::cout << formula.to_string() << std::endl;

    EXPECT_TRUE(true);
}

TEST(TestFormulaGenerator, testNoIntervals) {

    FormulaGenerator formGe;
    FormulaGenerator::rSeed();

    auto ato1 = std::make_shared<Atomic>("a1");
    auto ato2 = std::make_shared<Atomic>("a2");
    auto pred1 = std::make_shared<Predicate>("p1", "p1", std::vector<std::string>{"params1", "params2"},
                                             std::vector<std::string>{});

    formGe.addAtomic(ato1, 1.0);
    formGe.addAtomic(ato2, 1.0);
    formGe.addPredicate(pred1, 3.0);
    formGe.addOperator(notSymbol, 1.0);
    formGe.addOperator(andSymbol, 2.0);
    formGe.addOperator(orSymbol, 2.0);
    formGe.addOperator(xorSymbol, 2.0);
    formGe.addOperator(implicationSymbol, 2.0);
    formGe.addOperator(equivalenceSymbol, 2.0);
    formGe.addOperator(sinceSymbol, 2.0);
    formGe.addOperator(globallySymbol, 2.0);

    auto formula = formGe.generateFormula(7);
    EXPECT_TRUE(true);
}

TEST(TestFormulaGenerator, testOneOperator) {

    FormulaGenerator formGe;
    FormulaGenerator::rSeed();

    auto ato1 = std::make_shared<Atomic>("a1");
    auto ato2 = std::make_shared<Atomic>("a2");
    auto pred1 = std::make_shared<Predicate>("p1", "p1", std::vector<std::string>{"params1", "params2"},
                                             std::vector<std::string>{});

    formGe.addAtomic(ato1, 1.0);
    formGe.addAtomic(ato2, 1.0);
    formGe.addPredicate(pred1, 3.0);
    formGe.addOperator(globallySymbol, 2.0);

    auto formula = formGe.generateFormula(1);

    EXPECT_TRUE(true);
}

TEST(TestFormulaGenerator, testNoOperator) {

    FormulaGenerator formGe;
    FormulaGenerator::rSeed();

    auto ato1 = std::make_shared<Atomic>("a1");
    auto ato2 = std::make_shared<Atomic>("a2");
    auto pred1 = std::make_shared<Predicate>("p1", "p1", std::vector<std::string>{"params1", "params2"},
                                             std::vector<std::string>{});

    formGe.addAtomic(ato1, 1.0);
    formGe.addAtomic(ato2, 1.0);
    formGe.addPredicate(pred1, 3.0);
    formGe.addOperator(notSymbol, 1.0);
    formGe.addOperator(equivalenceSymbol, 5.0);
    formGe.addOperator(sinceSymbol, 2.0);
    formGe.addOperator(globallySymbol, 2.0);

    auto formula = formGe.generateFormula(0);

    EXPECT_TRUE(true);
}