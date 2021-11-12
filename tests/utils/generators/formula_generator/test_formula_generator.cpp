//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "test_formula_generator.h"
#include "monitors/utils/generators/formula_generator/formula_generator.h"

TEST(TestFormulaGenerator, tfg) {

    FormulaGenerator fg;
    fg.rseed();

    auto a1 = std::make_shared<Atomic>("a1");
    auto a2 = std::make_shared<Atomic>("a2");
    auto p1 = std::make_shared<Predicate>("p1", "p1", std::vector<std::string>{"params1", "params2"});

    fg.addAtomic(a1, 1.0);
    fg.addAtomic(a2, 1.0);
    fg.addPredicate(p1, 3.0);
    fg.addOperator(OP_NOT, 1.0);
    fg.addOperator(OP_AND, 2.0);
    fg.addOperator(OP_OR, 2.0);
    fg.addOperator(OP_XOR, 2.0);
    fg.addOperator(OP_IF, 2.0);
    fg.addOperator(OP_IFF, 2.0);
    fg.addOperator(OP_SINCE, 2.0);
    fg.addOperatorWithInterval(OP_FUTURE, 2.0, 0, 3, 10, 30);
    fg.addOperator(OP_GLOBALLY, 2.0);

    auto f = fg.generateFormula(7);
    std::cout << f << std::endl;

    EXPECT_TRUE(true);
}

TEST(TestFormulaGeneratorNoIntervals, tfgni) {

    FormulaGenerator fg;
    fg.rseed();

    auto a1 = std::make_shared<Atomic>("a1");
    auto a2 = std::make_shared<Atomic>("a2");
    auto p1 = std::make_shared<Predicate>("p1", "p1", std::vector<std::string>{"params1", "params2"});

    fg.addAtomic(a1, 1.0);
    fg.addAtomic(a2, 1.0);
    fg.addPredicate(p1, 3.0);
    fg.addOperator(OP_NOT, 1.0);
    fg.addOperator(OP_AND, 2.0);
    fg.addOperator(OP_OR, 2.0);
    fg.addOperator(OP_XOR, 2.0);
    fg.addOperator(OP_IF, 2.0);
    fg.addOperator(OP_IFF, 2.0);
    fg.addOperator(OP_SINCE, 2.0);
    fg.addOperator(OP_GLOBALLY, 2.0);

    auto f = fg.generateFormula(7);
    std::cout << f << std::endl;

    EXPECT_TRUE(true);
}

TEST(TestFormulaGeneratorOneOperator, tfgoo) {

    FormulaGenerator fg;
    fg.rseed();

    auto a1 = std::make_shared<Atomic>("a1");
    auto a2 = std::make_shared<Atomic>("a2");
    auto p1 = std::make_shared<Predicate>("p1", "p1", std::vector<std::string>{"params1", "params2"});

    fg.addAtomic(a1, 1.0);
    fg.addAtomic(a2, 1.0);
    fg.addPredicate(p1, 3.0);
    fg.addOperator(OP_GLOBALLY, 2.0);

    auto f = fg.generateFormula(1);
    std::cout << f << std::endl;

    EXPECT_TRUE(true);
}

TEST(TestFormulaGeneratorNoOperator, tfgno) {

    FormulaGenerator fg;
    fg.rseed();

    auto a1 = std::make_shared<Atomic>("a1");
    auto a2 = std::make_shared<Atomic>("a2");
    auto p1 = std::make_shared<Predicate>("p1", "p1", std::vector<std::string>{"params1", "params2"});

    fg.addAtomic(a1, 1.0);
    fg.addAtomic(a2, 1.0);
    fg.addPredicate(p1, 3.0);
    fg.addOperator(OP_NOT, 1.0);
    fg.addOperator(OP_IFF, 5.0);
    fg.addOperator(OP_SINCE, 2.0);
    fg.addOperator(OP_GLOBALLY, 2.0);

    auto f = fg.generateFormula(0);
    std::cout << f << std::endl;

    EXPECT_TRUE(true);
}