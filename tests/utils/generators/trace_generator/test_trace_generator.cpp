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

TEST(TestTraceGenerator, testGeneral) {
    TraceGenerator trg;
    TraceGenerator::rSeed();

    auto ato1 = std::make_shared<Atomic>("a1");
    auto ato2 = std::make_shared<Atomic>("a2");
    auto ato3 = std::make_shared<Atomic>("a3");
    auto pred1 = std::make_shared<Predicate>("p1", "p1", std::vector<std::string>{"params1", "params2"},
                                             std::vector<std::string>{});
    auto pred2 = std::make_shared<Predicate>("p2", "p2", std::vector<std::string>{"params1", "params2", "params3"},
                                             std::vector<std::string>{});

    trg.addAtomic(ato1, 1.0);
    trg.addAtomic(ato2, 1.0);
    trg.addAtomic(ato3, 3.0);
    trg.addPredicate(pred1, 3.0);
    trg.addPredicate(pred2, 2.0);

    auto tra = trg.generateTrace(10);

    EXPECT_TRUE(true);
}

TEST(TestTraceGenerator, testZeroLength) {
    TraceGenerator trg;
    TraceGenerator::rSeed();

    auto ato1 = std::make_shared<Atomic>("a1");
    auto ato2 = std::make_shared<Atomic>("a2");
    auto ato3 = std::make_shared<Atomic>("a3");
    auto pred1 = std::make_shared<Predicate>("p1", "p1", std::vector<std::string>{"params1", "params2"},
                                             std::vector<std::string>{});
    auto pred2 = std::make_shared<Predicate>("p2", "p2", std::vector<std::string>{"params1", "params2", "params3"},
                                             std::vector<std::string>{});

    trg.addAtomic(ato1, 1.0);
    trg.addAtomic(ato2, 1.0);
    trg.addAtomic(ato3, 3.0);
    trg.addPredicate(pred1, 3.0);
    trg.addPredicate(pred2, 2.0);

    auto tra = trg.generateTrace(0);

    EXPECT_TRUE(true);
}

TEST(TestTraceGenerator, testLengthOne) {
    TraceGenerator trg;
    TraceGenerator::rSeed();

    auto ato1 = std::make_shared<Atomic>("a1");
    auto ato2 = std::make_shared<Atomic>("a2");
    auto ato3 = std::make_shared<Atomic>("a3");
    auto pred1 = std::make_shared<Predicate>("p1", "p1", std::vector<std::string>{"params1", "params2"},
                                             std::vector<std::string>{});
    auto pred2 = std::make_shared<Predicate>("p2", "p2", std::vector<std::string>{"params1", "params2", "params3"},
                                             std::vector<std::string>{});

    trg.addAtomic(ato1, 1.0);
    trg.addAtomic(ato2, 1.0);
    trg.addAtomic(ato3, 3.0);
    trg.addPredicate(pred1, 3.0);
    trg.addPredicate(pred2, 2.0);

    auto tra = trg.generateTrace(1);

    EXPECT_TRUE(true);
}