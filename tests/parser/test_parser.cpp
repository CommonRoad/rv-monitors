#include "test_parser.h"
#include "../rv_algorithms/hydra_2/hydra_test_utils.h"
#include "monitors/expression_tree/binary/bool/equivalence.h"
#include "monitors/expression_tree/binary/bool/implication.h"
#include "monitors/expression_tree/binary/temporal/since.h"
#include "monitors/expression_tree/binary/temporal/until.h"
#include "monitors/expression_tree/nary/bool/and.h"
#include "monitors/expression_tree/nary/bool/or.h"
#include "monitors/expression_tree/unary/bool/not.h"
#include "monitors/expression_tree/unary/first_order/existential_quantifier.h"
#include "monitors/expression_tree/unary/first_order/universal_quantifier.h"
#include "monitors/expression_tree/unary/temporal/future.h"
#include "monitors/expression_tree/unary/temporal/globally.h"
#include "monitors/expression_tree/unary/temporal/historically.h"
#include "monitors/expression_tree/unary/temporal/next_strong.h"
#include "monitors/expression_tree/unary/temporal/once.h"
#include "monitors/expression_tree/unary/temporal/previously.h"
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

TEST(TestParser, universal) {
    std::string ex_1 = "A x, y in D. p(x, y)";
    Formula formula_1 = Parser::parse(ex_1, "Ex1");
    std::string ex_parsed_1 = "A x in D, y in D. p(x, y)";
    EXPECT_EQ(formula_1.to_string(), ex_parsed_1);

    std::string ex_2 = "A x0, x1 in D0, y0, y1, y2 in D2. p(x0, x1, y0, y1, y2)";
    Formula formula_2 = Parser::parse(ex_2, "Ex2");
    std::string ex_parsed_2 = "A x0 in D0, x1 in D0, y0 in D2, y1 in D2, y2 in D2. p(x0, x1, y0, y1, y2)";
    EXPECT_EQ(formula_2.to_string(), ex_parsed_2);

    std::string ex_3 = "A b, c in D . A x, y in is_same_lane(b, c). p(x, y)";
    Formula formula_3 = Parser::parse(ex_3, "Ex3");
    std::string ex_parsed_3 = "A b in D, c in D. A x in is_same_lane(b, c), y in is_same_lane(b, c). p(x, y)";
    EXPECT_EQ(formula_3.to_string(), ex_parsed_3);
}

TEST(TestParser, functionDomain) {
    std::string ex_1 = "A b,c in a(x, y). test";
    Formula formula_1 = Parser::parse(ex_1, "Ex1");
    std::string ex_parsed_1 = "A b in a(x, y), c in a(x, y). test";
    EXPECT_EQ(formula_1.to_string(), ex_parsed_1);

    std::string ex_2{"A x_o in DSL. in_same_lane(x_ego, x_o)"};
    Formula formula_2 = Parser::parse(ex_2, "Ex2");
    std::string ex_parsed_2 = "A x_o in DSL. in_same_lane(x_ego, x_o)";
    EXPECT_EQ(formula_2.to_string(), ex_parsed_2);
}

TEST(TestParser, existential) {
    std::string exi = "E x, y in D. p(x, y)";
    Formula formula = Parser::parse(exi, "Ex1");

    std::string ex_parsed = "E x in D, y in D. p(x, y)";
    EXPECT_EQ(formula.to_string(), ex_parsed);
}

TEST(TestParser, quote) {
    std::string ex_1 = "Q[0.1, 0.3] x, y in D. p(x, y)";
    Formula formula_1 = Parser::parse(ex_1, "Ex1");

    std::string ex_parsed_1 = "Q[0.100000, 0.300000] x in D, y in D. p(x, y)";
    EXPECT_EQ(formula_1.to_string(), ex_parsed_1);

    std::string ex_2 = "Q[1., 1.] x in D. p(x)";
    Formula formula_2 = Parser::parse(ex_2, "Ex2");

    std::string ex_parsed_2 = "Q[1.000000, 1.000000] x in D. p(x)";
    EXPECT_EQ(formula_2.to_string(), ex_parsed_2);
}

