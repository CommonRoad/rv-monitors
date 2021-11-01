//
// Created by Evald Nexhipi on 28.10.21.
//
#include "test_computation_time_generator.h"
#include "monitors/generators/computation_time_generator/computation_time_generator.h"
TEST(TestComputationTimeGenerator, tctg) {

    ComputationTimeGenerator ctg;
    ctg.rseed();
    std::vector< std::vector<std::shared_ptr<Expression>>*> allPreds;

    auto a1 = std::make_shared<Atomic>("a1");
    auto a2 = std::make_shared<Atomic>("a2");
    auto a3 = std::make_shared<Atomic>("a3");
    auto p1 = std::make_shared<Predicate>("p1", "na", std::vector<std::string>{"params1", "params2"});
    auto p2 = std::make_shared<Predicate>("p2", "na", std::vector<std::string>{"params1", "params2","params3"});

    ctg.AddAtomic(a1, 1.0);
    ctg.AddAtomic(a2, 1.0);
    ctg.AddAtomic(a3, 3.0);
    ctg.AddPredicate(p1, 3.0);
    ctg.AddPredicate(p2, 2.0);

    ctg.createPredicatesOnDelays({500,0,3,1000,2,9,750,3,18}, allPreds);

    EXPECT_TRUE(true);
}