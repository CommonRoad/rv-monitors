#include "test_database.h"
#include "hydra_2/hydra_test_utils.h"
#include "monitors/collections/domain.h"
#include <string>
#include <utility>

TestSample::TestSample(const std::string &testName, const std::string &formulaStr, std::vector<Verdict> expectedResult,
                       const std::vector<std::vector<std::string>> &trueAtomics, const std::vector<size_t> &timeSteps,
                       const std::vector<std::shared_ptr<DomainStorage>> &domains,
                       const std::vector<std::shared_ptr<IFunction<bool>>> &functions)
    : testName(testName), formula(std::make_shared<Formula>(Parser::parse(formulaStr, testName))),
      expectedResult(std::move(expectedResult)), domains(domains) {
    if (functions.empty())
        trace =
            Utils::createTrace(trueAtomics, timeSteps, std::make_shared<Formula>(Parser::parse(formulaStr, testName)));
    else
        trace = hydraTestUtils::createTraceWithVariable(timeSteps.size(), functions);
}

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
            "F5_5",
            "G((a)U[0, 4](b))",
            createExpectedResult({{0, true}, {1, true}, {2, true}, {3, true}, {4, false}, {5, false}, {6, false}}),
            {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"a"}, {"a"}},
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
            "G(F(!G(b -> Fc)))",
            createExpectedResult(
                {true, true, true, true, true, true, true, true, true, true, true, true, true, true, false}),
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
            createExpectedResult({true, true, true, false, false, true, false}),
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
            "G((b & X(!(b)) & d & !(a)) -> (O(G[0,30](b & c))))",
            createExpectedResult({true, true, true, true, true, false, true, true, true, true, true, true, true}),
            {{"d"}, {"d"}, {"d"}, {"d"}, {"d"}, {"b", "d"}, {"d"}, {"d"}, {"d"}, {}, {}, {}, {}},
        },
        {
            "F77_1",
            "G(a U[0,0] b)",
            createExpectedResult({{0, false}, {1, true}, {2, true}, {3, false}, {4, false}}),
            {{"a"}, {"a", "b"}, {"a", "b"}, {}, {}},
            {0, 1, 2, 3, 4},
        },
        {
            "F78_1",
            "G(X[0,0] a)",
            createExpectedResult({{0, false}, {1, false}, {2, false}, {3, false}, {4, false}}),
            {{"a"}, {"a", "b"}, {"a", "b"}, {}, {}},
            {0, 1, 2, 3, 4},
        },
        {
            "F79_1",
            "G((b U[0,0] (P[0,8] a)) U[0,10] (X[0,0] a))",
            createExpectedResult({{0, false}, {1, false}, {2, false}, {3, false}, {4, false}, {5, false}}),
            {{"a"}, {"a"}, {"a"}, {"a", "b"}, {"a"}, {"b"}},
            {0, 1, 2, 3, 4, 5},
        },
        {
            "F80_1",
            "G((a U[0,0] (P[0,8] b)))",
            createExpectedResult({{0, false}, {1, false}, {2, true}, {3, true}, {4, false}}),
            {{"a"}, {"a", "b"}, {"a", "b"}, {}, {}},
            {0, 1, 2, 3, 4},
        },
        {
            "F81_1",
            "G((b S a) & (X(G(b)) | X(b U c)))",
            createExpectedResult({{0, true}, {1, true}, {2, true}, {3, true}, {4, false}}),
            {{"a", "c"}, {"a", "b"}, {"b"}, {"b"}, {"b"}},
            {0, 1, 2, 3, 4},
        },
        {
            "F82_1",
            "G(X(b))",
            createExpectedResult({{0, true}, {1, true}, {2, true}, {3, true}, {4, false}}),
            {{"a", "c"}, {"a", "b"}, {"b"}, {"b"}, {"b"}},
            {0, 1, 2, 3, 4},
        },
        {"F83_1",
         "G(A x in D. higher_than_time_step(x))",
         createExpectedResult({true, true, false, false, false}),
         {{}, {}, {}, {}, {}},
         {0, 1, 2, 3, 4},
         {std::make_shared<DomainStorage>(DomainStorage(
             "D", {2ul, 3ul, 4ul}, [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); }))},
         {std::make_shared<Function<bool, size_t, size_t>>(
             Function<bool, size_t, size_t>("higher_than_time_step", {"timeStep", "val"},
                                            hydraTestUtils::higher_than_time_step, hydraTestUtils::twoSizeTArgs))}},
        {"F84_1",
         "G(A x in D. x)",
         createExpectedResult({true, true, false, false}),
         {{"a", "b", "c"}, {"a", "b", "c"}, {"b"}, {"a", "b"}},
         {0, 1, 2, 3},
         {std::make_shared<DomainStorage>(DomainStorage(
             "D", {"a", "b", "c"}, [](const std::any &val) { return std::any_cast<const char *>(val); }))}},
        {"F85_1",
         "G(A x in D. x)",
         createExpectedResult({true, true, true}),
         {{}, {}, {}},
         {0, 1, 2},
         {std::make_shared<DomainStorage>(DomainStorage(
             "D", std::vector<std::any>{}, [](const std::any &val) { return std::any_cast<const char *>(val); }))}},
        {"F86_1",
         "G(A x in D. X(divisible_by_time_step(x)))",
         createExpectedResult({true, true, false, true, false}),
         {{}, {}, {}, {}, {}},
         {0, 1, 2, 3, 4},
         {std::make_shared<DomainStorage>(DomainStorage(
             "D", {4ul, 8ul, 16ul}, [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); }))},
         {std::make_shared<Function<bool, size_t, size_t>>(
             Function<bool, size_t, size_t>("divisible_by_time_step", {"timeStep", "val"},
                                            hydraTestUtils::divisible_by_time_step, hydraTestUtils::twoSizeTArgs))}},
        {"F87_1",
         "G(A x in D. ((true)U(x)))",
         createExpectedResult({true, true, true, true, false}),
         {{"a"}, {}, {}, {"a", "b"}, {"c"}},
         {0, 1, 2, 3, 4},
         {std::make_shared<DomainStorage>(DomainStorage(
             "D", {"a", "b", "c"}, [](const std::any &val) { return std::any_cast<const char *>(val); }))}},
        {"F88_1",
         "G(A x in D. P(x))",
         createExpectedResult({false, true, false, true}),
         {{"a", "b", "c", "d"}, {"a", "c", "d"}, {"a", "b", "c"}, {}},
         {0, 1, 2, 3},
         {std::make_shared<DomainStorage>(DomainStorage(
             "D", {"a", "b", "c"}, [](const std::any &val) { return std::any_cast<const char *>(val); }))}},
        {"F89_1",
         "G(E x in D. equal_to_time_step(x))",
         createExpectedResult({false, true, false, true, false, true}),
         {{}, {}, {}, {}, {}, {}},
         {0, 1, 2, 3, 4, 5},
         {std::make_shared<DomainStorage>(
             DomainStorage("D", {1ul, 3ul, 5ul, 7ul},
                           [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); }))},
         {std::make_shared<Function<bool, size_t, size_t>>(
             Function<bool, size_t, size_t>("equal_to_time_step", {"timeStep", "val"},
                                            hydraTestUtils::equal_to_time_step, hydraTestUtils::twoSizeTArgs))}},
        {"F90_1",
         "G(Q[0.2, 0.8] x in D. lower_than_time_step(x))",
         createExpectedResult({false, true, true, true, true, false}),
         {{}, {}, {}, {}, {}, {}},
         {0, 1, 2, 3, 4, 5},
         {std::make_shared<DomainStorage>(
             DomainStorage("D", {0ul, 1ul, 2ul, 3ul, 4ul},
                           [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); }))},
         {std::make_shared<Function<bool, size_t, size_t>>(
             Function<bool, size_t, size_t>("lower_than_time_step", {"timeStep", "val"},
                                            hydraTestUtils::lower_than_time_step, hydraTestUtils::twoSizeTArgs))}},
        {"F91_1",
         "G(Q[0.5, 1.] x in D. x)",
         createExpectedResult({true, false, true, false, true}),
         {{"a", "b"}, {"c"}, {"a", "b", "c"}, {"a"}, {"b", "c"}},
         {0, 1, 2, 3, 4},
         {std::make_shared<DomainStorage>(DomainStorage(
             "D", {"a", "b", "c"}, [](const std::any &val) { return std::any_cast<const char *>(val); }))}},
        {"F92_1",
         "G(C=2 x in D. x)",
         createExpectedResult({false, true, true, false, false}),
         {{"a"}, {"a", "d"}, {"b", "d"}, {"c"}, {"a", "b", "d"}},
         {0, 1, 2, 3, 4},
         {std::make_shared<DomainStorage>(DomainStorage(
             "D", {"a", "b", "d"}, [](const std::any &val) { return std::any_cast<const char *>(val); }))}},
        {"F93_1",
         "G(C<=2 x in D. x)",
         createExpectedResult({false, true, true, true}),
         {{"a", "b", "c"}, {}, {"b"}, {"a", "c"}},
         {0, 1, 2, 3},
         {std::make_shared<DomainStorage>(DomainStorage(
             "D", {"a", "b", "c"}, [](const std::any &val) { return std::any_cast<const char *>(val); }))}},
        {"F94_1",
         "G(Q[0.5, 1.] x in D0. E y in D1. x & y)",
         createExpectedResult({true, false, false, true, false}),
         {{"a", "b", "c", "d"}, {"b", "e"}, {"b", "c"}, {"a", "c", "e", "f"}, {"g"}},
         {0, 1, 2, 3, 4},
         {std::make_shared<DomainStorage>(DomainStorage(
              "D0", {"a", "b", "c"}, [](const std::any &val) { return std::any_cast<const char *>(val); })),
          std::make_shared<DomainStorage>(DomainStorage(
              "D1", {"d", "e", "f", "g"}, [](const std::any &val) { return std::any_cast<const char *>(val); }))}},
        {"F95_1",
         "G(E x in D0. x & A y in D1. y)",
         createExpectedResult({true, false, false, true}),
         {{"a", "d", "e", "f"}, {"d", "e", "f"}, {"a", "b", "c", "e", "f"}, {"b", "c", "d", "e", "f"}},
         {0, 1, 2, 3},
         {std::make_shared<DomainStorage>(DomainStorage(
              "D0", {"a", "b", "c"}, [](const std::any &val) { return std::any_cast<const char *>(val); })),
          std::make_shared<DomainStorage>(DomainStorage(
              "D1", {"d", "e", "f"}, [](const std::any &val) { return std::any_cast<const char *>(val); }))}},
        {"F96_1",
         "G(A x in D0. E x in D1. equal_to_time_step(x))",
         createExpectedResult({true, true, true, false}),
         {{}, {}, {}, {}},
         {0, 1, 2, 3},
         {std::make_shared<DomainStorage>(DomainStorage(
              "D0", {7ul, 8ul, 9ul}, [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); })),
          std::make_shared<DomainStorage>(DomainStorage(
              "D1", {0ul, 1ul, 2ul}, [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); }))},
         {std::make_shared<Function<bool, size_t, size_t>>(
             Function<bool, size_t, size_t>("equal_to_time_step", {"timeStep", "val"},
                                            hydraTestUtils::equal_to_time_step, hydraTestUtils::twoSizeTArgs))}},
        {"F97_1",
         "G(E x in D. (O(x) & F(x)))",
         createExpectedResult({true, true, true, false, false, true}),
         {{"a", "b", "c"}, {}, {"a", "b"}, {}, {}, {"d"}},
         {0, 1, 2, 3, 4, 5},
         {std::make_shared<DomainStorage>(DomainStorage(
             "D", {"a", "b", "d"}, [](const std::any &val) { return std::any_cast<const char *>(val); }))}},
        {"F97_11",
         "G(O(d) & F(d))",
         createExpectedResult({false, false, false, false, false, true}),
         {{"a", "b", "c"}, {}, {"a", "b"}, {}, {}, {"d"}},
         {0, 1, 2, 3, 4, 5},
         {std::make_shared<DomainStorage>(DomainStorage(
             "D", {"a", "b", "d"}, [](const std::any &val) { return std::any_cast<const char *>(val); }))}},
        {"F97_2",
         "G(E x in D. (O(x) & F(x)))",
         createExpectedResult({true, true, true, false, false, true, false}),
         {{"a", "b", "c"}, {}, {"a", "b"}, {}, {}, {"d"}, {}},
         {0, 1, 2, 3, 4, 5, 6},
         {std::make_shared<DomainStorage>(DomainStorage(
             "D", {"a", "b", "d"}, [](const std::any &val) { return std::any_cast<const char *>(val); }))}},
        {"F98_1",
         "G(A x in D. ((a)S(x)))",
         createExpectedResult({false, true, true, true, false}),
         {{"b", "c"}, {"a"}, {"a", "b", "c"}, {"a", "b", "c"}, {"b"}},
         {0, 1, 2, 3, 4},
         {std::make_shared<DomainStorage>(DomainStorage(
             "D", {"a", "b", "c"}, [](const std::any &val) { return std::any_cast<const char *>(val); }))}},
        {"F99_1",
         "G(C=2 x in D. X(P(X(P(x)))))",
         createExpectedResult({true, false, true, false, false, false}),
         {{"a", "b"}, {"a", "b", "c"}, {"a", "c"}, {}, {"b"}, {"b", "c"}},
         {0, 1, 2, 3, 4, 5},
         {std::make_shared<DomainStorage>(DomainStorage(
             "D", {"a", "b", "c"}, [](const std::any &val) { return std::any_cast<const char *>(val); }))}},
        {"F100_1",
         "G(A x in D. x)",
         createExpectedResult({true, false, false}),
         {{"a", "b", "c"}, {}, {"a", "c"}},
         {0, 1, 2},
         {std::make_shared<DomainStorage>(DomainStorage(
             "D", {"a", "b", "c"}, [](const std::any &val) { return std::any_cast<const char *>(val); }))}},
        {"F101_1",
         "G(E x in D. x)",
         createExpectedResult({true, true, true, false, false, true}),
         {{"a"}, {"a"}, {"d"}, {}, {}, {"d"}},
         {0, 1, 2, 3, 4, 5},
         {std::make_shared<DomainStorage>(DomainStorage(
             "D", {"a", "b", "d"}, [](const std::any &val) { return std::any_cast<const char *>(val); }))}},
        {"F102_1",
         "G(E x in D. (P(x) & x))",
         createExpectedResult({false, true, true, true, false, false}),
         {{"a", "b", "c"}, {"a"}, {"a", "b", "d"}, {"d"}, {}, {"d"}},
         {0, 1, 2, 3, 4, 5},
         {std::make_shared<DomainStorage>(DomainStorage(
             "D", {"a", "b", "d"}, [](const std::any &val) { return std::any_cast<const char *>(val); }))}},
        {
            "F103_1",
            "G((PREV[0,0] (NEXT[1,2] ((((NEXT[7,9] (a3 UNTIL[0,0] (a0 UNTIL[0,0] a2))) SINCE[1,8] a2) UNTIL[1,1] ((NOT "
            "(((NEXT[10,10] (PREV[5,5] ((NEXT[1,2] a0) UNTIL[0,4] (a0 SINCE[1,5] a2)))) UNTIL[0,0] ((a2 UNTIL[0,5] a2) "
            "SINCE[0,0] (PREV[10,10] a0))) SINCE[2,7] (((PREV[5,9] a2) UNTIL[1,3] (PREV[8,INFINITY] a0)) OR (NOT "
            "(NEXT[0,0] a1))))) UNTIL[0,2] (NOT ((a3 UNTIL[4,8] a2) UNTIL[4,5] a3)))) OR a1))))",
            createExpectedResult({{0, false},
                                  {9, false},
                                  {11, false},
                                  {17, false},
                                  {18, false},
                                  {20, false},
                                  {24, false},
                                  {31, false},
                                  {32, false},
                                  {34, false}}),
            {{"a0", "a1", "a2", "a3"},
             {"a0", "a1", "a2"},
             {"a1", "a2", "a3"},
             {"a0", "a1", "a2", "a3"},
             {"a1", "a2", "a3"},
             {"a2", "a3"},
             {"a0", "a1", "a2", "a3"},
             {"a1", "a2", "a3"},
             {"a0", "a1", "a2", "a3"},
             {"a0", "a1", "a2"}},
            {0, 9, 11, 17, 18, 20, 24, 31, 32, 34},
        },
        {"F104_1",
         "G((PREV[0,0] (NEXT[1,2] ((((NEXT[7,9] (a3 UNTIL[0,0] a3)) SINCE[1,8] a1) UNTIL[1,1] a0) OR a0))))",
         createExpectedResult({{0, false},   {9, false},   {11, false},  {17, false},  {18, false},
                               {20, false},  {24, false},  {31, false},  {32, false},  {34, false},
                               {43, false},  {52, false},  {60, false},  {67, false},  {75, false},
                               {83, false},  {88, false},  {92, false},  {101, false}, {108, false},
                               {115, false}, {125, false}, {132, false}, {139, false}, {143, false}}),
         {{"a0", "a1", "a2", "a3"},
          {"a0", "a1", "a2"},
          {"a1", "a2", "a3"},
          {"a0", "a1", "a2", "a3"},
          {"a1", "a2", "a3"},
          {"a2", "a3"},
          {"a0", "a1", "a2", "a3"},
          {"a1", "a2", "a3"},
          {"a0", "a1", "a2", "a3"},
          {"a0", "a1", "a2"},
          {"a0", "a1", "a2", "a3"},
          {"a0", "a1", "a2", "a3"},
          {"a1", "a2", "a3"},
          {"a0", "a1", "a2", "a3"},
          {"a0", "a1", "a2", "a3"},
          {"a0", "a1", "a2", "a3"},
          {"a0", "a2"},
          {"a1"},
          {"a0", "a1", "a2", "a3"},
          {"a0", "a1", "a2", "a3"},
          {"a1", "a3"},
          {"a0", "a2", "a3"},
          {"a0", "a1", "a2", "a3"},
          {"a1", "a2"},
          {"a0", "a1", "a2", "a3"}},
         {0, 9, 11, 17, 18, 20, 24, 31, 32, 34, 43, 52, 60, 67, 75, 83, 88, 92, 101, 108, 115, 125, 132, 139, 143}}};
}

