//
// Created by Evald Nexhipi on 23.10.21.
//
#include "test_trace_generator.h"
#include "monitors/expression_tree/atomic/atomic.h"
#include "monitors/expression_tree/atomic/predicate.h"
#include "monitors/utils/generators/trace_generator/trace_generator.h"
TEST(TestTraceGenerator, ttg) {
    TraceGenerator fg;
    TraceGenerator::rseed();

    auto a1 = std::make_shared<Atomic>("a1");
    auto a2 = std::make_shared<Atomic>("a2");
    auto a3 = std::make_shared<Atomic>("a3");
    auto p1 = std::make_shared<Predicate>("p1", "na", std::vector<std::string>{"params1", "params2"});
    auto p2 = std::make_shared<Predicate>("p2", "na", std::vector<std::string>{"params1", "params2", "params3"});

    fg.AddAtomic(a1, 1.0);
    fg.AddAtomic(a2, 1.0);
    fg.AddAtomic(a3, 3.0);
    fg.AddPredicate(p1, 3.0);
    fg.AddPredicate(p2, 2.0);

    auto f = fg.GenerateTrace(10);
    std::cout << f.to_string() << std::endl;

    EXPECT_TRUE(true);
}

TEST(TestTraceGeneratorZeroLength, ttgzl) {
    TraceGenerator fg;
    TraceGenerator::rseed();

    auto a1 = std::make_shared<Atomic>("a1");
    auto a2 = std::make_shared<Atomic>("a2");
    auto a3 = std::make_shared<Atomic>("a3");
    auto p1 = std::make_shared<Predicate>("p1", "na", std::vector<std::string>{"params1", "params2"});
    auto p2 = std::make_shared<Predicate>("p2", "na", std::vector<std::string>{"params1", "params2", "params3"});

    fg.AddAtomic(a1, 1.0);
    fg.AddAtomic(a2, 1.0);
    fg.AddAtomic(a3, 3.0);
    fg.AddPredicate(p1, 3.0);
    fg.AddPredicate(p2, 2.0);

    auto f = fg.GenerateTrace(0);
    std::cout << f.to_string() << std::endl;

    EXPECT_TRUE(true);
}

TEST(TestTraceGeneratorLengthOne, ttglo) {
    TraceGenerator fg;
    TraceGenerator::rseed();

    auto a1 = std::make_shared<Atomic>("a1");
    auto a2 = std::make_shared<Atomic>("a2");
    auto a3 = std::make_shared<Atomic>("a3");
    auto p1 = std::make_shared<Predicate>("p1", "na", std::vector<std::string>{"params1", "params2"});
    auto p2 = std::make_shared<Predicate>("p2", "na", std::vector<std::string>{"params1", "params2", "params3"});

    fg.AddAtomic(a1, 1.0);
    fg.AddAtomic(a2, 1.0);
    fg.AddAtomic(a3, 3.0);
    fg.AddPredicate(p1, 3.0);
    fg.AddPredicate(p2, 2.0);

    auto f = fg.GenerateTrace(1);
    std::cout << f.to_string() << std::endl;

    EXPECT_TRUE(true);
}