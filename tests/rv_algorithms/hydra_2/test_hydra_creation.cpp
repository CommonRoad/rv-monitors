//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "test_hydra_creation.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra_2/hydra_creation_visitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/hydra_monitor.h"

TEST(TestHydraCreation, testAnd) {
    Formula formula = Parser::parse("a(test) and c(test)");
    formula.setId("R1");
    std::vector<std::tuple<std::string, size_t, double>> predicateCosts{{"a", 100, 10.0}, {"b", 10, 10}};
    HydraCreationVisitor hc{HydraCreationVisitor(predicateCosts)};
    std::shared_ptr<HydraMonitor> mon = hc.createMonitor(formula, 0, true);
    EXPECT_EQ(mon->to_string(), "a(test) and c(test)");
}

//TEST(TestHydraCreation, testAndReorder) {
//    Formula formula = Parser::parse("a(test) and c(test)");
//    formula.setId("R1");
//    std::vector<std::tuple<std::string, size_t, double>> predicateCosts{{"a", 10, 10.0}, {"b", 100, 10}};
//    HydraCreationVisitor hc{HydraCreationVisitor(predicateCosts)};
//    std::shared_ptr<HydraMonitor> mon = hc.createMonitor(formula, 0, true);
//    EXPECT_EQ(mon->to_string(), "c(test) and a(test)");
//}