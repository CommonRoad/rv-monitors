//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//

#include "test_hydra.h"
#include "monitors/parser/parser.h"

void TestHydra::executeHydraEval(Trace &trace, const std::vector<Formula> &formulas,
                                 const std::vector<std::vector<Verdict>> &expSolution) {
    auto monitor1{Hydra(formulas, false, false, false)};
    evalHydra(formulas, expSolution, monitor1, trace, "fff");
    auto monitor2{Hydra(formulas, false, false, true)};
    evalHydra(formulas, expSolution, monitor2, trace, "fft");
    auto monitor3{Hydra(formulas, false, true, false)};
    evalHydra(formulas, expSolution, monitor3, trace, "ftf");
    auto monitor4{Hydra(formulas, false, true, true)};
    evalHydra(formulas, expSolution, monitor4, trace, "ftt");
    auto monitor5{Hydra(formulas, true, false, false)};
    evalHydra(formulas, expSolution, monitor5, trace, "tff");
    auto monitor6{Hydra(formulas, true, false, true)};
    evalHydra(formulas, expSolution, monitor6, trace, "tft");
    auto monitor7{Hydra(formulas, true, true, false)};
    evalHydra(formulas, expSolution, monitor7, trace, "ttf");
    auto monitor9{Hydra(formulas, true, true, true)};
    evalHydra(formulas, expSolution, monitor9, trace, "ttt");
}

void TestHydra::evalHydra(const std::vector<Formula> &formulas, const std::vector<std::vector<Verdict>> &expSolution,
                          Hydra &monitor, Trace &trace, const std::string &evaluation) {
    auto solution{monitor.evaluate(trace)};
    for (int i{0}; i < formulas.size(); ++i) {
        if (solution.at(formulas[i].getId()).size() != expSolution[i].size())
            throw std::logic_error("Hydra Test /" + evaluation +
                                   ": length of expected solution and solution does not match");
        for (size_t j{0}; j < expSolution[i].size(); ++j)
            EXPECT_TRUE(expSolution[i][j] == solution.at(formulas[i].getId())[j])
                << evaluation + ": Mismatch at element index: " << j;
    }
}

