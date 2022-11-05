//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2022 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//

#include "test_database.h"
#include <cstring>
#include <string>
#include <utility>

TestSample::TestSample(const std::string &testName, const std::string &formulaStr, std::vector<Verdict> expectedResult,
                       const std::vector<std::vector<std::string>> &trueAtomics, const std::vector<size_t> &timeSteps,
                       DomainCollection domainCollection, FunctionCollection functionCollection)
    : testName(testName), formula(std::make_shared<Formula>(Parser::parse(formulaStr, testName))),
      trace(Utils::createTrace(trueAtomics, timeSteps, std::make_shared<Formula>(Parser::parse(formulaStr, testName)))),
      expectedResult(std::move(expectedResult)), domainCollection(std::move(domainCollection)),
      functionCollection(std::move(functionCollection)) {}

TestSample::TestSample(const std::string &testName, const std::string &formulaStr, std::vector<Verdict> expectedResult,
                       const std::set<std::string> &atomics, const std::vector<std::vector<std::string>> &trueAtomics,
                       const std::vector<size_t> &timeSteps, DomainCollection domainCollection,
                       FunctionCollection functionCollection)
    : testName(testName), formula(std::make_shared<Formula>(Parser::parse(formulaStr, testName))),
      trace(Utils::createTrace(atomics, trueAtomics, timeSteps)), expectedResult(std::move(expectedResult)),
      domainCollection(std::move(domainCollection)), functionCollection(std::move(functionCollection)) {}