TEST(TestParser, counting) {
    std::string ex_1 = "C=2 x in D. p(x)";
    Formula formula_1 = Parser::parse(ex_1, "Ex1");

    std::string ex_parsed_1 = "C=2 x in D. p(x)";
    EXPECT_EQ(formula_1.to_string(), ex_parsed_1);

    std::string ex_2 = "C<=10 x, y in D. p(x, y)";
    Formula formula_2 = Parser::parse(ex_2, "Ex2");

    std::string ex_parsed_2 = "C<=10 x in D, y in D. p(x, y)";
    EXPECT_EQ(formula_2.to_string(), ex_parsed_2);

    std::string ex_3 = "C>=0 x, y in D0, z in D1. p(x, y, z)";
    Formula formula_3 = Parser::parse(ex_3, "Ex3");

    std::string ex_parsed_3 = "C>=0 x in D0, y in D0, z in D1. p(x, y, z)";
    EXPECT_EQ(formula_3.to_string(), ex_parsed_3);
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
    const std::shared_ptr<Trace> trace = Parser::parse_log(ex_log);

    // NOTE: This test depends on unordered_map order (fragile)
    // Changed output order does not indicate a defect
    std::string log_parsed = "{p2: true; p1: true; p0: false; 0}\n"
                             "{p2: true; p1: false; p0: true; 0}\n"
                             "{p2: false; p1: false; p0: false; 1}\n"
                             "{p2: true; p1: true; p0: true; 4}\n"
                             "{p2: true; p1: true; p0: false; 4}\n"
                             "{p2: true; p1: true; p0: true; 5}\n";
    EXPECT_EQ(trace->to_string(), log_parsed);
}

TEST(TestParser, ex_eval_rvm_syntax_static_log) {
    std::string ex_mtl = "G(p OR (q U[2,2] r))";

    const std::set<std::string> atomics = {"p", "q", "r"};
    const std::vector<std::vector<std::string>> log_entries = {
        {"q", "r"}, {"p", "r"}, {}, {"p", "q", "r"}, {"q", "r"}, {"p", "q", "r"},
    };
    const std::vector<size_t> time_steps = {0, 0, 1, 4, 4, 5};
    const std::shared_ptr<Trace> trace = Utils::createTrace(atomics, log_entries, time_steps);

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

    const std::shared_ptr<Trace> trace = Parser::parse_log(ex_log);

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
    const std::shared_ptr<Trace> trace = Parser::parse_log(ex_log);

    std::vector<Verdict> expSolution{Verdict{0, false}, Verdict{0, true},  Verdict{1, false},
                                     Verdict{4, true},  Verdict{4, false}, Verdict{5, true}};
    hydraTestUtils::executeHydraEval(trace, {formula}, {expSolution});
}

namespace {

// Renders the structure of a parse tree with explicit operator names, so that tests do not depend on how
// Expression::to_string() orders and brackets operands. Operands of the commutative operators are sorted.
std::string shape(const std::shared_ptr<Expression> &expr);

std::string shapeOf(const std::string &name, const std::vector<std::shared_ptr<Expression>> &children) {
    std::vector<std::string> parts;
    for (const auto &child : children)
        parts.push_back(shape(child));
    std::string result{name + "("};
    for (size_t i{0}; i < parts.size(); ++i)
        result += (i == 0 ? "" : ", ") + parts[i];
    return result + ")";
}

template <typename T> std::string shapeOfNary(const std::string &name, const std::shared_ptr<T> &nary) {
    std::vector<std::string> parts;
    for (const auto &child : nary->getExpressions())
        parts.push_back(shape(child));
    std::sort(parts.begin(), parts.end());
    std::string result{name + "("};
    for (size_t i{0}; i < parts.size(); ++i)
        result += (i == 0 ? "" : ", ") + parts[i];
    return result + ")";
}

std::string shape(const std::shared_ptr<Expression> &expr) {
    if (const auto e{std::dynamic_pointer_cast<And>(expr)})
        return shapeOfNary("And", e);
    if (const auto e{std::dynamic_pointer_cast<Or>(expr)})
        return shapeOfNary("Or", e);
    if (const auto e{std::dynamic_pointer_cast<Implication>(expr)})
        return shapeOf("Impl", {e->getLeft(), e->getRight()});
    if (const auto e{std::dynamic_pointer_cast<Equivalence>(expr)})
        return shapeOf("Equiv", {e->getLeft(), e->getRight()});
    if (const auto e{std::dynamic_pointer_cast<Until>(expr)})
        return shapeOf("Until", {e->getLeft(), e->getRight()});
    if (const auto e{std::dynamic_pointer_cast<Since>(expr)})
        return shapeOf("Since", {e->getLeft(), e->getRight()});
    if (const auto e{std::dynamic_pointer_cast<Not>(expr)})
        return shapeOf("Not", {e->getLeft()});
    if (const auto e{std::dynamic_pointer_cast<NextStrong>(expr)})
        return shapeOf("X", {e->getLeft()});
    if (const auto e{std::dynamic_pointer_cast<Future>(expr)})
        return shapeOf("F", {e->getLeft()});
    if (const auto e{std::dynamic_pointer_cast<Globally>(expr)})
        return shapeOf("G", {e->getLeft()});
    if (const auto e{std::dynamic_pointer_cast<Once>(expr)})
        return shapeOf("O", {e->getLeft()});
    if (const auto e{std::dynamic_pointer_cast<Previously>(expr)})
        return shapeOf("P", {e->getLeft()});
    if (const auto e{std::dynamic_pointer_cast<Historically>(expr)})
        return shapeOf("H", {e->getLeft()});
    if (const auto e{std::dynamic_pointer_cast<UniversalQuantifier>(expr)})
        return shapeOf("A", {e->getLeft()});
    if (const auto e{std::dynamic_pointer_cast<ExistentialQuantifier>(expr)})
        return shapeOf("E", {e->getLeft()});
    return expr->to_string(); // atomic propositions and predicates
}

std::string parseShape(const std::string &formula) { return shape(Parser::parse(formula).getExpr()); }

} // namespace