TEST(TestHydra, testSinglePrevious) {
    Trace trace =
        Utils::createTrace({"a", "b", "c"}, {{"b", "c"}, {"a", "c"}, {}, {"a", "b", "c"}, {"b", "c"}, {"a", "b", "c"}},
                           std::vector<size_t>{0, 0, 1, 4, 4, 5});
    Formula formula = Parser::parse("P[0, 1](c)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, false}, Verdict{0, true}, Verdict{1, true},
                                     Verdict{4, false}, Verdict{4, true}, Verdict{5, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testSingleNext) {
    Trace trace =
        Utils::createTrace({"a", "b", "c"}, {{"b", "c"}, {"a", "c"}, {}, {"a", "b", "c"}, {"b", "c"}, {"a", "b", "c"}},
                           std::vector<size_t>{0, 0, 1, 4, 4, 5});
    Formula formula = Parser::parse("X[0, 1](c)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{0, false}, Verdict{1, false},
                                     Verdict{4, true}, Verdict{4, true},  Verdict{5, false}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testSingleSince) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}},
                                     std::vector<size_t>{0, 0, 2, 4, 5, 10});
    Formula formula = Parser::parse("(a)S[0, 4](b)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, false}, Verdict{0, false}, Verdict{2, false},
                                     Verdict{4, true},  Verdict{5, true},  Verdict{10, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testSingleUntil1) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}},
                                     std::vector<size_t>{0, 0, 2, 4, 5, 10});
    Formula formula = Parser::parse("(a)U[0, 4](b)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{0, true},  Verdict{2, true},
                                     Verdict{4, true}, Verdict{5, false}, Verdict{10, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testSingleUntil2) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}},
                                     std::vector<size_t>{0, 1, 2, 3, 4, 5});
    Formula formula = Parser::parse("(a)U[0, 4](b)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true},
                                     Verdict{3, true}, Verdict{4, true}, Verdict{5, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testSingleUntil3) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"b"}, {}, {"a", "b"}, {"a"}, {"b"}},
                                     std::vector<size_t>{0, 1, 2, 3, 4, 5});
    Formula formula = Parser::parse("(a)U[0, 4](b)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, false},
                                     Verdict{3, true}, Verdict{4, true}, Verdict{5, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testSingleUntil4) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {}, {"b"}},
                                     std::vector<size_t>{0, 1, 2, 3, 4, 5, 6});
    Formula formula = Parser::parse("(a)U[0, 4](b)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true},  Verdict{1, true},  Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, false}, Verdict{5, false}, Verdict{6, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testSingleUntilAnd) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}},
                                     std::vector<size_t>{0, 0, 2, 4, 5, 10});
    Formula formula = Parser::parse("((a)U[0, 4](b)) and a");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{0, true},  Verdict{2, true},
                                     Verdict{4, true}, Verdict{5, false}, Verdict{10, false}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testSinceUntilOr) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}},
                                     std::vector<size_t>{0, 0, 2, 4, 5, 10});
    Formula formula = Parser::parse("(a)S[0, 4](b) or (a)U[0, 4](b)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{0, true}, Verdict{2, true},
                                     Verdict{4, true}, Verdict{5, true}, Verdict{10, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testSinceUntilAnd) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}},
                                     std::vector<size_t>{0, 0, 2, 4, 5, 10});
    Formula formula = Parser::parse("(a)S[0, 4](b) and (a)U[0, 4](b)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, false}, Verdict{0, false}, Verdict{2, false},
                                     Verdict{4, true},  Verdict{5, false}, Verdict{10, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testSinceUntilNegated1) {
    Trace trace =
        Utils::createTrace({"a", "b", "c"}, {{"b", "c"}, {"a", "c"}, {}, {"a", "b", "c"}, {"b", "c"}, {"a", "b", "c"}},
                           std::vector<size_t>{0, 0, 1, 4, 4, 5});
    Formula formula = Parser::parse("P[0, 4](!c)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, false}, Verdict{0, false}, Verdict{1, false},
                                     Verdict{4, true},  Verdict{4, false}, Verdict{5, false}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testImplication) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"b"}, {"a"}, {"b"}, {"b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("a -> P(b)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, false}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, true},  Verdict{5, true}, Verdict{6, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testFutureAsUntil) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("true U a");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, true}, Verdict{5, true}, Verdict{6, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testFuture) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("F(a)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, true}, Verdict{5, true}, Verdict{6, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testGloballyAsUntil) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("!((true) U (!a))");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, true}, Verdict{5, true}, Verdict{6, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testGloballySatisfied) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, true}, Verdict{5, true}, Verdict{6, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testGloballyViolatedTwice) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"a"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, false}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, false}, Verdict{5, true}, Verdict{6, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testGloballyFutureOnceIntervalViolatedOnce) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"b"}, {"b"}, {"b"}, {"a"}, {"b"}, {"b"}, {"b"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b & O[0, 6](c)))");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, false},
                                     Verdict{4, true}, Verdict{5, true}, Verdict{6, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testGloballyFutureOnceIntervalSatisfied) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"b"}, {"a"}, {"b"}, {"c"}, {"b"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b & O[0, 6](c)))");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, true}, Verdict{5, true}, Verdict{6, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testGloballyFutureIntervalSatiesfied) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b))");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, true}, Verdict{5, true}, Verdict{6, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testGloballyFutureIntervalViolated) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> F[0, 6](b))");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true},  Verdict{1, false}, Verdict{2, false}, Verdict{3, false},
                                     Verdict{4, false}, Verdict{5, false}, Verdict{6, false}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testGloballyOnceIntervalViolated) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> O[0, 4](b))");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, false}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testUntil_1) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"},{"c"},{"a","b"},{"a","b"},{"c"}});
    Formula formula = Parser::parse("(a & b) U[0,2](c)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testUntil_2) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a","b"},{"c"},{"c"},{"a","b"},{"a","b"},{"c"}});
    Formula formula = Parser::parse("(a & b) U[0,2](c)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, true}, Verdict{5, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testGloballyOnceIntervalSatisfied) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"b"}, {"c"}, {"a"}, {"b"}, {"b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(a -> O[0, 6](b))");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, true}, Verdict{5, true}, Verdict{6, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testOnceIntervalSatisfied) {
    Trace trace = Utils::createTrace({"a"}, {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("O[0, 6](a)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, true}, Verdict{5, true}, Verdict{6, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testOnceIntervalViolated) {
    Trace trace = Utils::createTrace({"a"}, {{}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("O[0, 6](a)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, false}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, true},  Verdict{5, true}, Verdict{6, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testGloballyPreviousViolated) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> P(b))");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, false}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, false}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testOnceIntervallSatisfied) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("O[0,2](a)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, true}, Verdict{5, true}, Verdict{6, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testOnceIntervallViolated) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"b"}, {"b"}, {"b", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("O[0,2](a)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, false}, Verdict{1, false}, Verdict{2, false}, Verdict{3, true},
                                     Verdict{4, true},  Verdict{5, true},  Verdict{6, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testHistoricallyAsSinceViolated) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("!(true S(!a))");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, true}, Verdict{5, true}, Verdict{6, false}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testHistoricallySatisfied) {
    Trace trace =
        Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"a", "b"}});
    Formula formula = Parser::parse("H(a)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, true}, Verdict{5, true}, Verdict{6, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testHistoricallyViolated) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("H(a)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, true}, Verdict{5, true}, Verdict{6, false}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testGloballyOncePreviousViolated) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"b"}, {"b"}, {"b"}, {"c"}, {"a"}});
    Formula formula = Parser::parse("G(a -> O(b & P(!b)))");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, false}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testPreviousAndNegation) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"a", "b"}, {"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("b & P(!b)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, false}, Verdict{1, true}, Verdict{2, false}, Verdict{3, false},
                                     Verdict{4, false}, Verdict{5, true}, Verdict{6, false}, Verdict{7, false}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testGloballyOncePrevious) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"a", "b"}, {"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G(a -> O(b & P(!b)))");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, true}, Verdict{5, true}, Verdict{6, true}, Verdict{7, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testGloballyOncePrevious2) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"a", "b"}, {"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("true S (P(!(b)) & b)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, false}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, true},  Verdict{5, true}, Verdict{6, true}, Verdict{7, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testPreviousAnd) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"a", "b"}, {"c"}, {"a"}, {"b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("P(!(b)) & b");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, false}, Verdict{1, true}, Verdict{2, false}, Verdict{3, false},
                                     Verdict{4, false}, Verdict{5, true}, Verdict{6, false}, Verdict{7, false}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testGloballyAndOncePreviousImplicationViolatedOnce) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"b"}, {"a"}, {"c"}});
    Formula formula = Parser::parse("G((a & O(b & P(!b))) -> c)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, false},
                                     Verdict{4, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testGloballyAndOncePreviousImplicationSatisfied1) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"b"}, {"b"}, {"c", "a"}, {"c"}});
    Formula formula = Parser::parse("G((a & O(b & P(!b))) -> c)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testGloballyAndOncePreviousImplicationSatisfied2) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("G((a & O(b & P(!b))) -> c)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testGloballyImplicationOnceHistoricallySatisfied) {
    Trace trace = Utils::createTrace(
        {"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,5](H[0,2](a & b))))");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, true}, Verdict{5, true}, Verdict{6, true}, Verdict{7, true},
                                     Verdict{8, true}, Verdict{9, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testGloballyImplicationOnceHistoricallyViolatedOnce) {
    Trace trace =
        Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a", "b"}, {"a"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,5](H[0,2](a & b))))");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true},  Verdict{3, true},
                                     Verdict{4, true}, Verdict{5, true}, Verdict{6, false}, Verdict{7, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testGloballyImplicationOncePreviousViolatedOnce) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a"}, {"a"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,3](a & b)))");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true},  Verdict{3, true},
                                     Verdict{4, true}, Verdict{5, true}, Verdict{6, false}, Verdict{7, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testOnceIntervalAnd) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a"}, {"a"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("O[1,3](a & b)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, false}, Verdict{1, false}, Verdict{2, false}, Verdict{3, true},
                                     Verdict{4, true},  Verdict{5, true},  Verdict{6, false}, Verdict{7, false}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testNegationAndPrevious) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a"}, {"a"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("!a & P(a)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, false}, Verdict{1, false}, Verdict{2, false}, Verdict{3, false},
                                     Verdict{4, false}, Verdict{5, false}, Verdict{6, true},  Verdict{7, false}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testGloballyImplicationOncePreviousSatisfied) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"}, {"a"}, {"a", "b"}, {"a"}, {"c"}, {"c"}});
    Formula formula = Parser::parse("G((!a & P(a)) -> (O[1,3](a & b)))");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true},
                                     Verdict{3, true}, Verdict{4, true}, Verdict{5, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testGloballyImplicationHistoricallySatisfied) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(c -> H[0,1](a & b))");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, true}, Verdict{5, true}, Verdict{6, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testGloballyImplicationHistoricallyViolatedOnce) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a"}, {"a"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(c -> H[0,1](a & b))");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, false},
                                     Verdict{3, true}, Verdict{4, true}, Verdict{5, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testGloballyNext) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"b"}, {"a"}, {"b"}});
    Formula formula = Parser::parse("G(b -> (X(a)))");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, false}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testCombinedSinceUntilFormulas) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}},
                                     std::vector<size_t>{0, 0, 2, 4, 5, 10});
    std::vector<Formula> formulas = {Parser::parse("(a)S[0, 4](b)"), Parser::parse("(a)U[0, 4](b)"),
                                     Parser::parse("((a)U[0, 4](b)) and a"),
                                     Parser::parse("(a)S[0, 4](b) or (a)U[0, 4](b)")};
    formulas[0].setId("R1");
    formulas[1].setId("R2");
    formulas[2].setId("R3");
    formulas[3].setId("R4");
    std::vector<std::vector<Verdict>> expSolutions{
        {Verdict{0, false}, Verdict{0, false}, Verdict{2, false}, Verdict{4, true}, Verdict{5, true},
         Verdict{10, true}},
        {Verdict{0, true}, Verdict{0, true}, Verdict{2, true}, Verdict{4, true}, Verdict{5, false}, Verdict{10, true}},
        {Verdict{0, true}, Verdict{0, true}, Verdict{2, true}, Verdict{4, true}, Verdict{5, false}, Verdict{10, false}},
        {Verdict{0, true}, Verdict{0, true}, Verdict{2, true}, Verdict{4, true}, Verdict{5, true}, Verdict{10, true}}};
    TestHydra::executeHydraEval(trace, formulas, expSolutions);
}

