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
TEST(TestTraceGenerator, ttg) {
    TraceGenerator fg;
    TraceGenerator::rSeed();

    auto a1 = std::make_shared<Atomic>("a1");
    auto a2 = std::make_shared<Atomic>("a2");
    auto a3 = std::make_shared<Atomic>("a3");
    auto p1 = std::make_shared<Predicate>("p1", "p1", std::vector<std::string>{"params1", "params2"});
    auto p2 = std::make_shared<Predicate>("p2", "p2", std::vector<std::string>{"params1", "params2", "params3"});

    fg.addAtomic(a1, 1.0);
    fg.addAtomic(a2, 1.0);
    fg.addAtomic(a3, 3.0);
    fg.addPredicate(p1, 3.0);
    fg.addPredicate(p2, 2.0);

    auto f = fg.generateTrace(10);

    EXPECT_TRUE(true);
}

TEST(TestTraceGeneratorZeroLength, ttgzl) {
    TraceGenerator fg;
    TraceGenerator::rSeed();

    auto a1 = std::make_shared<Atomic>("a1");
    auto a2 = std::make_shared<Atomic>("a2");
    auto a3 = std::make_shared<Atomic>("a3");
    auto p1 = std::make_shared<Predicate>("p1", "p1", std::vector<std::string>{"params1", "params2"});
    auto p2 = std::make_shared<Predicate>("p2", "p2", std::vector<std::string>{"params1", "params2", "params3"});

    fg.addAtomic(a1, 1.0);
    fg.addAtomic(a2, 1.0);
    fg.addAtomic(a3, 3.0);
    fg.addPredicate(p1, 3.0);
    fg.addPredicate(p2, 2.0);

    auto f = fg.generateTrace(0);

    EXPECT_TRUE(true);
}

TEST(TestTraceGeneratorLengthOne, ttglo) {
    TraceGenerator fg;
    TraceGenerator::rSeed();

    auto a1 = std::make_shared<Atomic>("a1");
    auto a2 = std::make_shared<Atomic>("a2");
    auto a3 = std::make_shared<Atomic>("a3");
    auto p1 = std::make_shared<Predicate>("p1", "p1", std::vector<std::string>{"params1", "params2"});
    auto p2 = std::make_shared<Predicate>("p2", "p2", std::vector<std::string>{"params1", "params2", "params3"});

    fg.addAtomic(a1, 1.0);
    fg.addAtomic(a2, 1.0);
    fg.addAtomic(a3, 3.0);
    fg.addPredicate(p1, 3.0);
    fg.addPredicate(p2, 2.0);

    auto f = fg.generateTrace(1);

    EXPECT_TRUE(true);
}