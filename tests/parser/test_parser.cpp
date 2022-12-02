//
// Created by Sebastian Maierhofer on 12.01.21.
//

#include "test_parser.h"
#include "../rv_algorithms/hydra_2/hydra_test_utils.h"
#include "monitors/formula/formula.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra_2/hydra.h"

TEST(TestParser, interval) {
    Formula formula = Parser::parse("F[0, 3](b)");
    EXPECT_EQ(formula.to_string(), "F[0, 3](b)");

    formula = Parser::parse("F(b)");
    EXPECT_EQ(formula.to_string(), "F(b)");

    formula = Parser::parse("F[0, inf](b)");
    EXPECT_EQ(formula.to_string(), "F[0, inf](b)");

    formula = Parser::parse("(a U[0, 3] b)");
    EXPECT_EQ(formula.to_string(), "(a U[0, 3] b)");

    formula = Parser::parse("(a U b)");
    EXPECT_EQ(formula.to_string(), "(a U b)");

    formula = Parser::parse("(a U[0, inf] b)");
    EXPECT_EQ(formula.to_string(), "(a U[0, inf] b)");

    formula = Parser::parse("(a S[0, 3] b)");
    EXPECT_EQ(formula.to_string(), "(a S[0, 3] b)");

    formula = Parser::parse("(a S b)");
    EXPECT_EQ(formula.to_string(), "(a S b)");

    formula = Parser::parse("(a S[0, inf] b)");
    EXPECT_EQ(formula.to_string(), "(a S[0, inf] b)");

    formula = Parser::parse("P[0, 3](a)");
    EXPECT_EQ(formula.to_string(), "P[0, 3](a)");

    formula = Parser::parse("P[0, inf](a)");
    EXPECT_EQ(formula.to_string(), "P[0, inf](a)");

    formula = Parser::parse("P(a)");
    EXPECT_EQ(formula.to_string(), "P(a)");

    formula = Parser::parse("O[0, 3](a)");
    EXPECT_EQ(formula.to_string(), "O[0, 3](a)");

    formula = Parser::parse("O[0, inf](a)");
    EXPECT_EQ(formula.to_string(), "O[0, inf](a)");

    formula = Parser::parse("O(a)");
    EXPECT_EQ(formula.to_string(), "O(a)");

    formula = Parser::parse("X[0, 3](a)");
    EXPECT_EQ(formula.to_string(), "X[0, 3](a)");

    formula = Parser::parse("X[0, inf](a)");
    EXPECT_EQ(formula.to_string(), "X[0, inf](a)");

    formula = Parser::parse("X(a)");
    EXPECT_EQ(formula.to_string(), "X(a)");

    formula = Parser::parse("X[0, 0](a)");
    EXPECT_EQ(formula.to_string(), "X[0, 0](a)");
}

TEST(TestParser, constant_formula) {
    std::string ex_mtl = "true";
    Formula formula = Parser::parse(ex_mtl, "Ex1");

    std::string ex_mtl_parsed = "true";
    EXPECT_EQ(formula.to_string(), ex_mtl_parsed);
}

TEST(TestParser, ex_formula_parse_rvm_syntax) {
    std::string ex_mtl = "p or (q U[2,2] r)";
    Formula formula = Parser::parse(ex_mtl, "Ex1");

    std::string ex_mtl_parsed = "((q U[2, 2] r) | p)";
    EXPECT_EQ(formula.to_string(), ex_mtl_parsed);
}

TEST(TestParser, ex_formula_parse_hydra_syntax) {
    std::string ex_mtl = "p0 OR (p1 UNTIL[2,2] p2)";
    Formula formula = Parser::parse(ex_mtl, "Ex1");

    std::string ex_mtl_parsed = "((p1 U[2, 2] p2) | p0)";
    EXPECT_EQ(formula.to_string(), ex_mtl_parsed);
}

TEST(TestParser, ex_log_parse_hydra_syntax) {
    std::string ex_log = "@0 p1 p2\n"
                         "@0 p0 p2\n"
                         "@1\n"
                         "@4 p0 p1 p2\n"
                         "@4 p1 p2\n"
                         "@5 p0 p1 p2\n";
    Trace trace = Parser::parse_log(ex_log);

    // NOTE: This test depends on unordered_map order (fragile)
    // Changed output order does not indicate a defect
    std::string log_parsed = "{p2: true; p1: true; p0: false; 0}\n"
                             "{p2: true; p1: false; p0: true; 0}\n"
                             "{p2: false; p1: false; p0: false; 1}\n"
                             "{p2: true; p1: true; p0: true; 4}\n"
                             "{p2: true; p1: true; p0: false; 4}\n"
                             "{p2: true; p1: true; p0: true; 5}\n";
    EXPECT_EQ(trace.to_string(), log_parsed);
}

TEST(TestParser, ex_eval_rvm_syntax_static_log) {
    std::string ex_mtl = "G(p OR (q U[2,2] r))";

    const std::set<std::string> atomics = {"p", "q", "r"};
    const std::vector<std::vector<std::string>> log_entries = {
        {"q", "r"}, {"p", "r"}, {}, {"p", "q", "r"}, {"q", "r"}, {"p", "q", "r"},
    };
    const std::vector<size_t> time_steps = {0, 0, 1, 4, 4, 5};
    Trace trace = Utils::createTrace(atomics, log_entries, time_steps);

    Formula formula = Parser::parse(ex_mtl, "Ex1");
    std::string ex_mtl_parsed = "G(((q U[2, 2] r) | p))";
    EXPECT_EQ(formula.to_string(), ex_mtl_parsed);

    std::vector<Verdict> expSolution{Verdict{0, false}, Verdict{0, true},  Verdict{1, false},
                                     Verdict{4, true},  Verdict{4, false}, Verdict{5, true}};

    hydraTestUtils::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestParser, ex_eval_rvm_syntax) {
    std::string ex_mtl = "G(p OR (q UNTIL[2,2] r))";
    Formula formula = Parser::parse(ex_mtl, "Ex1");

    std::string ex_log = "@0 q r\n"
                         "@0 p r\n"
                         "@1\n"
                         "@4 p q r\n"
                         "@4 q r\n"
                         "@5 p q r\n";

    Trace trace = Parser::parse_log(ex_log);

    std::vector<Verdict> expSolution{Verdict{0, false}, Verdict{0, true},  Verdict{1, false},
                                     Verdict{4, true},  Verdict{4, false}, Verdict{5, true}};
    hydraTestUtils::executeHydraEval(trace, {formula}, {expSolution});
}

TEST(TestParser, ex_eval_hydra_syntax) {
    std::string ex_mtl = "G(p0 OR (p1 UNTIL[2,2] p2))";
    Formula formula = Parser::parse(ex_mtl, "Ex1");

    std::string ex_log = "@0 p1 p2\n"
                         "@0 p0 p2\n"
                         "@1\n"
                         "@4 p0 p1 p2\n"
                         "@4 p1 p2\n"
                         "@5 p0 p1 p2\n";
    Trace trace = Parser::parse_log(ex_log);

    std::vector<Verdict> expSolution{Verdict{0, false}, Verdict{0, true},  Verdict{1, false},
                                     Verdict{4, true},  Verdict{4, false}, Verdict{5, true}};
    hydraTestUtils::executeHydraEval(trace, {formula}, {expSolution});
}