// Negation binds to the next unary expression. Before, `!` followed by anything but an atomic proposition, a boolean
// or a bracketed expression negated the remainder of the formula, e.g., `!lo(x) & X(hi(x))` was `!(lo(x) & X(hi(x)))`.
TEST(TestParser, negationBindsToNextUnaryExpression) {
    // negated predicate with arguments
    EXPECT_EQ(parseShape("!lo(x) & X(hi(x))"), "And(Not(lo(x)), X(hi(x)))");
    EXPECT_EQ(parseShape("!lo(x) | hi(x)"), "Or(Not(lo(x)), hi(x))");
    EXPECT_EQ(parseShape("!lo(x) -> hi(x)"), "Impl(Not(lo(x)), hi(x))");
    EXPECT_EQ(parseShape("!lo(x) <-> hi(x)"), "Equiv(Not(lo(x)), hi(x))");
    EXPECT_EQ(parseShape("!lo(x) U hi(x)"), "Until(Not(lo(x)), hi(x))");
    EXPECT_EQ(parseShape("!lo(x) S hi(x)"), "Since(Not(lo(x)), hi(x))");
    EXPECT_EQ(parseShape("p & !lo(x) & q"), "And(Not(lo(x)), p, q)");
    EXPECT_EQ(parseShape("!lo('c') & p"), "And(Not(lo('c')), p)");
    EXPECT_EQ(parseShape("!lo(x, y) & p"), "And(Not(lo(x, y)), p)");

    // negated unary temporal operators, with and without interval
    EXPECT_EQ(parseShape("!X(q) & p"), "And(Not(X(q)), p)");
    EXPECT_EQ(parseShape("!X[0, 4](q) & p"), "And(Not(X(q)), p)");
    EXPECT_EQ(parseShape("!F(q) & p"), "And(Not(F(q)), p)");
    EXPECT_EQ(parseShape("!F[1, 2](q) & p"), "And(Not(F(q)), p)");
    EXPECT_EQ(parseShape("!G(q) & p"), "And(Not(G(q)), p)");
    EXPECT_EQ(parseShape("!G[0, inf](q) & p"), "And(Not(G(q)), p)");
    EXPECT_EQ(parseShape("!P(q) & p"), "And(Not(P(q)), p)");
    EXPECT_EQ(parseShape("!P[0, 2](q) & p"), "And(Not(P(q)), p)");
    EXPECT_EQ(parseShape("!O(q) & p"), "And(Not(O(q)), p)");
    EXPECT_EQ(parseShape("!O[0, 2](q) & p"), "And(Not(O(q)), p)");
    EXPECT_EQ(parseShape("!H(q) & p"), "And(Not(H(q)), p)");
    EXPECT_EQ(parseShape("!H[0, 2](q) & p"), "And(Not(H(q)), p)");
    EXPECT_EQ(parseShape("!X(q) -> p"), "Impl(Not(X(q)), p)");
    EXPECT_EQ(parseShape("!X(q) U p"), "Until(Not(X(q)), p)");

    // double negation
    EXPECT_EQ(parseShape("!!lo(x) & p"), "And(Not(Not(lo(x))), p)");
    EXPECT_EQ(parseShape("!!p & q"), "And(Not(Not(p)), q)");
    EXPECT_EQ(parseShape("!!X(q) & p"), "And(Not(Not(X(q))), p)");
    EXPECT_EQ(parseShape("!(!lo(x)) & p"), "And(Not(Not(lo(x))), p)");

    // forms that already parsed correctly
    EXPECT_EQ(parseShape("!p & q"), "And(Not(p), q)");
    EXPECT_EQ(parseShape("(!p) & q"), "And(Not(p), q)");
    EXPECT_EQ(parseShape("!(p) & q"), "And(Not(p), q)");
    EXPECT_EQ(parseShape("!(p | q) & r"), "And(Not(Or(p, q)), r)");
    EXPECT_EQ(parseShape("!(lo(x) & hi(x)) & p"), "And(Not(And(hi(x), lo(x))), p)");
    EXPECT_EQ(parseShape("!true & p"), "And(Not(true), p)");

    // negation inside temporal operators
    EXPECT_EQ(parseShape("G(!lo(x) & X(hi(x)))"), "G(And(Not(lo(x)), X(hi(x))))");
    EXPECT_EQ(parseShape("X(!lo(x) & p)"), "X(And(Not(lo(x)), p))");
}

