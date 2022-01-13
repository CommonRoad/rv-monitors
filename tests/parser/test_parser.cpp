//
// Created by Sebastian Maierhofer on 12.01.21.
//

#include "test_parser.h"
#include "../rv_algorithms/hydra_2/test_hydra.h"
#include "monitors/formula/formula.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra_2/hydra.h"

TEST(TestParser, interval) {
    //    Formula formula = Parser::parse("G(a -> F[0, 3](b))");
    //    std::cout << formula.to_string();
    //    EXPECT_EQ(formula.to_string(), "G(a -> F[0, 3](b))");
}

TEST(TestParser, parse_true) {
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

    std::string log_parsed = "{p0: false; p1: true; p2: true; 0}\n"
                             "{p0: true; p1: false; p2: true; 0}\n"
                             "{p0: false; p1: false; p2: false; 0}\n"
                             "{p0: true; p1: true; p2: true; 0}\n"
                             "{p0: false; p1: true; p2: true; 0}\n"
                             "{p0: true; p1: true; p2: true; 1}\n";
    EXPECT_EQ(trace.to_string(), log_parsed);
}

TEST(TestParser, ex_eval_rvm_syntax_static_log) {
    std::string ex_mtl = "p OR (q U[2,2] r)";

    const std::set<std::string> atomics = {"p", "q", "r"};
    const std::vector<std::vector<std::string>> log_entries = {
        {"q", "r"}, {"p", "r"}, {}, {"p", "q", "r"}, {"q", "r"}, {"p", "q", "r"},
    };
    const std::vector<size_t> time_steps = {0, 0, 1, 4, 4, 5};
    Trace trace = Utils::createTrace(atomics, log_entries, time_steps);

    Formula formula = Parser::parse(ex_mtl, "Ex1");
    std::string ex_mtl_parsed = "((q U[2, 2] r) | p)";
    EXPECT_EQ(formula.to_string(), ex_mtl_parsed);

    std::vector<Verdict> expSolution{Verdict{0, false}, Verdict{0, true},  Verdict{1, false},
                                     Verdict{4, true},  Verdict{4, false}, Verdict{5, true}};
    TestHydra::evalHydra(trace, {formula}, {expSolution});
}

TEST(TestParser, ex_eval_rvm_syntax) {
    std::string ex_mtl = "p OR (q UNTIL[2,2] r)";
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
    TestHydra::evalHydra(trace, {formula}, {expSolution});
}

TEST(TestParser, ex_eval_hydra_syntax) {
    std::string ex_mtl = "p0 OR (p1 UNTIL[2,2] p2)";
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
    TestHydra::evalHydra(trace, {formula}, {expSolution});
}