TEST(TestHydra, testAndNegatedOnceInterval) {
    Trace trace = Utils::createTrace({"a", "b"}, {{}, {}, {"a", "b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("a & !O[0,30](b)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, false}, Verdict{1, false}, Verdict{2, false}, Verdict{3, false},
                                     Verdict{4, false}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testAndOnceInterval) {
    Trace trace = Utils::createTrace({"a", "b"}, {{}, {}, {"a", "b"}, {"a"}, {"a"}});
    Formula formula = Parser::parse("a & O[0,30](!b)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, false}, Verdict{1, false}, Verdict{2, true}, Verdict{3, true},
                                     Verdict{4, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, testAndOnceIntervallAnd) {
    Trace trace = Utils::createTrace(
        {"in_same_lane", "in_single_lane"},
        {{"in_single_lane"}, {"in_single_lane"}, {"in_same_lane"}, {"in_same_lane"}, {"in_same_lane"}});
    Formula formula = Parser::parse("in_same_lane & !O[0,30](in_same_lane & !(in_single_lane) & P(in_single_lane))");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, false}, Verdict{1, false}, Verdict{2, false}, Verdict{3, false},
                                     Verdict{4, false}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}


TEST(TestHydra, violated_single_until) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a","b"}, {"a","b"}, {"a","b"}, {"a","b"}, {"a"}, {"c"},{"a"},{"a"}}, std::vector<size_t>{0, 1, 2, 3, 4, 5, 6, 7});
    Formula formula = Parser::parse("(a & b) U[0, 5] (c)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true},
                                     Verdict{3, true}, Verdict{4, true}, Verdict{5, true},
                                     Verdict{6, true}, Verdict{7, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

/**** UNTIL ***/
TEST(TestHydra, satisfied_single_until_3) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"a","b"},{"c"},{"c"},{"a","b"},{"a","b"},{"c"}}, std::vector<size_t>{0, 1, 2, 3, 4, 5});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true},
                                     Verdict{3, true}, Verdict{4, true}, Verdict{5, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, violated_single_until_4) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"},{"a"},{"a"},{"b"}}, std::vector<size_t>{0, 1, 2, 3});
    Formula formula = Parser::parse("a U[0, 2] b");
    formula.setId("R1");
    std::vector<Verdict> expSolution{Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true}};
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, satisfied_single_until_long) {
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"a","b"},{"a","b"},{"a","b"},{"a","b"},{"c"},{"c"},{"c"},{"c"},{"c"},{"c"},{"c"}},
                                     std::vector<size_t>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    Formula formula = Parser::parse("(a & b) U[1, 4] c");
    formula.setId("R1");
    std::cout<<trace.to_string()<<std::endl;
    std::vector<Verdict> expSolution{
        Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
        Verdict{4, true}, Verdict{5, true}, Verdict{6, true}, Verdict{7, true},
        Verdict{8, true}, Verdict{9, true}, Verdict{10, true}
    };
    TestHydra::executeHydraEval(trace, {formula}, {expSolution}); // WHY ISN'T THIS CORRECT?
}

