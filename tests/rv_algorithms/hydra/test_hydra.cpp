//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//

#include "test_hydra.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra/hydra.h"

void TestHydra::evalHydra(Trace &trace, const Formula &formula, const std::vector<Verdict> &expSolution) {
    auto monitor{Hydra(formula)};
    auto solution{monitor.evaluate(trace)};
    for (size_t i{0}; i < expSolution.size(); ++i)
        EXPECT_TRUE(expSolution[i] == solution[i]) << "Mismatch at time step: " << i;
}

TEST(TestHydra, testPreviousSingle) {
    Trace trace =
        Utils::createTrace({"a", "b", "c"}, {{"b", "c"}, {"a", "c"}, {}, {"a", "b", "c"}, {"b", "c"}, {"a", "b", "c"}},
                           std::vector<size_t>{0, 0, 1, 4, 4, 5});
    Formula formula = Parser::parse("P[0, 1](c)");
    std::vector<Verdict> expSolution{Verdict{0, false}, Verdict{0, true}, Verdict{1, true},
                                     Verdict{4, false}, Verdict{4, true}, Verdict{5, true}};
    TestHydra::evalHydra(trace, formula, expSolution);
}

TEST(TestHydra, testNextSingle) {
    Trace trace =
        Utils::createTrace({"a", "b", "c"}, {{"b", "c"}, {"a", "c"}, {}, {"a", "b", "c"}, {"b", "c"}, {"a", "b", "c"}},
                           std::vector<size_t>{0, 0, 1, 4, 4, 5});
    Formula formula = Parser::parse("X[0, 1](c)");

    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{0, false}, Verdict{1, false},
                                     Verdict{4, true}, Verdict{4, true},  Verdict{5, false}};
    TestHydra::evalHydra(trace, formula, expSolution);
}

TEST(TestHydra, testSinceSingle) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}},
                                     std::vector<size_t>{0, 0, 2, 4, 5, 10});
    Formula formula = Parser::parse("(a)S[0, 4](b)");
    std::vector<Verdict> expSolution{Verdict{0, false}, Verdict{0, false}, Verdict{2, false},
                                     Verdict{4, true},  Verdict{5, true},  Verdict{10, true}};
    TestHydra::evalHydra(trace, formula, expSolution);
}
