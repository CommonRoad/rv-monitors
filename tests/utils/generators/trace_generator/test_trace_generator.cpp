//
// Created by Evald Nexhipi.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "test_trace_generator.h"
#include "monitors/expression_tree/atomic/atomic.h"
#include "monitors/expression_tree/atomic/predicate.h"
#include "monitors/utils/generators/trace_generator/trace_generator.h"

TEST(TestTraceGenerator, testrageneral) {
    TraceGenerator trag;
    TraceGenerator::rSeed();

    auto atom1 = std::make_shared<Atomic>("a1");
    auto atom2 = std::make_shared<Atomic>("a2");
    auto atom3 = std::make_shared<Atomic>("a3");
    auto pred1 = std::make_shared<Predicate>("p1", "p1", std::vector<std::string>{"params1", "params2"});
    auto pred2 = std::make_shared<Predicate>("p2", "p2", std::vector<std::string>{"params1", "params2", "params3"});

    trag.addAtomic(atom1, 1.0);
    trag.addAtomic(atom2, 1.0);
    trag.addAtomic(atom3, 3.0);
    trag.addPredicate(pred1, 3.0);
    trag.addPredicate(pred2, 2.0);

    auto tra = trag.generateTrace(10);

    EXPECT_TRUE(true);
}

TEST(TestTraceGenerator, testZeroLength) {
    TraceGenerator trag;
    TraceGenerator::rSeed();

    auto atom1 = std::make_shared<Atomic>("a1");
    auto atom2 = std::make_shared<Atomic>("a2");
    auto atom3 = std::make_shared<Atomic>("a3");
    auto pred1 = std::make_shared<Predicate>("p1", "p1", std::vector<std::string>{"params1", "params2"});
    auto pred2 = std::make_shared<Predicate>("p2", "p2", std::vector<std::string>{"params1", "params2", "params3"});

    trag.addAtomic(atom1, 1.0);
    trag.addAtomic(atom2, 1.0);
    trag.addAtomic(atom3, 3.0);
    trag.addPredicate(pred1, 3.0);
    trag.addPredicate(pred2, 2.0);

    auto tra = trag.generateTrace(0);

    EXPECT_TRUE(true);
}

TEST(TestTraceGenerator, testLengthOne) {
    TraceGenerator trag;
    TraceGenerator::rSeed();

    auto atom1 = std::make_shared<Atomic>("a1");
    auto atom2 = std::make_shared<Atomic>("a2");
    auto atom3 = std::make_shared<Atomic>("a3");
    auto pred1 = std::make_shared<Predicate>("p1", "p1", std::vector<std::string>{"params1", "params2"});
    auto pred2 = std::make_shared<Predicate>("p2", "p2", std::vector<std::string>{"params1", "params2", "params3"});

    trag.addAtomic(atom1, 1.0);
    trag.addAtomic(atom2, 1.0);
    trag.addAtomic(atom3, 3.0);
    trag.addPredicate(pred1, 3.0);
    trag.addPredicate(pred2, 2.0);

    auto tra = trag.generateTrace(1);

    EXPECT_TRUE(true);
}