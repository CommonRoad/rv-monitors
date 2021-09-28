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

TEST(TestHydra, testSinglePrevious) {
    Trace trace =
        Utils::createTrace({"a", "b", "c"}, {{"b", "c"}, {"a", "c"}, {}, {"a", "b", "c"}, {"b", "c"}, {"a", "b", "c"}},
                           std::vector<size_t>{0, 0, 1, 4, 4, 5});
    Formula formula = Parser::parse("P[0, 1](c)");
    std::vector<Verdict> expSolution{Verdict{0, false}, Verdict{0, true}, Verdict{1, true},
                                     Verdict{4, false}, Verdict{4, true}, Verdict{5, true}};
    TestHydra::evalHydra(trace, formula, expSolution);
}

TEST(TestHydra, testSingleNext) {
    Trace trace =
        Utils::createTrace({"a", "b", "c"}, {{"b", "c"}, {"a", "c"}, {}, {"a", "b", "c"}, {"b", "c"}, {"a", "b", "c"}},
                           std::vector<size_t>{0, 0, 1, 4, 4, 5});
    Formula formula = Parser::parse("X[0, 1](c)");

    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{0, false}, Verdict{1, false},
                                     Verdict{4, true}, Verdict{4, true},  Verdict{5, false}};
    TestHydra::evalHydra(trace, formula, expSolution);
}

TEST(TestHydra, testSingleSince) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}},
                                     std::vector<size_t>{0, 0, 2, 4, 5, 10});
    Formula formula = Parser::parse("(a)S[0, 4](b)");
    std::vector<Verdict> expSolution{Verdict{0, false}, Verdict{0, false}, Verdict{2, false},
                                     Verdict{4, true},  Verdict{5, true},  Verdict{10, true}};
    TestHydra::evalHydra(trace, formula, expSolution);
}

TEST(TestHydra, testSingleUntil) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}},
                                     std::vector<size_t>{0, 0, 2, 4, 5, 10});
    Formula formula = Parser::parse("(a)U[0, 4](b)");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{0, true},  Verdict{2, true},
                                     Verdict{4, true}, Verdict{5, false}, Verdict{10, true}};
    TestHydra::evalHydra(trace, formula, expSolution);
}

TEST(TestHydra, testSinceUntilOr) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}},
                                     std::vector<size_t>{0, 0, 2, 4, 5, 10});
    Formula formula = Parser::parse("(a)S[0, 4](b) or (a)U[0, 4](b)");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{0, true}, Verdict{2, true},
                                     Verdict{4, true}, Verdict{5, true}, Verdict{10, true}};
    TestHydra::evalHydra(trace, formula, expSolution);
}

TEST(TestHydra, testSinceUntilAnd) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}},
                                     std::vector<size_t>{0, 0, 2, 4, 5, 10});
    Formula formula = Parser::parse("(a)S[0, 4](b) and (a)U[0, 4](b)");
    std::vector<Verdict> expSolution{Verdict{0, false}, Verdict{0, false}, Verdict{2, false},
                                     Verdict{4, true},  Verdict{5, false}, Verdict{10, true}};
    TestHydra::evalHydra(trace, formula, expSolution);
}

TEST(TestHydra, testSinceUntilNegated1) {
    Trace trace =
        Utils::createTrace({"a", "b", "c"}, {{"b", "c"}, {"a", "c"}, {}, {"a", "b", "c"}, {"b", "c"}, {"a", "b", "c"}},
                           std::vector<size_t>{0, 0, 1, 4, 4, 5});
    Formula formula = Parser::parse("P[0, 4](!c)");
    std::vector<Verdict> expSolution{Verdict{0, false}, Verdict{0, false}, Verdict{1, false},
                                     Verdict{4, true},  Verdict{4, false}, Verdict{5, false}};
    TestHydra::evalHydra(trace, formula, expSolution);
}