TestDataBase::TestDataBase() {
    testCases = {
        {"F1_1", "G(P[0, 1](c))", createExpectedResult(), {}},
        {
            "F1_2",
            "G(P[0, 1](c))",
            createExpectedResult({{0, false}, {0, true}, {1, true}, {4, false}, {4, true}, {5, true}}),
            {{"b", "c"}, {"a", "c"}, {}, {"a", "b", "c"}, {"b", "c"}, {"a", "b", "c"}},
            {0, 0, 1, 4, 4, 5},
        },
        {
            "F2_1",
            "G(X[0, 1](c))",
            createExpectedResult({{0, true}, {0, false}, {1, false}, {4, true}, {4, true}, {5, false}}),
            {{"b", "c"}, {"a", "c"}, {}, {"a", "b", "c"}, {"b", "c"}, {"a", "b", "c"}},
            {0, 0, 1, 4, 4, 5},
        },
        {
            "F3_1",
            "G((a)S[0, 4](b))",
            createExpectedResult({{0, false}, {0, false}, {2, false}, {4, true}, {5, true}, {10, true}}),
            {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}},
            {0, 0, 2, 4, 5, 10},
        },
        {
            "F4_1",
            "G((a)S[1, inf](b))",
            createExpectedResult(
                {{0, false}, {0, false}, {0, false},  {0, false},  {4, false}, {4, false},  {4, false},  {4, false},
                 {8, true},  {8, true},  {8, true},   {8, true},   {9, true},  {9, true},   {9, true},   {9, true},
                 {10, true}, {10, true}, {10, true},  {10, true},  {13, true}, {13, true},  {13, false}, {13, false},
                 {15, true}, {15, true}, {15, false}, {15, false}, {16, true}, {16, false}, {16, false}, {16, false}}),
            {{},         {"a"},      {"a"}, {"a"},      {},         {"a"},      {"b"}, {"a"}, {"a"},      {"a", "b"},
             {"a", "b"}, {"a", "b"}, {"a"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a"}, {"a"}, {"a", "b"}, {"a"},

             {"a", "b"}, {"a", "b"}, {"b"}, {"a", "b"}, {"a"},      {"a"},      {"b"}, {"b"}, {"a", "b"}, {},
             {"b"},      {"a", "b"}},
            {0,  0,  0,  0,  4,  4,  4,  4,  8,  8,  8,  8,  9,  9,  9,  9,
             10, 10, 10, 10, 13, 13, 13, 13, 15, 15, 15, 15, 16, 16, 16, 16},
        },
        {
            "F5_1",
            "G((a)U[0, 4](b))",
            createExpectedResult({{0, true}, {0, true}, {2, true}, {4, true}, {5, false}, {10, true}}),
            {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}},
            {0, 0, 2, 4, 5, 10},
        },
        {
            "F5_2",
            "G((a)U[0, 4](b))",
            createExpectedResult({{0, true}, {1, true}, {2, true}, {3, true}, {4, true}, {5, true}}),
            {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}},
            {0, 1, 2, 3, 4, 5},
        },
        {
            "F5_3",
            "G((a)U[0, 4](b))",
            createExpectedResult({{0, true}, {1, true}, {2, false}, {3, true}, {4, true}, {5, true}}),
            {{"a"}, {"b"}, {}, {"a", "b"}, {"a"}, {"b"}},
            {0, 1, 2, 3, 4, 5},
        },
        {
            "F5_4",
            "G((a)U[0, 4](b))",
            createExpectedResult({{0, true}, {1, true}, {2, true}, {3, true}, {4, false}, {5, false}, {6, true}}),
            {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {}, {"b"}},
            {0, 1, 2, 3, 4, 5, 6},
        },
        {
            "F6_1",
            "G(((a)U[0, 4](b)) and a)",
            createExpectedResult({{0, true}, {0, true}, {2, true}, {4, true}, {5, false}, {10, false}}),
            {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}},
            {0, 0, 2, 4, 5, 10},
        },
        {
            "F7_1",
            "G((a & b) U[0,2](c))",
            createExpectedResult({true, true, true, true, true}),
            {{"c"}, {"c"}, {"a", "b"}, {"a", "b"}, {"c"}},
        },
        {
            "F7_2",
            "G((a & b) U[0,2](c))",
            createExpectedResult({true, true, true, true, true, true}),
            {{"a", "b"}, {"c"}, {"c"}, {"a", "b"}, {"a", "b"}, {"c"}},
        },
        {
            "F7_3",
            "G((a & b) U[0, 2] c)",
            createExpectedResult({true, true, false, false, false}),
            {{"c"}, {"c"}, {"a", "b"}, {"a", "b"}, {"a", "b"}},
        },
        {
            "F7_4",
            "G((a & b) U[0, 2] c)",
            createExpectedResult({true, true, true, true, false}),
            {{"c"}, {"c"}, {"a", "b"}, {"c"}, {"a", "b"}},
        },
        {
            "F7_5",
            "G((a & b) U[0, 2] c)",
            createExpectedResult({true, true, true, true, true, true, true, true}),
            {{"c"}, {"c"}, {"a", "b"}, {"a", "b"}, {"c"}, {"a", "b"}, {"a", "b"}, {"a", "b", "c"}},
        },
        {
            "F7_6",
            "G((a & b) U[0, 2] c)",
            createExpectedResult({true, true, true, true, true, true}),
            {{"a", "b"}, {"a", "b"}, {"c"}, {"a", "b"}, {"a", "b"}, {"c"}},
        },
        {
            "F7_7",
            "G((a & b) U[0, 2] c)",
            createExpectedResult({true, true, true, false, false, false}),
            {{"a", "b"}, {"a", "b"}, {"c"}, {"a", "b"}, {"a", "b"}, {"a", "b"}},
        },
        {
            "F7_8",
            "G((a & b) U[0, 2] c)",
            createExpectedResult({true, true, true, true, true}),
            {{"a", "b"}, {"a", "b"}, {"c"}, {"a", "b"}, {"c"}},
        },
        {
            "F7_9",
            "G((a & b) U[0, 2] c)",
            createExpectedResult({true, true, true, true}),
            {{"a", "b"}, {"c"}, {"a", "b"}, {"c"}},
        },
        {
            "F8_1",
            "G((a)S[0, 4](b) or (a)U[0, 4](b))",
            createExpectedResult({{0, true}, {0, true}, {2, true}, {4, true}, {5, true}, {10, true}}),
            {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}},
            {0, 0, 2, 4, 5, 10},
        },
        {
            "F9_1",
            "G((a)S[0, 4](b) and (a)U[0, 4](b))",
            createExpectedResult({{0, false}, {0, false}, {2, false}, {4, true}, {5, false}, {10, true}}),
            {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}},
            {0, 0, 2, 4, 5, 10},
        },
        {
            "F10_1",
            "G(P[0, 4](!c))",
            createExpectedResult({{0, false}, {0, false}, {1, false}, {4, true}, {4, false}, {5, false}}),
            {{"b", "c"}, {"a", "c"}, {}, {"a", "b", "c"}, {"b", "c"}, {"a", "b", "c"}},
            {0, 0, 1, 4, 4, 5},
        },
        {
            "F11_1",
            "G(a -> P(b))",
            createExpectedResult({false, true, true, true, true, true, true}),
            {{"a"}, {"b"}, {"a"}, {"b"}, {"b"}, {"a"}, {"b"}},
        },
        {
            "F11_2",
            "G(a -> P(b))",
            createExpectedResult({true, false, true, true, false}),
            {{"c"}, {"a"}, {"b"}, {"a"}, {"a"}},
        },
        {
            "F11_3",
            "G(a -> P(b))",
            createExpectedResult({true, true, true, true, true, true, true}),
            {{"c"}, {"b"}, {"a"}, {"b"}, {"b"}, {"a"}, {"b"}},
        },
        {
            "F12_1",
            "G(true U a)",
            createExpectedResult({true, true, true, true, true, true, true}),
            {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}},
        },
        {
            "F13_1",
            "G(F(a))",
            createExpectedResult({true, true, true, true, true, true, true}),
            {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}},
        },
        {
            "F14_1",
            "G(!((true) U (!a)))",
            createExpectedResult({true, true, true, true, true, true, true}),
            {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}},
        },
        {
            "F15_1",
            "G(a)",
            createExpectedResult({true, true, true, true, true, true, true}),
            {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}},
        },
        {
            "F15_2",
            "G(a)",
            createExpectedResult({false, true, true, true, false, true, true}),
            {{"b"}, {"a"}, {"a"}, {"a"}, {"b"}, {"a"}, {"a"}},
        },
        {
            "F16_1",
            "G(a -> F[0, 6](b & O[0, 6](c)))",
            createExpectedResult({true, true, true, false, true, true, true}),
            {{"b"}, {"b"}, {"b"}, {"a"}, {"b"}, {"b"}, {"b"}},
        },
        {
            "F16_2",
            "G(a -> F[0, 6](b & O[0, 6](c)))",
            createExpectedResult({true, true, true, true, true, true, true}),
            {{"c"}, {"b"}, {"b"}, {"a"}, {"b"}, {"c"}, {"b"}},
        },
        {
            "F17_1",
            "G(a -> F[0, 6](b))",
            createExpectedResult({true, true, true, true, true, true, true}),
            {{"b"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"b"}},
        },
        {
            "F17_2",
            "G(a -> F[0, 6](b))",
            createExpectedResult({true, false, false, false, false, false, false}),
            {{"b"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}},
        },
        {
            "F17_3",
            "G(a -> F[0, 6](b))",
            createExpectedResult({true, true, true, false, false}),
            {{"c"}, {"a"}, {"b"}, {"a"}, {"a"}},
        },
        {
            "F18_1",
            "G(a -> O[0, 4](b))",
            createExpectedResult({true, false, true, true, true}),
            {{"c"}, {"a"}, {"b"}, {"a"}, {"a"}},
        },
        {
            "F19_1",
            "G(a -> O[0, 6](b))",
            createExpectedResult({true, true, true, true, true, true, true}),
            {{"b"}, {"c"}, {"a"}, {"b"}, {"b"}, {"a"}, {"b"}},
        },
        {
            "F20_1",
            "G(O[0, 6](a))",
            createExpectedResult({true, true, true, true, true, true, true}),
            {{"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}},
        },
        {
            "F20_2",
            "G(O[0, 6](a))",
            createExpectedResult({false, true, true, true, true, true, true}),
            {{}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}, {"a"}},
        },
        {
            "F21_1",
            "(a & b) U[0, 5] (c)",
            createExpectedResult({true, true, false, false, false, false, true, true}),
            {{"a"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"c"}, {"a"}},
        },
        {
            "F22_1",
            "(a & b) U[0, 5] (c)",
            createExpectedResult({false, false, false, false, true, true, true}),
            {{"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a"}, {"c"}, {"a"}, {"a"}},
        },
        {
            "F22_1",
            "G(O[0,2](a))",
            createExpectedResult({true, true, true, true, true, true, true}),
            {{"a"}, {"a"}, {"a", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}},
        },
        {
            "F22_2",
            "G(O[0,2](a))",
            createExpectedResult({false, false, false, true, true, true, true}),
            {{"b"}, {"b"}, {"b", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}},
        },
        {
            "F23_1",
            "G(!(true S(!a)))",
            createExpectedResult({true, true, true, true, true, true, false}),
            {{"a"}, {"a"}, {"a", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}},
        },
        {
            "F24_1",
            "G(H(a))",
            createExpectedResult({true, true, true, true, true, true, true}),
            {{"a"}, {"a"}, {"a", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"a", "b"}},
        },
        {
            "F24_2",
            "G(H(a))",
            createExpectedResult({true, true, true, true, true, true, false}),
            {{"a"}, {"a"}, {"a", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}},
        },
        {
            "F25_1",
            "G(a -> O(b & P(!b)))",
            createExpectedResult({true, true, true, true, false}),
            {{"b"}, {"b"}, {"b"}, {"c"}, {"a"}},
        },
        {
            "F25_2",
            "G(a -> O(b & P(!b)))",
            createExpectedResult({true, true, true, true, true, true, true, true}),
            {{"c"}, {"b"}, {"a", "b"}, {"c"}, {"a"}, {"b"}, {"a"}, {"a"}},
        },
        {
            "F26_1",
            "G(b & P(!b))",
            createExpectedResult({false, true, false, false, false, true, false, false}),
            {{"c"}, {"b"}, {"a", "b"}, {"c"}, {"a"}, {"b"}, {"a"}, {"a"}},
        },
        {
            "F27_1",
            "G(true S (P(!(b)) & b))",
            createExpectedResult({false, true, true, true, true, true, true, true}),
            {{"c"}, {"b"}, {"a", "b"}, {"c"}, {"a"}, {"b"}, {"a"}, {"a"}},
        },
        {
            "F28_1",
            "G(P(!(b)) & b)",
            createExpectedResult({false, true, false, false, false, true, false, false}),
            {{"c"}, {"b"}, {"a", "b"}, {"c"}, {"a"}, {"b"}, {"a"}, {"a"}},
        },
        {
            "F29_1",
            "G((a & O(b & P(!b))) -> c)",
            createExpectedResult({true, true, true, false, true}),
            {{"c"}, {"b"}, {"b"}, {"a"}, {"c"}},
        },
        {
            "F29_2",
            "G((a & O(b & P(!b))) -> c)",
            createExpectedResult({true, true, true, true, true}),
            {{"c"}, {"b"}, {"b"}, {"c", "a"}, {"c"}},
        },
        {
            "F29_3",
            "G((a & O(b & P(!b))) -> c)",
            createExpectedResult({true, true, true}),
            {{"a"}, {"a"}, {"a"}},
        },
        {
            "F30_1",
            "G((!a & P(a)) -> (O[1,5](H[0,2](a & b))))",
            createExpectedResult({true, true, true, true, true, true, true, true, true, true}),
            {{"c"}, {"a"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a"}, {"a"}, {"c"}, {"c"}},
        },
        {
            "F30_2",
            "G((!a & P(a)) -> (O[1,5](H[0,2](a & b))))",
            createExpectedResult({true, true, true, true, true, true, false, true}),
            {{"c"}, {"a"}, {"a", "b"}, {"a", "b"}, {"a"}, {"a"}, {"c"}, {"c"}},
        },
        {
            "F31_1",
            "G((!a & P(a)) -> (O[1,3](a & b)))",
            createExpectedResult({true, true, true, true, true, true, false, true}),
            {{"c"}, {"a"}, {"a", "b"}, {"a"}, {"a"}, {"a"}, {"c"}, {"c"}},
        },
        {
            "F31_2",
            "G((!a & P(a)) -> (O[1,3](a & b)))",
            createExpectedResult({true, true, true, true, true, true}),
            {{"c"}, {"a"}, {"a", "b"}, {"a"}, {"c"}, {"c"}},
        },
        {
            "F32_1",
            "G(O[1,3](a & b))",
            createExpectedResult({false, false, false, true, true, true, false, false}),
            {{"c"}, {"a"}, {"a", "b"}, {"a"}, {"a"}, {"a"}, {"c"}, {"c"}},
        },
        {
            "F33_1",
            "G(!a & P(a))",
            createExpectedResult({false, false, false, false, false, false, true, false}),
            {{"c"}, {"a"}, {"a", "b"}, {"a"}, {"a"}, {"a"}, {"c"}, {"c"}},
        },
        {
            "F34_1",
            "G(c -> H[0,1](a & b))",
            createExpectedResult({true, true, true, true, true, true, true}),
            {{"a"}, {"a"}, {"a", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}},
        },
        {
            "F34_2",
            "G(c -> H[0,1](a & b))",
            createExpectedResult({true, true, false, true, true, true}),
            {{"a"}, {"a"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}},
        },
        {
            "F35_1",
            "G(b -> (X(a)))",
            createExpectedResult({true, true, false}),
            {{"b"}, {"a"}, {"b"}},
        },
        {
            "F36_1",
            "G(a & !O[0,30](b))",
            createExpectedResult({false, false, false, false, false}),
            {{}, {}, {"a", "b"}, {"a"}, {"a"}},
        },
        {
            "F37_1",
            "G(a & O[0,30](!b))",
            createExpectedResult({false, false, true, true, true}),
            {{}, {}, {"a", "b"}, {"a"}, {"a"}},
        },
        {
            "F38_1",
            "G(G[1,2](a & b))",
            createExpectedResult({true, true, true, true, true}),
            {{"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}},
        },
        {
            "F38_2",
            "G(G[1,2](a & b))",
            createExpectedResult({true, true, true, true, true}),
            {{"b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}},
        },
        {
            "F39_1",
            "G(in_same_lane & !O[0,30](in_same_lane & !(in_single_lane) & P(in_single_lane)))",
            createExpectedResult({false, false, false, false, false}),
            {{"in_single_lane"}, {"in_single_lane"}, {"in_same_lane"}, {"in_same_lane"}, {"in_same_lane"}},
        },
        {
            "F40_1",
            "G((in_same_lane & !O[0,30](in_same_lane & !(in_single_lane) & orientation_towards & P(in_single_lane & "
            "orientation_towards))) -> keeps_safe_distance_prec)",
            createExpectedResult({true, true, false, false, false}),
            {{"in_same_lane", "keeps_safe_distance_prec"},
             {"in_same_lane", "keeps_safe_distance_prec"},
             {"in_same_lane", "in_single_lane"},
             {"in_same_lane", "in_single_lane"},
             {"in_same_lane", "in_single_lane"}},
        },
        {
            "F41_1",
            "G(a -> F(b))",
            createExpectedResult(
                {true, true, true, true, true, true, true, true, true, true, true, true, true, true, false}),
            {{"a"}, {"b"}, {"a"}, {"b"}, {"a"}, {"c"}, {"a"}, {"a"}, {"b"}, {"g"}, {"f"}, {"h"}, {"c"}, {"b"}, {"a"}},
        },
        {
            "F42_1",
            "F(!G(b -> Fc))",
            createExpectedResult(
                {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true}),
            {{"a"}, {"b"}, {"a"}, {"b"}, {"a"}, {"c"}, {"a"}, {"a"}, {"b"}, {"g"}, {"f"}, {"h"}, {"c"}, {"b"}, {"a"}},
        },
        {
            "F42_2",
            "F(!G(b -> Fc))",
            createExpectedResult({false, false, false, false, false, false, false, false, false, false, false, false,
                                  false, false, false}),
            {{"a"}, {"b"}, {"a"}, {"b"}, {"a"}, {"c"}, {"a"}, {"a"}, {"b"}, {"g"}, {"f"}, {"h"}, {"c"}, {"b"}, {"c"}},
        },
        {
            "F43_1",
            "G(b -> Fc)",
            createExpectedResult(
                {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true}),
            {{"a"}, {"b"}, {"a"}, {"b"}, {"a"}, {"c"}, {"a"}, {"a"}, {"b"}, {"g"}, {"f"}, {"h"}, {"c"}, {"b"}, {"c"}},
        },
        {
            "F44_1",
            "G((a & b & c & F(d)) -> (!(!(e) & F(e))))",
            createExpectedResult(
                {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true}),
            {{"a"}, {"b"}, {"a"}, {"b"}, {"a"}, {"c"}, {"a"}, {"a"}, {"b"}, {"e"}, {"d"}, {"e"}, {"c"}, {"b"}, {"c"}},
        },
        {
            "F45_1",
            "G(green -> !red U yellow)",
            createExpectedResult({false, true, true, true, true, true, false, true}),
            {{"green"}, {"red"}, {"yellow"}, {"red"}, {"green"}, {"yellow"}, {"green"}, {"red"}},
        },
        {
            "F45_2",
            "G(green -> !red U yellow)",
            createExpectedResult({false, true, true, true, true, true, true, true}),
            {{"green"}, {"red"}, {"yellow"}, {"red"}, {"yellow"}, {"red"}, {"green"}, {"yellow"}},
        },
        {
            "F45_3",
            "G(green -> !red U yellow)",
            createExpectedResult({true, true, true, true, true, true, true}),
            {{"green"}, {"yellow"}, {"red"}, {"green"}, {"yellow"}, {"red"}, {"red"}},
        },
        {
            "F45_4",
            "G(green -> !red U yellow)",
            createExpectedResult({true, true, true, true, true, false, true}),
            {{"green"}, {"yellow"}, {"red"}, {"green"}, {"yellow"}, {"green"}, {"red"}},
        },
        {
            "F45_5",
            "G(green -> !red U yellow)",
            createExpectedResult({true, true, true, true, true, true, true}),
            {{"green"}, {"yellow"}, {"red"}, {"yellow"}, {"red"}, {"green"}, {"yellow"}},
        },
        {
            "F46_1",
            "G(green -> P(yellow))",
            createExpectedResult({false, true, true, true, true, false}),
            {{"green"}, {"red"}, {"yellow"}, {"green"}, {"red"}, {"green"}},
        },
        {
            "F46_2",
            "G(green -> P(yellow))",
            createExpectedResult({true, false, true, true, true, false, true}),
            {{"red"}, {"green"}, {"red"}, {"yellow"}, {"red"}, {"green"}, {"yellow"}},
        },
        {
            "F46_3",
            "G(green -> P(yellow))",
            createExpectedResult({true, true, true, true, true, true, true, true}),
            {{"yellow"}, {"green"}, {"red"}, {"yellow"}, {"green"}, {"red"}, {"yellow"}, {"green"}},
        },
        {
            "F47_1",
            "a U b",
            createExpectedResult({true, true, true}),
            {{"b"}, {"a"}, {"b"}},
        },
        {
            "F48_1",
            "G(X(a))",
            createExpectedResult({true, true, true, false}),
            {{"a"}, {"a"}, {"a"}, {"a"}},
        },
        {
            "F49_1",
            "G(W(a))",
            createExpectedResult({true, true, true, true}),
            {{"a"}, {"a"}, {"a"}, {"a"}},
        },
        {
            "F50_1",
            "G(a -> F[2, 5](b))",
            createExpectedResult({false, true}),
            {{"a"}, {"b"}},
        },
        {
            "F51_1",
            "G(G[2,4](b))",
            createExpectedResult({true, false, false, false, false, true, true}),
            {{"a"}, {"a"}, {"b"}, {"b"}, {"b"}, {"a"}, {"a"}},
        },
        {
            "F52_1",
            "G(G[2,4](a & b))",
            createExpectedResult({true, false, false, false, false, true, true}),
            {{"a"}, {"b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a"}, {"a"}},
        },
        {
            "F53_1",
            "G((a & b) U[0, 4] c)",
            createExpectedResult({false, true, true, true, true, true, true, false, false}),
            {{"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"c"}, {"c"}, {"a"}, {"a"}},
        },
        {
            "F53_2",
            "G((a & b) U[0, 4] c)",
            createExpectedResult({true, true, true, true, true}),
            {{"a", "b"}, {"c"}, {"a", "b"}, {"a", "b"}, {"c"}},
        },
        {
            "F54_1",
            "G((a & b) U[0, 3] c)",
            createExpectedResult({false, true, true, true, true}),
            {{"a"}, {"a", "b"}, {"a", "b"}, {"c"}, {"c"}},
        },
        {
            "F54_2",
            "G((a & b) U[0, 3] c)",
            createExpectedResult({false, false, false, false, true, true, true, true, false}),
            {{"d"}, {"d", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"c"}, {"a"}},
        },
        {
            "F55_1",
            "G((a & b) U[2, 4] (c))",
            createExpectedResult({false, false, true, true, false, false, false, false}),
            {{"a"}, {"a"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"c"}, {"c"}, {"a"}},
        },
        {
            "F55_2",
            "G((a & b) U[2, 4] (c))",
            createExpectedResult({true, true, true, true, false, false, false}),
            {{"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b", "c"}, {"c"}, {"a", "c"}},
        },
        {
            "F56_1",
            "G(a U[0, 2] b)",
            createExpectedResult({false, true, true, true}),
            {{"a"}, {"a"}, {"a"}, {"b"}},
        },
        {
            "F56_2",
            "G(a U[0, 2] b)",
            createExpectedResult({true, true, true}),
            {{"a"}, {"a"}, {"b"}},
        },
        {
            "F56_3",
            "G(a U[0, 2] b)",
            createExpectedResult({true, true, true, true}),
            {{"a"}, {"b"}, {"a"}, {"b"}},
        },
        {
            "F57_1",
            "G((a & b) U[1, 3] c)",
            createExpectedResult({false, true, true, true, false}),
            {{"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"c"}},
        },
        {
            "F57_2",
            "G((a & b) U[1, 3] c)",
            createExpectedResult({false, true, true, true, false}),
            {{"c"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"c"}},
        },
        {
            "F58_1",
            "G((a & b) U[0, 1] c)",
            createExpectedResult({false, false}),
            {{"a", "b"}, {"a"}},
        },
        {
            "F59_1",
            "G(a S[0, 2] b)",
            createExpectedResult({true, true, true, true, true}),
            {{"b"}, {"b"}, {"b"}, {"a"}, {"a"}},
        },
        {
            "F60_1",
            "G((a & b) S[0, 2] c)",
            createExpectedResult({true, true, true, true, true, false}),
            {{"c"}, {"c"}, {"c"}, {"a", "b"}, {"a", "b"}, {"a", "b"}},
        },
        {
            "F60_2",
            "G((a & b) S[0, 2] c)",
            createExpectedResult({false, true, true, true, true}),
            {{}, {"c"}, {"c"}, {"a", "b"}, {"a", "b"}},
        },
        {
            "F61_1",
            "G((a & b) S[0, 3] c)",
            createExpectedResult({true, true, true, true, true, true}),
            {{"c"}, {"c"}, {"c"}, {"a", "b"}, {"a", "b"}, {"a", "b"}},
        },
        {
            "F62_1",
            "G((a & b) S[1, 3] c)",
            createExpectedResult({false, false, false, true, true}),
            {{"c"}, {"c"}, {"c"}, {"a", "b"}, {"a", "b"}},
        },
        {
            "F63_1",
            "G(X[0,1](a & b))",
            createExpectedResult({true, true, true, true, false}),
            {{"a"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}},
        },
        {
            "F63_2",
            "G(X[0,1](a & b))",
            createExpectedResult({true, true, true, false, false, false}),
            {{"a"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"b"}, {"b"}},
        },
        {
            "F63_3",
            "G(X[0,1](a & b))",
            createExpectedResult({false, false, true, false, false}),
            {{"a"}, {"b"}, {"b"}, {"a", "b"}, {"b"}},
        },
        {
            "F63_4",
            "G(X[0,1](a & b))",
            createExpectedResult({
                true,
                true,
                true,
                false,
                false,
                true,
                false,
            }),
            {{"a"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"b"}, {"b"}, {"a", "b"}},
        },
        {
            "F64_1",
            "G(P[0,1](a & b))",
            createExpectedResult({false, true, true, true, true}),
            {{"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"b"}},
        },
        {
            "F65_1",
            "G(G[0,2](a & b))",
            createExpectedResult({true, true, true, true, true}),
            {{"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}},
        },
        {
            "F65_2",
            "G(G[0,2](a & b))",
            createExpectedResult({true, true, false, false, false}),
            {{"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"b"}},
        },
        {
            "F65_3",
            "G(G[0,2](a & b))",
            createExpectedResult({false, false, false, false, false, false}),
            {{"a", "b"}, {"a"}, {"a"}, {"a", "b"}, {"a", "b"}, {"b"}},
        },
        {
            "F65_4",
            "G(G[0,2](a & b))",
            createExpectedResult({false, false, false, false, false}),
            {{"b"}, {"a"}, {"a", "b"}, {"a", "b"}, {"b"}},
        },
        {
            "F65_5",
            "G(G[0,2](a & b))",
            createExpectedResult({false, false, false, false, false}),
            {{"a", "b"}, {"a", "b"}, {"a"}, {"a", "b"}, {"b"}},
        },
        {
            "F65_6",
            "G(G[0,2](a & b))",
            createExpectedResult({false, true, true, true, true}),
            {{"b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}},
        },
        {
            "F66_1",
            "G(G[2,4](b))",
            createExpectedResult({true, true, true, true, true}),
            {{"a"}, {}, {"b"}, {"a", "b"}, {"a", "b"}},
        },
        {
            "F67_1",
            "G(true U[0,2] !(a & b))",
            createExpectedResult({false, false, true, true, true}),
            {{"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"b"}},
        },
        {
            "F68_1",
            "G(!((true) U[1,2] (!(a & b))))",
            createExpectedResult({true, true, true, true, true}),
            {{"b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}},
        },
        {
            "F68_2",
            "G(!((true) U[1,2] (!(a & b))))",
            createExpectedResult({true, true, true, true, true}),
            {{"a,"
              "b"},
             {"a", "b"},
             {"a", "b"},
             {"a", "b"},
             {"a", "b"}},
        },
        {
            "F69_1",
            "G(F[1,3](a & b))",
            createExpectedResult({true, true, true, true, false}),
            {{"a"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}},
        },
        {
            "F69_2",
            "G(F[1,3](a & b))",
            createExpectedResult({true, true, true, true, false, true, true, true, false}),
            {{"a"}, {"b"}, {"a"}, {"a", "b"}, {"a", "b"}, {"a"}, {"b"}, {"a"}, {"a", "b"}},
        },
        {
            "F70_1",
            "G(F[1,2](a & b))",
            createExpectedResult({false, true, true, true, false}),
            {{"a"}, {"b"}, {"a"}, {"a", "b"}, {"a", "b"}},
        },
        {
            "F71_1",
            "G(F[0,2](a & b))",
            createExpectedResult({true, true, true, true, true}),
            {{"a"}, {"b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}},
        },
        {
            "F71_2",
            "G(F[0,2](a & b))",
            createExpectedResult({false, true, true, true, true}),
            {{"a"}, {"b"}, {"a"}, {"a", "b"}, {"a", "b"}},
        },
        {
            "F71_3",
            "G(F[0,2](a & b))",
            createExpectedResult({true, true, true, true, true, false}),
            {{"a"}, {"b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"a"}},
        },
        {
            "F72_1",
            "G(O[0, 2](a))",
            createExpectedResult({false, true, true, true, true, true, true}),
            {{}, {"a"}, {"b"}, {"a"}, {"a"}, {"a"}, {"a"}},
        },
        {
            "F72_2",
            "G(O[0, 2](a))",
            createExpectedResult({true, true, true, true, true, true, true}),
            {{"a"}, {"a"}, {"a", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}},
        },
        {
            "F72_3",
            "G(O[0, 2](a))",
            createExpectedResult({false, false, false, true, true, true, true}),
            {{"b"}, {"b"}, {"b", "b"}, {"a", "b", "c"}, {"a"}, {"a"}, {"b"}},
        },
        {
            "F73_1",
            "G(O[1, 2](a))",
            createExpectedResult({false, true, true, false, true, true, true}),
            {{"a"}, {"b"}, {"b"}, {"a"}, {"a"}, {"a"}, {"a"}},
        },
        {
            "F74_1",
            "G(H[0,2](a & b))",
            createExpectedResult({true, true, true, true, false}),
            {{"a", "b"}, {"a", "b"}, {"a", "b"}, {"a", "b"}, {"b"}},
        },
        {
            "F74_2",
            "G(H[0,2](a & b))",
            createExpectedResult({true, true, false, false, false}),
            {{"a", "b"}, {"a", "b"}, {}, {"a"}, {"b"}},
        },
        {
            "F75_1",
            "G(((a)U[0, 4](b)) & a)",
            createExpectedResult({true, true, true, true, true, false}),
            {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}},
        },
        {
            "F76_1",
            "G(A x in D. higher_than_time_step(x))",
            createExpectedResult({true, true, false, false, false}),
            {},
            {{}, {}, {}, {}, {}},
            {0, 1, 2, 3, 4},
            {Domain{"D", {"2, 3, 4"}}},
            {{"higher_than_time_step", 1,
              [](int argc, char *args[]) -> bool {
                  size_t timeStep = strtol(args[0], nullptr, 10);
                  size_t val = strtol(args[1], nullptr, 10);
                  return val > timeStep;
              }, true
            }}
        },
        {
            "F77_1",
            "G(A x in D. p(x))",
            createExpectedResult({true, true, false, false}),
            {"p(a)", "p(b)", "p(c)"},
            {{"p(a)", "p(b)", "p(c)"}, {"p(a)", "p(b)", "p(c)"}, {"p(b)"}, {"p(a)", "p(b)"}},
            {0, 1, 2, 3},
            {Domain{"D", {"a", "b", "c"}}}
        },
        {
            "F78_1",
            "G(A x in D. x)",
            createExpectedResult({true, true, true}),
            {},
            {{}, {}, {}},
            {0, 1, 2},
            {Domain{"D", std::vector<std::string>()}}
        },
        {
            "F79_1",
            "G(A x in D. X(divisible_by_time_step(x)))",
            createExpectedResult({true, true, false, true, false}),
            {},
            {{}, {}, {}, {}, {}},
            {0, 1, 2, 3, 4},
            {Domain{"D", {"4, 8, 16"}}},
            {{"divisible_by_time_step", 1,
              [](int argc, char *args[]) -> bool {
                  size_t timeStep = strtol(args[0], nullptr, 10);
                  size_t val = strtol(args[1], nullptr, 10);
                  if (timeStep == 0)
                      return false;
                  return val % timeStep == 0;
              }, true
            }}
        },
        {
            "F80_1",
            "G(A x in D. ((true)U(x)))",
            createExpectedResult({true, true, true, true, false}),
            {"a", "b", "c"},
            {{"a"}, {}, {}, {"a", "b"}, {"c"}},
            {0, 1, 2, 3, 4},
            {Domain{"D", {"a", "b", "c"}}}
        },
        {
            "F81_1",
            "G(A x in D. P(x))",
            createExpectedResult({false, true, false, true}),
            {"a", "b", "c", "d"},
            {{"a", "b", "c", "d"}, {"a", "c", "d"}, {"a", "b", "c"}, {}},
            {0, 1, 2, 3},
            {Domain{"D", {"a", "b", "c"}}}
        },
        {
            "F82_1",
            "G(E x in D. equal_to_time_step(x))",
            createExpectedResult({false, true, false, true, false, true}),
            {},
            {{}, {}, {}, {}, {}, {}},
            {0, 1, 2, 3, 4, 5},
            {Domain{"D", {"1", "3", "5", "7"}}},
            {{"equal_to_time_step", 1,
              [](int argc, char *args[]) -> bool {
                  size_t timeStep = strtol(args[0], nullptr, 10);
                  size_t val = strtol(args[1], nullptr, 10);
                  return val == timeStep;
              }, true
            }}
        },
        {
            "F83_1",
            "G(Q[0.2, 0.8] x in D. lower_than_time_step(x))",
            createExpectedResult({false, true, true, true, true, false}),
            {},
            {{}, {}, {}, {}, {}, {}},
            {0, 1, 2, 3, 4, 5},
            {Domain{"D", {"0", "1", "2", "3", "4"}}},
            {{"lower_than_time_step", 1,
              [](int argc, char *args[]) {
                  size_t timeStep = strtol(args[0], nullptr, 10);
                  size_t val = strtol(args[1], nullptr, 10);
                  return val < timeStep;
              }, true
            }}
        },
        {
            "F84_1",
            "G(Q[0.5, 1.] x in D. x)",
            createExpectedResult({true, false, true, false, true}),
            {"a", "b", "c"},
            {{"a", "b"}, {"c"}, {"a", "b", "c"}, {"a"}, {"b", "c"}},
            {0, 1, 2, 3, 4},
            {Domain{"D", {"a", "b", "c"}}}
        },
        {
            "F85_1",
            "G(C=2 x in D. x)",
            createExpectedResult({false, true, true, false, false}),
            {"a", "b", "c", "d"},
            {{"a"}, {"a", "d"}, {"b", "d"}, {"c"}, {"a", "b", "d"}},
            {0, 1, 2, 3, 4},
            {Domain{"D", {"a", "b", "d"}}}
        },
        {
            "F86_1",
            "G(C<=2 x in D. x)",
            createExpectedResult({false, true, true, true}),
            {"a", "b", "c"},
            {{"a", "b", "c"}, {}, {"b"}, {"a", "c"}},
            {0, 1, 2, 3},
            {Domain{"D", {"a", "b", "c"}}}
        },
        {
            "F87_1",
            "G(Q[0.5, 1.] x in D0. E y in D1. x & y)",
            createExpectedResult({true, false, false, true}),
            {"a", "b", "c", "d", "e", "f", "g"},
            {{"a", "b", "c", "d"}, {"b", "e"}, {"b", "c"}, {"a", "c", "e", "f"}},
            {0, 1, 2, 3},
            std::vector<Domain>{Domain{"D0", {"a", "b", "c"}}, Domain{"D1", {"d", "e", "f", "g"}}}
        },
        {
            "F88_1",
            "G(E x in D0. x & A y in D1. y)",
            createExpectedResult({true, false, false, true}),
            {"a", "b", "c", "d", "e", "f"},
            {{"a", "d", "e", "f"}, {"d", "e", "f"}, {"a", "b", "c", "e", "f"}, {"b", "c", "d", "e", "f"}},
            {0, 1, 2, 3},
            std::vector<Domain>{Domain{"D0", {"a", "b", "c"}}, Domain{"D1", {"d", "e", "f"}}}
        },
        {
            "F89_1",
            "G(A x in D0. E x in D1. equal_to_time_step(x))",
            createExpectedResult({true, true, true, false}),
            {},
            {{}, {}, {}, {}},
            {0, 1, 2, 3},
            std::vector<Domain>{Domain{"D0", {"7", "8", "9"}}, Domain{"D1", {"0", "1", "2"}}},
            {{"equal_to_time_step", 1,
              [](int argc, char *args[]) {
                  size_t timeStep = strtol(args[0], nullptr, 10);
                  size_t val = strtol(args[1], nullptr, 10);
                  return val == timeStep;
              }, true
            }}
        },
        {
            "F90_1",
            "G(A x, y in D. x == y -> equals(x, y))",
            createExpectedResult(std::vector<bool>{true}),
            {},
            {{}},
            {0},
            {Domain{"D", {"1", "2", "3", "4"}}},
            {{"equals", 2,
              [](int argc, char *args[]) {
                  size_t leftVal = strtol(args[0], nullptr, 10);
                  size_t rightVal = strtol(args[1], nullptr, 10);
                  return leftVal == rightVal;
              }
            }}
        },
        {
            "F91_1",
            "G(E x in D. (O(x) & F(x)))",
            createExpectedResult({true, true, true, false, false, true}),
            {"a", "b", "c", "d"},
            {{"a", "b", "c"}, {}, {"a", "b"}, {}, {}, {"d"}},
            {0, 1, 2, 3, 4, 5},
            {Domain{"D", {"a", "c", "d"}}}
        },
        {
            "F92_1",
            "G(A x in D. ((a)S(x)))",
            createExpectedResult({false, true, true, true, false}),
            {"a", "b", "c"},
            {{"b", "c"}, {"a"}, {"a", "b", "c"}, {"a", "b", "c"}, {"b"}},
            {0, 1, 2, 3, 4},
            {Domain{"D", {"a", "b", "c"}}}
        },
        {
            "F93_1",
            "G(C=2 x in D. X(P(X(P(x)))))",
            createExpectedResult({true, false, true, false, false, false}),
            {"a", "b", "c"},
            {{"a", "b"}, {"a", "b", "c"}, {"a", "c"}, {}, {"b"}, {"b", "c"}},
            {0, 1, 2, 3, 4, 5},
            {Domain{"D", {"a", "b", "c"}}}
        },
        {
            "F94_1",
            "G(X(E x in D. (x & x == 'a')))",
            createExpectedResult({false, true, false, false}),
            {"a", "b", "c"},
            {{"a", "b"}, {"b"}, {"a"}, {"c"}},
            {0, 1, 2, 3},
            {Domain{"D", {"a", "b", "c"}}}
        },
        {
            "F95_1",
            "G((b & X(!(b)) & d & !(a)) -> (O(G[0,30](b & c))))",
            createExpectedResult({true, true, true, true, true, false, true, true, true, true, true, true, true}),
            {{"d"}, {"d"}, {"d"}, {"d"}, {"d"}, {"b", "d"}, {"d"}, {"d"}, {"d"}, {}, {}, {}, {}},
        },
        {
            "F96_1",
            "G(E x0 in D0, x1 in D1. p0(x0, x1) & p1(x0, x1))",
            createExpectedResult({true, false, false, true}),
            {"p0(a, c)", "p0(a, d)", "p0(b, c)", "p0(b, d)", "p1(a, c)", "p1(a, d)", "p1(b, c)", "p1(b, d)"},
            {{"p0(a, c)", "p0(a, d)", "p1(a, c)"}, {"p0(a, d)", "p1(a, c)", "p1(b, c)"}, {}, {"p0(b, d)", "p1(b, d)"}},
            {0, 1, 2, 3},
            std::vector<Domain>({Domain{"D0", {Value("a"), Value("b")}}, Domain{"D1", {Value("c"), Value("d")}}})
        },
        {
            "F97_1",
            "G(C>=3 x0 in D0, x1 in D1, x2 in D2. p(x0, x1, x2))",
            createExpectedResult({true, false, true, false}),
            {"p(a, c, e)", "p(b, c, e)", "p(a, d, e)", "p(b, d, e)", "p(a, c, f)", "p(b, c, f)", "p(a, d, f)", "p(b, d, f)"},
            {{"p(a, c, e)", "p(a, c, f)", "p(b, c, f)"}, {"p(b, c, f)", "p(a, d, f)"}, {"p(a, c, e)", "p(b, c, e)", "p(a, d, e)", "p(b, d, e)", "p(a, c, f)"}, {}},
            {0, 1, 2, 3},
            std::vector<Domain>({Domain{"D0", {Value("a"), Value("b")}}, Domain{"D1", {Value("c"), Value("d")}},
            Domain{"D2", {Value("e"), Value("f")}}})
        },
        {
            "F98_1",
            "G(A x in D. x(d))",
            createExpectedResult({true, false, false}),
            {"a(d)", "b(d)", "c(d)"},
            {{"a(d)", "b(d)", "c(d)"}, {}, {"a(d)", "c(d)"}},
            {0, 1, 2},
            std::vector<Domain>({Domain{"D", {"a", "b", "c"}}})
        }
    };
}

std::vector<Verdict> TestDataBase::createExpectedResult(const std::vector<std::tuple<size_t, bool>> &baseVerdicts) {
    std::vector<Verdict> newVerdicts;
    for (const auto &base : baseVerdicts)
        newVerdicts.emplace_back(std::get<0>(base), std::get<1>(base));
    return newVerdicts;
}

std::vector<Verdict> TestDataBase::createExpectedResult(const std::vector<bool> &baseVerdicts) {
    std::vector<Verdict> newVerdicts;
    size_t idx{0};
    for (const auto &base : baseVerdicts)
        newVerdicts.emplace_back(idx++, base);
    return newVerdicts;
}

std::vector<TestSample>
TestDataBase::getDatabaseEntries(const std::vector<MonitoringProperties> &supportedFormulaTypes) {
    std::vector<TestSample> supportedTestCases;
    bool requiresEquidistantTime{std::any_of(supportedFormulaTypes.begin(), supportedFormulaTypes.end(),
                                             [](MonitoringProperties prop) { return prop.equidistantTime; })};
    for (const auto &testCase : testCases) {
        auto trace{testCase.trace};
        if (testCase.formula->checkCompatibility(supportedFormulaTypes) and
            (!requiresEquidistantTime or requiresEquidistantTime and trace.isEquidistant()))
            supportedTestCases.push_back(testCase);
    }
    return supportedTestCases;
}
TestSample TestDataBase::getTestSample(const std::string &name) {
    for (const auto &testCase : testCases)
        if (testCase.testName == name)
            return testCase;
    throw std::runtime_error("Test case does not exist in database.");
}
Verdict TestDataBase::mergeResult(const std::vector<Verdict> &expectedResult) {
    for (const auto &elem : expectedResult)
        if (!elem.value)
            return {false};

    return {true};
}
