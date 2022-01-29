//
// Created by Evald Nexhipi.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "test_formula_generator.h"
#include "monitors/utils/generators/formula_generator/formula_generator.h"

TEST(TestFormulaGenerator, generalTest) {

    FormulaGenerator fg;
    FormulaGenerator::rSeed();

    auto a1 = std::make_shared<Atomic>("a1");
    auto a2 = std::make_shared<Atomic>("a2");
    auto p1 = std::make_shared<Predicate>("p1", "p1", std::vector<std::string>{"params1", "params2"});

    fg.addAtomic(a1, 1.0);
    fg.addAtomic(a2, 1.0);
    fg.addPredicate(p1, 3.0);
    fg.addOperator(notSymbol, 1.0);
    fg.addOperator(andSymbol, 2.0);
    fg.addOperator(orSymbol, 2.0);
    fg.addOperator(xorSymbol, 2.0);
    fg.addOperator(implicationSymbol, 2.0);
    fg.addOperator(equivalenceSymbol, 2.0);
    fg.addOperator(sinceSymbol, 2.0);
    fg.addOperatorWithInterval(futureSymbol, 2.0, 0, 3, 10, 30);
    fg.addOperator(globallySymbol, 2.0);

    auto f = fg.generateFormula(7);
    std::cout<<f.to_string()<<std::endl;

    EXPECT_TRUE(true);
}

TEST(TestFormulaGenerator, testNoIntervals) {

    FormulaGenerator fg;
    FormulaGenerator::rSeed();

    auto a1 = std::make_shared<Atomic>("a1");
    auto a2 = std::make_shared<Atomic>("a2");
    auto p1 = std::make_shared<Predicate>("p1", "p1", std::vector<std::string>{"params1", "params2"});

    fg.addAtomic(a1, 1.0);
    fg.addAtomic(a2, 1.0);
    fg.addPredicate(p1, 3.0);
    fg.addOperator(notSymbol, 1.0);
    fg.addOperator(andSymbol, 2.0);
    fg.addOperator(orSymbol, 2.0);
    fg.addOperator(xorSymbol, 2.0);
    fg.addOperator(implicationSymbol, 2.0);
    fg.addOperator(equivalenceSymbol, 2.0);
    fg.addOperator(sinceSymbol, 2.0);
    fg.addOperator(globallySymbol, 2.0);

    auto f = fg.generateFormula(7);
    EXPECT_TRUE(true);
}

TEST(TestFormulaGenerator, testOneOperator) {

    FormulaGenerator fg;
    FormulaGenerator::rSeed();

    auto a1 = std::make_shared<Atomic>("a1");
    auto a2 = std::make_shared<Atomic>("a2");
    auto p1 = std::make_shared<Predicate>("p1", "p1", std::vector<std::string>{"params1", "params2"});

    fg.addAtomic(a1, 1.0);
    fg.addAtomic(a2, 1.0);
    fg.addPredicate(p1, 3.0);
    fg.addOperator(globallySymbol, 2.0);

    auto f = fg.generateFormula(1);

    EXPECT_TRUE(true);
}

TEST(TestFormulaGenerator, testNoOperator) {

    FormulaGenerator fg;
    FormulaGenerator::rSeed();

    auto a1 = std::make_shared<Atomic>("a1");
    auto a2 = std::make_shared<Atomic>("a2");
    auto p1 = std::make_shared<Predicate>("p1", "p1", std::vector<std::string>{"params1", "params2"});

    fg.addAtomic(a1, 1.0);
    fg.addAtomic(a2, 1.0);
    fg.addPredicate(p1, 3.0);
    fg.addOperator(notSymbol, 1.0);
    fg.addOperator(equivalenceSymbol, 5.0);
    fg.addOperator(sinceSymbol, 2.0);
    fg.addOperator(globallySymbol, 2.0);

    auto f = fg.generateFormula(0);

    EXPECT_TRUE(true);
}