TEST(TestParser, negationToString) {
    EXPECT_EQ(Parser::parse("!lo(x) & X(hi(x))").to_string(), Parser::parse("(!lo(x)) & (X(hi(x)))").to_string());
    EXPECT_EQ(Parser::parse("!lo(x) & X(hi(x))").to_string(), Parser::parse("(!(lo(x))) & (X(hi(x)))").to_string());
    EXPECT_EQ(Parser::parse("!X(q) & p").to_string(), Parser::parse("(!(X(q))) & p").to_string());
    EXPECT_EQ(Parser::parse("!X(q) & p").to_string(), "(!(X(q)) & p)");
    EXPECT_EQ(Parser::parse("!p & q").to_string(), Parser::parse("(!p) & q").to_string());
    EXPECT_EQ(Parser::parse("!(p | q) & r").to_string(), Parser::parse("(!(p | q)) & r").to_string());
    // the negation must not contain the conjunction
    EXPECT_NE(Parser::parse("!lo(x) & X(hi(x))").to_string(), Parser::parse("!(lo(x) & X(hi(x)))").to_string());
    EXPECT_NE(Parser::parse("!X(q) & p").to_string(), Parser::parse("!(X(q) & p)").to_string());
}

// The scope of a unary temporal operator is its (bracketed) operand and not the rest of the formula.
TEST(TestParser, temporalOperatorsBindToOperand) {
    for (const std::string op : {"X", "F", "G", "P", "O", "H"}) {
        EXPECT_EQ(parseShape(op + "(a) & b"), "And(" + shape(Parser::parse(op + "(a)").getExpr()) + ", b)") << op;
        EXPECT_EQ(parseShape(op + "[0, 4](a) & b"), "And(" + shape(Parser::parse(op + "(a)").getExpr()) + ", b)") << op;
        EXPECT_EQ(parseShape(op + "[0, 4] a & b"), "And(" + shape(Parser::parse(op + "(a)").getExpr()) + ", b)") << op;
        EXPECT_EQ(parseShape(op + " a | b"), "Or(" + shape(Parser::parse(op + "(a)").getExpr()) + ", b)") << op;
        EXPECT_EQ(parseShape(op + "(a) -> b"), "Impl(" + shape(Parser::parse(op + "(a)").getExpr()) + ", b)") << op;
    }
    EXPECT_EQ(parseShape("X[0, 4] p & q"), "And(X(p), q)");
    EXPECT_EQ(parseShape("X[0, 4] p & q").find("X(And"), std::string::npos);
}