TEST(TestMTL, testImplication) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"b"}, {"a"}, {"b"}, {"b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("a -> P(b)");
    std::vector<Verdict> expSolution{Verdict{0, false}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, true},  Verdict{5, true}, Verdict{6, true}};
    TestHydra::evalHydra(trace, formula, expSolution);
}

TEST(TestMTL, testFutureAsUntil) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("true U a");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, true}, Verdict{5, true}, Verdict{6, true}};
    TestHydra::evalHydra(trace, formula, expSolution);
}

TEST(TestMTL, testFuture) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("F(a)");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, true}, Verdict{5, true}, Verdict{6, true}};
    TestHydra::evalHydra(trace, formula, expSolution);
}

TEST(TestMTL, testGloballyAsUntil) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("!((true) U (!a))");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, true}, Verdict{5, true}, Verdict{6, true}};
    TestHydra::evalHydra(trace, formula, expSolution);
}

TEST(TestMTL, testGloballySatisfied) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a)");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, true}, Verdict{5, true}, Verdict{6, true}};
    TestHydra::evalHydra(trace, formula, expSolution);
}

TEST(TestMTL, testGloballyViolatedTwice) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"a"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a)");
    std::vector<Verdict> expSolution{Verdict{0, false}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, false}, Verdict{5, true}, Verdict{6, true}};
    TestHydra::evalHydra(trace, formula, expSolution);
}

TEST(TestMTL, testGloballyFutureOnceIntervalViolatedOnce) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"b"}, {"b"}, {"b"}, {"a"}, {"b"}, {"b"}, {"b"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b & O[0, 6](c)))");
    std::vector<Verdict> expSolution{Verdict{0, true},  Verdict{1, true}, Verdict{2, true},
                                     Verdict{3, false}, Verdict{4, true}, Verdict{5, true}};
    TestHydra::evalHydra(trace, formula, expSolution);
}

TEST(TestMTL, testGloballyFutureOnceIntervalSatisfied) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"b"}, {"a"}, {"b"}, {"c"}, {"b"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b & O[0, 6](c)))");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true},
                                     Verdict{3, true}, Verdict{4, true}, Verdict{5, true}};
    TestHydra::evalHydra(trace, formula, expSolution);
}

TEST(TestMTL, testGloballyFutureIntervalSatiesfied) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b))");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true},
                                     Verdict{3, true}, Verdict{4, true}, Verdict{5, true}};
    TestHydra::evalHydra(trace, formula, expSolution);
}

TEST(TestMTL, testGloballyFutureIntervalViolated) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b))");
    std::vector<Verdict> expSolution{Verdict{0, true},  Verdict{1, false}, Verdict{2, false},
                                     Verdict{3, false}, Verdict{4, false}, Verdict{5, false}};
    TestHydra::evalHydra(trace, formula, expSolution);
}

TEST(TestMTL, testGloballyOnceIntervalViolated) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> O[0, 4](b))");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, false}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, true}};
    TestHydra::evalHydra(trace, formula, expSolution);
}

TEST(TestMTL, testGloballyOnceIntervalSatisfied) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"b"}, {"c"}, {"a"}, {"b"}, {"b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(a -> O[0, 6](b))");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, true}};
    TestHydra::evalHydra(trace, formula, expSolution);
}

TEST(TestMTL, testOnceIntervalSatisfied) {
    Trace trace = Utils::createTrace({"a"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("O[0, 6](a)");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true},
                                     Verdict{3, true}, Verdict{4, true}, Verdict{5, true}};
    TestHydra::evalHydra(trace, formula, expSolution);
}

TEST(TestMTL, testOnceIntervalViolated) {
    Trace trace = Utils::createTrace({"a"}, {{}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("O[0, 6](a)");
    std::vector<Verdict> expSolution{Verdict{0, false}, Verdict{1, true}, Verdict{2, true},
                                     Verdict{3, true},  Verdict{4, true}, Verdict{5, true}};
    TestHydra::evalHydra(trace, formula, expSolution);
}

TEST(TestMTL, testGloballyPreviousViolated) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> P(b))");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, false}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, false}};
    TestHydra::evalHydra(trace, formula, expSolution);
}