TEST(TestHydra, unsat_special_single_until_long_2) {
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"a","b"},{"a","b"},{"a","b"},{"a","b","c"},{"a","b"},{"a","b"},{"a","b","c"}},
                                     std::vector<size_t>{0, 1, 2, 3, 4, 5,6});
    Formula formula = Parser::parse("(a & b) U[1, 3] c");
    formula.setId("R1");
    std::cout<<trace.to_string()<<std::endl;
    std::vector<Verdict> expSolution{
        Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
        Verdict{4, true}, Verdict{5, true}, Verdict{6, true}
    };
    TestHydra::executeHydraEval(trace, {formula}, {expSolution}); // WHY ISN'T THIS CORRECT?
}

TEST(TestHydra, unsat_special_single_until_long_3) {
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"a","b"},{"a","b"},{"a","b"},{"a","b","c"},{"c"},{"c"},{"c"}},
                                     std::vector<size_t>{0, 1, 2, 3, 4, 5,6});
    Formula formula = Parser::parse("(a & b) U[1, 3] c");
    formula.setId("R1");
    std::cout<<trace.to_string()<<std::endl;
    std::vector<Verdict> expSolution{
        Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
        Verdict{4, true}, Verdict{5, true}, Verdict{6, true}
    };
    TestHydra::executeHydraEval(trace, {formula}, {expSolution}); // WHY ISN'T THIS CORRECT?
}