// Precedence of the binary operators: & binds tighter than |, | tighter than ->, -> tighter than <->; the temporal
// binary operators U, R, S bind tighter than &; -> and <-> are right-associative.
TEST(TestParser, binaryOperatorPrecedence) {
    EXPECT_EQ(parseShape("a | b & c"), "Or(And(b, c), a)");
    EXPECT_EQ(parseShape("a & b | c"), "Or(And(a, b), c)");
    EXPECT_EQ(parseShape("a & b -> c | d"), "Impl(And(a, b), Or(c, d))");
    EXPECT_EQ(parseShape("a -> b -> c"), "Impl(a, Impl(b, c))");
    EXPECT_EQ(parseShape("a <-> b <-> c"), "Equiv(a, Equiv(b, c))");
    EXPECT_EQ(parseShape("a -> b <-> c"), "Equiv(Impl(a, b), c)");
    EXPECT_EQ(parseShape("a <-> b -> c"), "Equiv(a, Impl(b, c))");
    EXPECT_EQ(parseShape("a U b & c"), "And(Until(a, b), c)");
    EXPECT_EQ(parseShape("a & b S c"), "And(Since(b, c), a)");
    EXPECT_EQ(parseShape("a U[0, 3] b | c"), "Or(Until(a, b), c)");
    EXPECT_EQ(parseShape("!a U b"), "Until(Not(a), b)");
    EXPECT_EQ(parseShape("a U !b"), "Until(a, Not(b))");
}

// The body of a quantifier extends as far to the right as possible, also below a negation.
TEST(TestParser, quantifierScope) {
    EXPECT_EQ(parseShape("A x in D. p(x) & q"), "A(And(p(x), q))");
    EXPECT_EQ(parseShape("E x in D. p(x) | q"), "E(Or(p(x), q))");
    EXPECT_EQ(parseShape("!(E x in D. p(x)) & q"), "And(Not(E(p(x))), q)");
    EXPECT_EQ(parseShape("!E x in D. p(x) & q"), "Not(E(And(p(x), q)))");
    EXPECT_EQ(parseShape("(E x in D. p(x)) & !q(y)"), "And(E(p(x)), Not(q(y)))");
    EXPECT_EQ(parseShape("A x in D. !p(x) & X(q(x))"), "A(And(Not(p(x)), X(q(x))))");
}

namespace {

bool lowAlways(size_t /*timeStep*/, size_t /*val*/) { return false; }
bool highAlways(size_t /*timeStep*/, size_t /*val*/) { return true; }

} // namespace

// G(A x in N . (!lo(x) & X(hi(x)))) was parsed as G(A x in N . !(X(hi(x)) & lo(x))), which is true at the last step
// because the strong next X(hi(x)) is false there. With the correct parse, !lo(x) holds and X(hi(x)) fails at the last
// step, so the verdict at the last step is false. (Hydra2 reports the verdict of the formula below G at every step.)
TEST(TestParser, negationVerdictQuantifiedPredicate) {
    const auto functions{std::vector<std::shared_ptr<IFunction<bool>>>{
        std::make_shared<Function<bool, size_t, size_t>>(
            Function<bool, size_t, size_t>("lo", {"timeStep", "val"}, lowAlways, hydraTestUtils::twoSizeTArgs)),
        std::make_shared<Function<bool, size_t, size_t>>(
            Function<bool, size_t, size_t>("hi", {"timeStep", "val"}, highAlways, hydraTestUtils::twoSizeTArgs))}};
    const std::shared_ptr<Trace> trace{hydraTestUtils::createTraceWithVariable(3, functions)};
    const std::vector<std::shared_ptr<DomainStorage>> domains{std::make_shared<DomainStorage>(DomainStorage(
        "N", {size_t{1}, size_t{2}}, [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); }))};

    Formula globallyQuantified = Parser::parse("G(A x in N . (!lo(x) & X(hi(x))))");
    globallyQuantified.setId("R1");
    hydraTestUtils::executeHydraEval(trace, {globallyQuantified}, {{{0, true}, {1, true}, {2, false}}}, domains);
}

TEST(TestParser, negationVerdictNext) {
    // p and q never hold: !X(q) & p is false everywhere, hence the verdict is false at every step. Before the fix it
    // was !(X(q) & p), i.e., true everywhere.
    const std::shared_ptr<Trace> trace{
        Utils::createTrace({"p", "q"}, std::vector<std::vector<std::string>>{{}, {}}, {0, 1})};
    Formula formulaGlobally = Parser::parse("G(!X(q) & p)");
    formulaGlobally.setId("R1");
    hydraTestUtils::executeHydraEval(trace, {formulaGlobally}, {{{0, false}, {1, false}}});
}