// TEST(TestMTL, testGloballyOncePreviousViolated) {
//    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"b"}, {"b"}, {"b"}, {"c"}, {"a"}});
//    Formula formula = Parser::parse("G(a -> O(b & P(!b)))");
//    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true},
//                                     Verdict{3, true}, Verdict{4, false}};
//    TestHydra::evalHydra(trace, formula, expSolution);
//}

// TEST(TestMTL, satisfied7) {
//    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"a", "b"}, {"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
//    Formula formula = Parser::parse("G(a -> O(b & P(!b)))");
//    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
//                                     Verdict{4, true}, Verdict{5, true}, Verdict{6, true}, Verdict{7, true}};
//    TestHydra::evalHydra(trace, formula, expSolution);
//}

// TEST(TestMTL, violated8) {
//    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"b"}, {"a"}, {"c"}});
//    Formula formula = Parser::parse("G((a & O(b & P(!b))) -> c)");
//    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true},
//                                     Verdict{3, true}, Verdict{4, true}};
//    TestHydra::evalHydra(trace, formula, expSolution);
//}
//
// TEST(TestMTL, satisfied8) {
//    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"b"}, {"c", "a"}, {"c"}});
//    Formula formula = Parser::parse("G((a & O(b & P(!b))) -> c)");
//    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true},
//                                     Verdict{3, true}, Verdict{4, true}};
//    TestHydra::evalHydra(trace, formula, expSolution);
//}
//
// TEST(TestMTL, satisfied9) {
//    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a"}});
//    Formula formula = Parser::parse("G((a & O(b & P(!b))) -> c)");
//    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true}};
//    TestHydra::evalHydra(trace, formula, expSolution);
//}

// TEST(TestMTL, satisfied10) {
//    Trace trace = Utils::createTrace(
//        {"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a"}, {"a"}, {"c"}, {"c"}});
//    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,5](H[0,2](a & b))))");
//    auto monitor{MTLEventConsuming(formula)};
//    EXPECT_TRUE(monitor.evaluate(trace).at(0).b);
//}
//
// TEST(TestMTL, violated10) {
//    Trace trace =
//        Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a", "b"}, {"a"}, {"a"}, {"c"}, {"c"}});
//    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,5](H[0,2](a & b))))");
//    auto monitor{MTLEventConsuming(formula)};
//    EXPECT_FALSE(monitor.evaluate(trace).at(0).b);
//}
//
// TEST(TestMTL, violated11) {
//    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a"}, {"a"}, {"a"}, {"c"}, {"c"}});
//    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,3](a & b)))");
//    auto monitor{MTLEventConsuming(formula)};
//    EXPECT_FALSE(monitor.evaluate(trace).at(0).b);
//}
//
// TEST(TestMTL, satisfied11) {
//    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a"}, {"c"}, {"c"}});
//    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,3](a & b)))");
//    auto monitor{MTLEventConsuming(formula)};
//    EXPECT_TRUE(monitor.evaluate(trace).at(0).b);
//}
//
// TEST(TestMTL, satisfied12) {
//    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b"}, {"a", "b", "c"}, {"a"}, {"a"},
//    {"b"}}); Formula formula = Parser::parse("G(c -> H[0,1](a & b))"); auto monitor{MTLEventConsuming(formula)};
//    EXPECT_TRUE(monitor.evaluate(trace).at(0).b);
//}
//
// TEST(TestMTL, violated12) {
//    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}});
//    Formula formula = Parser::parse("G(c -> H[0,1](a & b))");
//    auto monitor{MTLEventConsuming(formula)};
//    EXPECT_FALSE(monitor.evaluate(trace).at(0).b);
//}