TEST(TestHydra, unsat_special_single_until_long_4) {
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"a","b"},{"a","b"},{"a","b"},{"a","b","c"},{"a","b","c"},{"a","b","c"},{"a","b","c"}},
                                     std::vector<size_t>{0, 1, 2, 3, 4, 5,6});
    Formula formula = Parser::parse("(a & b) U[1, 3] c");
    formula.setId("R1");
    std::cout<<trace.to_string()<<std::endl;
    std::vector<Verdict> expSolution{
        Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
        Verdict{4, true}, Verdict{5, true}, Verdict{6, true}
    };
    TestHydra::executeHydraEval(trace, {formula}, {expSolution}); // WHY ISN'T THIS CORRECT?
}

TEST(TestHydra, sat_special_single_until_extra_1) {
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"c"},{"c"},{"a","b"},{"a","b"},{"c"}},
                                     std::vector<size_t>{0, 1, 2, 3, 4});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    formula.setId("R1");
    std::cout<<trace.to_string()<<std::endl;
    std::vector<Verdict> expSolution{
        Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
        Verdict{4, true}
    };
    TestHydra::executeHydraEval(trace, {formula}, {expSolution}); // WHY ISN'T THIS CORRECT?
}

TEST(TestHydra, unsat_special_single_until_extra_) {
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"c"},{"c"},{"a","b"},{"a","b"},{"a","b"}},
                                     std::vector<size_t>{0, 1, 2, 3, 4});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    formula.setId("R1");
    std::cout<<trace.to_string()<<std::endl;
    std::vector<Verdict> expSolution{
        Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
        Verdict{4, true}
    };
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, unsat_special_single_until_extra_2) {
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"c"},{"c"},{"a","b"},{"c"},{"a","b"}},
                                     std::vector<size_t>{0, 1, 2, 3, 4});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    formula.setId("R1");
    std::cout<<trace.to_string()<<std::endl;
    std::vector<Verdict> expSolution{
        Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
        Verdict{4, true}
    };
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, unsat_single_until_fin_1) {
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"c"},{"c"},{"a","b"},{"a","b"},{"a","b"}},
                                     std::vector<size_t>{0, 1, 2, 3, 4});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    formula.setId("R1");
    std::cout<<trace.to_string()<<std::endl;
    std::vector<Verdict> expSolution{
        Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
        Verdict{4, true}
    };
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, sat_single_until_fin_1) {
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"c"},{"c"},{"a","b"},{"a","b"},{"c"}},
                                     std::vector<size_t>{0, 1, 2, 3, 4});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    formula.setId("R1");
    std::cout<<trace.to_string()<<std::endl;
    std::vector<Verdict> expSolution{
        Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
        Verdict{4, true}
    };
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, sat_single_until_fin_2) {
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"c"},{"c"},{"a","b"},{"a","b"},{"c"},{"a","b"},{"a","b"},{"a","b","c"}},
                                     std::vector<size_t>{0, 1, 2, 3, 4, 5, 6, 7});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    formula.setId("R1");
    std::cout<<trace.to_string()<<std::endl;
    std::vector<Verdict> expSolution{
        Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
        Verdict{4, true}, Verdict{5, true}, Verdict{6, true}, Verdict{7, true}
    };
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, sat_single_until_fin_3) {
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"a","b"},{"a","b"},{"c"},{"a","b"},{"a","b"},{"c"}},
                                     std::vector<size_t>{0, 1, 2, 3, 4, 5});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    formula.setId("R1");
    std::cout<<trace.to_string()<<std::endl;
    std::vector<Verdict> expSolution{
        Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
        Verdict{4, true}, Verdict{5, true}
    };
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, unsat_single_until_fin_2) {
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"a","b"},{"a","b"},{"c"},{"a","b"},{"a","b"},{"a","b"}},
                                     std::vector<size_t>{0, 1, 2, 3, 4, 5});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    formula.setId("R1");
    std::cout<<trace.to_string()<<std::endl;
    std::vector<Verdict> expSolution{
        Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
        Verdict{4, true}, Verdict{5, true}
    };
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, unsat_single_until_fin_3) {
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"a","b"},{"a","b"},{"c"},{"a","b"},{"c"}},
                                     std::vector<size_t>{0, 1, 2, 3, 4});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    formula.setId("R1");
    std::cout<<trace.to_string()<<std::endl;
    std::vector<Verdict> expSolution{
        Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
        Verdict{4, true}
    };
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, unsat_single_until_fin_4) {
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"a","b"},{"c"},{"a","b"},{"a","b"},{"c"}},
                                     std::vector<size_t>{0, 1, 2, 3, 4});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    formula.setId("R1");
    std::cout<<trace.to_string()<<std::endl;
    std::vector<Verdict> expSolution{
        Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
        Verdict{4, true}
    };
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestHydra, unsat_single_until_fin_5) {
    Trace trace = Utils::createTrace({"a", "b","c"}, {{"a","b"},{"c"},{"c"},{"a","b"},{"a","b"},{"c"}},
                                     std::vector<size_t>{0, 1, 2, 3, 4, 5});
    Formula formula = Parser::parse("(a & b) U[0, 2] c");
    formula.setId("R1");
    std::cout<<trace.to_string()<<std::endl;
    std::vector<Verdict> expSolution{
        Verdict{0, true}, Verdict{1, true}, Verdict{2, true}, Verdict{3, true},
        Verdict{4, true}, Verdict{5, true}
    };
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

/*** SINCE ***/
TEST(TestHydra, violated_single_since_1) {
    Trace trace = Utils::createTrace({"a", "b", "c"}, {{"c"},{"c"},{"c"},{"a","b"},{"a","b"},{"a","b"}}, std::vector<size_t>{0, 1, 2, 3, 4, 5});
    Formula formula = Parser::parse("(a & b) S[0, 2] c");
    formula.setId("R1");
    std::vector<Verdict> expSolution{
        Verdict{0, true}, Verdict{1, true}, Verdict{2, true},
        Verdict{3, true}, Verdict{4, true}, Verdict{5, true}
    };
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

/*** NEXT STRONG ***/
TEST(TestHydra, satisfied_next_strong_1) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"},{"a","b"},{"a","b"},{"a","b"},{"a","b"}}, std::vector<size_t>{0, 1, 2, 3, 4});
    Formula formula = Parser::parse("X[0,1](a & b)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{
        Verdict{0, true}, Verdict{1, true}, Verdict{2, true},
        Verdict{3, true}, Verdict{4, true}
    };
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

/*** PREVIOUSLY ***/
TEST(TestHydra, satisfied_previously_1) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a","b"}, {"a","b"}, {"a","b"}, {"a", "b"},{"a","b"}}, std::vector<size_t>{0, 1, 2, 3, 4});
    Formula formula = Parser::parse("P[0,1](a & b)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{
        Verdict{0, true}, Verdict{1, true}, Verdict{2, true},
        Verdict{3, true}, Verdict{4, true}
    };
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

/*** GLOBALLY ***/
TEST(TestHydra, satisfied_globally_1) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a","b"}, {"a","b"}, {"a","b"}, {"a", "b"},{"b"}}, std::vector<size_t>{0, 1, 2, 3, 4});
    Formula formula = Parser::parse("G[0,2](a & b)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{
        Verdict{0, true}, Verdict{1, true}, Verdict{2, true},
        Verdict{3, true}, Verdict{4, true}
    };
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

/*** FUTURE ***/
TEST(TestHydra, satisfied_future_1) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"a","b"}, {"a","b"}, {"a", "b"},{"a","b"}}, std::vector<size_t>{0, 1, 2, 3, 4});
    Formula formula = Parser::parse("F[1,3](a & b)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{
        Verdict{0, true}, Verdict{1, true}, Verdict{2, true},
        Verdict{3, true}, Verdict{4, true }
    };
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}


/*** HISTORICALLY ***/
TEST(TestHydra, violated_historically_1) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a","b"}, {"a","b"}, {}, {"a"},{"b"}}, std::vector<size_t>{0, 1, 2, 3, 4});
    Formula formula = Parser::parse("H[0,2](a & b)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{
        Verdict{0, true}, Verdict{1, true}, Verdict{2, true},
        Verdict{3, true}, Verdict{4, true}
    };
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}

/*** ONCE ****/
TEST(TestHydra, violated_once_1) {
    Trace trace = Utils::createTrace({"a", "b"}, {{"a"}, {"b"}, {"b"}, {"a"}, {"a"}, {"a"}, {"a"}}, std::vector<size_t>{0, 1, 2, 3, 4, 5, 6});
    Formula formula = Parser::parse("O[1,2](a)");
    formula.setId("R1");
    std::vector<Verdict> expSolution{
        Verdict{0, true}, Verdict{1, true}, Verdict{2, true},
        Verdict{3, true}, Verdict{4, true}, Verdict{5, true},
        Verdict{6, true}
    };
    TestHydra::executeHydraEval(trace, {formula}, {expSolution});
}