std::vector<Verdict> TestDataBase::createExpectedResult(const std::vector<std::tuple<size_t, bool>> &baseVerdicts) {
    std::vector<Verdict> newVerdicts;
    newVerdicts.reserve(baseVerdicts.size());
    for (const auto &base : baseVerdicts)
        newVerdicts.emplace_back(std::get<0>(base), std::get<1>(base));
    return newVerdicts;
}

std::vector<Verdict> TestDataBase::createExpectedResult(const std::vector<bool> &baseVerdicts) {
    std::vector<Verdict> newVerdicts;
    size_t idx{0};
    newVerdicts.reserve(baseVerdicts.size());
    for (const auto &base : baseVerdicts)
        newVerdicts.emplace_back(idx++, base);
    return newVerdicts;
}

std::vector<TestSample> TestDataBase::getDatabaseEntries(const std::vector<MonitoringProperties> &supFormulas) {
    std::vector<TestSample> supportedTestCases;
    bool requiresEquidistantTime{std::any_of(supFormulas.begin(), supFormulas.end(),
                                             [](MonitoringProperties prop) { return prop.equidistantTime; })};
    for (const auto &testCase : testCases) {
        auto trace{testCase.trace};
        if (testCase.formula->checkCompatibility(supFormulas) and
            (!requiresEquidistantTime or requiresEquidistantTime and trace->isEquidistant()))
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
