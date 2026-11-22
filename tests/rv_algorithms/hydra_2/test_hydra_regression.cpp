#include "hydra_test_utils.h"
#include "monitors/collections/domain.h"
#include "monitors/parser/parser.h"
#include "monitors/rewriting/hydra_rewrite_visitor.h"

#include <gtest/gtest.h>

// Explicit regression tests for the bugs found by the randomized differential tests (test_hydra_differential.cpp).
// Each test is evaluated with all combinations of connected evaluation, reordering, and short-circuiting, and twice
// per monitor so that resetting is covered as well.

namespace {

void expectVerdicts(const std::string &formulaString, const std::shared_ptr<Trace> &trace,
                    const std::vector<Verdict> &expected,
                    const std::vector<std::shared_ptr<DomainStorage>> &domains = {}) {
    Formula formula = Parser::parse(formulaString);
    formula.setId("R1");
    SCOPED_TRACE(formulaString);
    hydraTestUtils::executeHydraEval(trace, {formula}, {expected}, domains);
}

std::vector<std::shared_ptr<DomainStorage>> atomDomain() {
    return {std::make_shared<DomainStorage>(
        DomainStorage("D", {"a", "b"}, [](const std::any &val) { return std::any_cast<const char *>(val); }))};
}

std::vector<std::shared_ptr<DomainStorage>> threeAtomDomain() {
    return {std::make_shared<DomainStorage>(
        DomainStorage("D", {"a", "b", "c"}, [](const std::any &val) { return std::any_cast<const char *>(val); }))};
}

} // namespace

// ---------------------------------------------------------------------------------------------------------------------
// Temporal monitors
// ---------------------------------------------------------------------------------------------------------------------

// O/H/S with a lower bound > 0 ignored the bound if they were first evaluated at the last position.
TEST(TestHydra2Regression, onceLowerBoundAtLastPosition) {
    expectVerdicts("G(O[1, 2](a))", Utils::createTrace({"a"}, {{"a"}}, {0}), {{0, false}});
    expectVerdicts("G(b & O[1, 2](a))", Utils::createTrace({"a", "b"}, {{}, {"a", "b"}}, {0, 1}),
                   {{0, false}, {1, false}});
}

TEST(TestHydra2Regression, historicallyLowerBoundAtLastPosition) {
    expectVerdicts("G(H[1, 2](a))", Utils::createTrace({"a"}, {{}}, {0}), {{0, true}});
    expectVerdicts("G(b & H[1, 2](a))", Utils::createTrace({"a", "b"}, {{"a"}, {"b"}}, {0, 1}),
                   {{0, false}, {1, true}});
}

TEST(TestHydra2Regression, sinceLowerBoundAtLastPosition) {
    expectVerdicts("G(b S[1, 2] a)", Utils::createTrace({"a", "b"}, {{"a"}}, {0}), {{0, false}});
}

// X[l, inf] compared the lower bound with the absolute time stamp instead of the time difference.
TEST(TestHydra2Regression, nextUnboundedIntervalUsesTimeDifference) {
    expectVerdicts("G(X[2, inf](a))", Utils::createTrace({"a"}, {{}, {"a"}}, {10, 11}), {{10, false}, {11, false}});
    expectVerdicts("G(X[2, inf](a))", Utils::createTrace({"a"}, {{}, {"a"}}, {0, 5}), {{0, true}, {5, false}});
}

// X at the last position returned true if its sub-formula is a past formula.
TEST(TestHydra2Regression, nextAtLastPositionOverPastFormula) {
    expectVerdicts("G(X(O(a)))", Utils::createTrace({"a"}, {{"a"}, {"a"}}, {0, 1}), {{0, true}, {1, false}});
    expectVerdicts("G(X(H(a)))", Utils::createTrace({"a"}, {{"a"}, {"a"}}, {0, 1}), {{0, true}, {1, false}});
}

// X at the last position returned true over a negated sub-formula.
TEST(TestHydra2Regression, nextAtLastPositionOverNegation) {
    expectVerdicts("G(X(!(X[0, 2](a))))", Utils::createTrace({"a"}, {{}}, {0}), {{0, false}});
}

// X[l, inf] held for equal time stamps although the time difference 0 is below the lower bound.
TEST(TestHydra2Regression, nextUnboundedIntervalWithEqualTimeStamps) {
    expectVerdicts("G(X[1, inf](a))", Utils::createTrace({"a"}, {{}, {"a"}}, {4, 4}), {{4, false}, {4, false}});
}

// Short-circuiting skipped steps of the temporal monitors, which left their head positions in a wrong state.
TEST(TestHydra2Regression, previousAfterSkippedSteps) {
    expectVerdicts("G(q -> P[0, 3](G(r)))", Utils::createTrace({"q", "r"}, {{}, {"q"}, {"q", "r"}}, {1, 2, 3}),
                   {{1, true}, {2, false}, {3, false}});
}

TEST(TestHydra2Regression, nextAfterSkippedSteps) {
    expectVerdicts("G(q & X[2, 3](r))", Utils::createTrace({"q", "r"}, {{"q"}, {}, {"q"}, {"r"}}, {3, 6, 9, 9}),
                   {{3, false}, {6, false}, {9, false}, {9, false}});
    expectVerdicts("G(X[0, 3](q) | (p S r))",
                   Utils::createTrace({"p", "q", "r"}, {{}, {"r"}, {}, {"q"}}, {11, 11, 13, 15}),
                   {{11, false}, {11, true}, {13, true}, {15, false}});
}

TEST(TestHydra2Regression, sinceAfterSkippedSteps) {
    expectVerdicts("G(q & (q S p))", Utils::createTrace({"p", "q"}, {{"p"}, {}, {}, {"q"}}, {5, 6, 8, 11}),
                   {{5, false}, {6, false}, {8, false}, {11, false}});
    expectVerdicts("G(q -> O[1, 1](p))", Utils::createTrace({"p", "q"}, {{}, {"p", "q"}}, {6, 7}),
                   {{6, true}, {7, false}});
}

// Quantifiers only evaluate the sub-monitors of some combinations at a step, so the until, since, and next monitors of
// the other combinations skip steps.
TEST(TestHydra2Regression, quantifiedUntilAfterSkippedSteps) {
    expectVerdicts("G(E x in D. (x U[2, 5] true))",
                   Utils::createTrace({"a", "b", "c"}, {{"a", "b"}, {"a", "c"}, {"b", "c"}, {"b", "c"}}, {1, 2, 3, 4}),
                   {{1, true}, {2, true}, {3, false}, {4, false}}, threeAtomDomain());
}

TEST(TestHydra2Regression, quantifiedPreviousAfterSkippedSteps) {
    expectVerdicts("G(Q[0.0, 0.4] x in D. (P[0, 2](r) & x))",
                   Utils::createTrace({"a", "b", "c", "r"}, {{"b", "c", "r"}, {"b", "c"}}, {12, 14}),
                   {{12, true}, {14, false}}, threeAtomDomain());
}

// Verdict && threw since the next monitor returned the time stamp of a previous step after skipped steps.
TEST(TestHydra2Regression, quantifiedNextAfterSkippedSteps) {
    expectVerdicts("G(E x, y in D. ((X[0, 4](p) & x) | y))",
                   Utils::createTrace({"a", "b", "c", "p"}, {{"b", "c"}, {"a", "b"}, {"b", "c"}}, {3, 4, 5}),
                   {{3, true}, {4, true}, {5, true}}, threeAtomDomain());
}

// The until monitor read the value of empty optionals (undefined behavior, non-deterministic verdicts).
TEST(TestHydra2Regression, untilWithoutUninitializedReads) {
    expectVerdicts("G(C=2 u, v in D. (v U u))", Utils::createTrace({"a", "b", "c"}, {{}, {"c"}}, {5, 6}),
                   {{5, false}, {6, false}}, threeAtomDomain());
}

// In mode globally_early_termination, the short-circuited O[1, 1] ignored its lower bound at the violating step.
TEST(TestHydra2Regression, earlyTerminationAfterSkippedSteps) {
    const std::shared_ptr<Trace> trace = Utils::createTrace({"p", "q"}, {{}, {"p", "q"}}, {6, 7});
    Formula formula = Parser::parse("G(q -> O[1, 1](p))");
    formula.setId("R1");
    for (const bool connectedEval : {false, true})
        for (const bool reordering : {false, true})
            for (const bool shortCircuiting : {false, true}) {
                auto monitor{Hydra2({formula}, connectedEval, reordering, shortCircuiting, globally_early_termination)};
                for (size_t run{0}; run < 2; ++run)
                    EXPECT_EQ(monitor.evaluate(trace).at("R1"), (std::vector<Verdict>{{7, false}}));
            }
}

// In mode initial_step, the short-circuited S[2, 5] ignored its lower bound.
TEST(TestHydra2Regression, initialStepAfterSkippedSteps) {
    const std::shared_ptr<Trace> trace = Utils::createTrace({"p", "q"}, {{"p"}, {"p"}, {"q"}}, {1, 2, 3});
    Formula formula = Parser::parse("F((q S[2, 5] p) & !p)");
    formula.setId("R1");
    for (const bool connectedEval : {false, true})
        for (const bool reordering : {false, true})
            for (const bool shortCircuiting : {false, true}) {
                auto monitor{Hydra2({formula}, connectedEval, reordering, shortCircuiting, initial_step)};
                for (size_t run{0}; run < 2; ++run)
                    EXPECT_FALSE(monitor.evaluate(trace).at("R1").at(0).value);
            }
}

// ---------------------------------------------------------------------------------------------------------------------
// Satisfaction probability
// ---------------------------------------------------------------------------------------------------------------------

// For a right-hand side probability of 0.32, rounding made the satisfaction probability of `true U right` and
// `true S right` 1.0000000000000004, which triggered the assertion in the Hydra2Monitor constructor (debug builds).
TEST(TestHydra2Regression, satisfactionProbabilityIsClamped) {
    const std::shared_ptr<Trace> trace = hydraTestUtils::createTraceWithVariable(
        3,
        {std::make_shared<Function<bool, size_t, size_t>>(
            Function<bool, size_t, size_t>("equal_to_time_step", {"timeStep", "val"},
                                           hydraTestUtils::equal_to_time_step, hydraTestUtils::twoSizeTArgs))},
        ConstantCollection({ConstantValue{
            "one", 1ul, [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); }}}));
    const std::map<std::string, std::array<double, 3>> costs{{"equal_to_time_step", {1.0, 0.32, 0.0}}};
    std::vector<Formula> formulas{Parser::parse("G(F(equal_to_time_step('one')))"),
                                  Parser::parse("G(O(equal_to_time_step('one')))")};
    formulas[0].setId("R1");
    formulas[1].setId("R2");
    for (const bool connectedEval : {false, true})
        for (const bool reordering : {false, true})
            for (const bool shortCircuiting : {false, true}) {
                auto monitor{Hydra2(formulas, connectedEval, reordering, shortCircuiting, globally, {}, costs)};
                const auto result{monitor.evaluate(trace)};
                EXPECT_EQ(result.at("R1"), (std::vector<Verdict>{{0, true}, {1, true}, {2, false}}));
                EXPECT_EQ(result.at("R2"), (std::vector<Verdict>{{0, false}, {1, true}, {2, true}}));
            }
}

// ---------------------------------------------------------------------------------------------------------------------
// Identical root formulas
// ---------------------------------------------------------------------------------------------------------------------

// With connected evaluation, identical root formulas share a monitor and the formula ID of the first formula was
// overwritten by the one of the second formula.
TEST(TestHydra2Regression, identicalFormulasWithConnectedEvaluation) {
    const std::shared_ptr<Trace> trace = Utils::createTrace({"a"}, {{"a"}, {}}, {0, 1});
    std::vector<Formula> formulas{Parser::parse("G(a)"), Parser::parse("G(a)"), Parser::parse("G(!a)")};
    formulas[0].setId("R1");
    formulas[1].setId("R2");
    formulas[2].setId("R3");
    for (const auto mode : {globally, globally_early_termination}) {
        auto monitor{Hydra2(formulas, true, false, false, mode)};
        const auto result{monitor.evaluate(trace)};
        EXPECT_EQ(result.size(), 3);
        EXPECT_EQ(result.count("R1"), 1);
        EXPECT_EQ(result.count("R2"), 1);
        EXPECT_EQ(result.count("R3"), 1);
    }
    hydraTestUtils::executeHydraEval(trace, formulas,
                                     {{{0, true}, {1, false}}, {{0, true}, {1, false}}, {{0, false}, {1, true}}});
}

// ---------------------------------------------------------------------------------------------------------------------
// Rewrite rules (HydraRewriteVisitor)
// ---------------------------------------------------------------------------------------------------------------------

// E x. !φ was rewritten to !E x. φ.
TEST(TestHydra2Regression, existentialQuantifierNotSwappedWithNegation) {
    expectVerdicts("G(E x in D. !x)", Utils::createTrace({"a", "b"}, {{"a"}, {"a", "b"}}, {0, 1}),
                   {{0, true}, {1, false}}, atomDomain());
}

// E x in D. O(φ) was rewritten to O(E x in D. φ) even if D depends on the time step. Then D is evaluated at an earlier
// time step (preliminaries: the domain of a quantifier is the one at the time step where it is evaluated).
TEST(TestHydra2Regression, existentialQuantifierNotSwappedWithOnceForDynamicDomain) {
    // D = {a} at time step 0 and {b} afterwards
    auto domainFunction{std::make_shared<Function<std::vector<std::any>, size_t>>(
        Function<std::vector<std::any>, size_t>(
            "dynDomain", {"stepNumber"},
            [](const size_t step) { return step == 0 ? std::vector<std::any>{"a"} : std::vector<std::any>{"b"}; },
            [](const std::vector<DomainValue> &args) {
                return std::tuple<size_t>{std::any_cast<size_t>(args.at(0).getValue())};
            }))};
    const std::vector domains{std::make_shared<DomainStorage>(
        DomainStorage("D", domainFunction, [](const std::any &val) { return std::any_cast<const char *>(val); },
                      [](size_t, const std::any &) { return true; }))};
    Formula formula = Parser::parse("G(E x in D. O(x))");
    formula.setId("R1");
    const auto trace{Utils::createTrace({"a", "b"}, {{"a"}, {}}, {0, 1})};
    // At step 1, x ranges over D(1) = {b}, and b never held, so the formula is violated.
    for (const bool reordering : {false, true}) {
        // the domain is known when the monitor is created, so the rewriter sees that it is dynamic
        auto monitor{Hydra2({formula}, true, reordering, false, globally, domains)};
        const auto result{monitor.evaluate(trace, domains).at("R1")};
        ASSERT_EQ(result.size(), 2);
        EXPECT_TRUE(result.at(0).value);
        EXPECT_FALSE(result.at(1).value);
    }
}

// E x in D. O(φ) is still swapped for constant domains known at creation.
TEST(TestHydra2Regression, existentialQuantifierSwappedWithOnceForConstantDomain) {
    Formula formula = Parser::parse("E x in D. O(x)");
    HydraRewriteVisitor rwv{atomDomain()};
    rwv.rewrite(formula);
    EXPECT_EQ(formula.getExpr()->to_string(), "(true S E x in D. x)");
    Formula formula2 = Parser::parse("E x in D. O(x)");
    HydraRewriteVisitor rwvUnknown;
    rwvUnknown.rewrite(formula2);
    EXPECT_EQ(formula2.getExpr()->to_string(), "E x in D. (true S x)");
}

// C=1 x. !φ was rewritten to !C=1 x. φ and C=1 x. F φ to F C=1 x. φ.
TEST(TestHydra2Regression, countingQuantifierNotSwappedWithUnaryOperator) {
    expectVerdicts("G(C=1 x in D. !x)", Utils::createTrace({"a", "b"}, {{"a"}}, {0}), {{0, true}}, atomDomain());
    expectVerdicts("G(C=1 x in D. F(x))", Utils::createTrace({"a", "b"}, {{"a"}, {"b"}}, {0, 1}),
                   {{0, false}, {1, true}}, atomDomain());
}

// Q[0.0, 0.4] x. !φ was rewritten to !Q[0.0, 0.4] x. φ.
TEST(TestHydra2Regression, quoteQuantifierNotSwappedWithUnaryOperator) {
    expectVerdicts("G(Q[0.0, 0.4] x in D. !x)", Utils::createTrace({"a", "b"}, {{"a"}}, {0}), {{0, false}},
                   atomDomain());
}

// E x. G φ was rewritten to G E x. φ.
TEST(TestHydra2Regression, existentialQuantifierNotSwappedWithGlobally) {
    expectVerdicts("G(E x in D. G(x))", Utils::createTrace({"a", "b"}, {{"a"}, {"b"}}, {0, 1}), {{0, false}, {1, true}},
                   atomDomain());
}

// E x. H φ was rewritten to H E x. φ.
TEST(TestHydra2Regression, existentialQuantifierNotSwappedWithHistorically) {
    expectVerdicts("G(E x in D. H(x))", Utils::createTrace({"a", "b"}, {{"a"}, {"b"}}, {0, 1}), {{0, true}, {1, false}},
                   atomDomain());
}

// φ U_I φ was rewritten to φ even if I does not contain 0.
TEST(TestHydra2Regression, untilWithIdenticalOperandsAndLowerBound) {
    expectVerdicts("G(a U[1, 2] a)", Utils::createTrace({"a"}, {{"a"}, {}}, {0, 1}), {{0, false}, {1, false}});
}

// φ S_I φ was rewritten to φ even if I does not contain 0.
TEST(TestHydra2Regression, sinceWithIdenticalOperandsAndLowerBound) {
    expectVerdicts("G(a S[1, 2] a)", Utils::createTrace({"a"}, {{}, {"a"}}, {0, 1}), {{0, false}, {1, false}});
}

// G_I(false) was rewritten to false even if I does not contain 0 (it holds if no step lies within I).
TEST(TestHydra2Regression, globallyFalseWithLowerBound) {
    expectVerdicts("G(G[1, 2](false))", Utils::createTrace({"a"}, {{}, {}}, {0, 5}), {{0, true}, {5, true}});
}

// H_I(false) was rewritten to false even if I does not contain 0 (it holds if no step lies within I).
TEST(TestHydra2Regression, historicallyFalseWithLowerBound) {
    expectVerdicts("G(H[1, 2](false))", Utils::createTrace({"a"}, {{}, {}}, {0, 5}), {{0, true}, {5, true}});
}

// Idempotence F(F(φ)) = F(φ), G(G(φ)) = G(φ), O(O(φ)) = O(φ), and H(H(φ)) = H(φ) was applied to bounded operators.
// Note: these rules do not fire at the moment since the sub-formulas are rewritten to U/S before, so this test and the
// next one guard against the unsound rules becoming active, e.g., if the rewriting order changes.
TEST(TestHydra2Regression, idempotenceOnlyWithoutInterval) {
    expectVerdicts("G(F[0, 1](F[0, 1](a)))", Utils::createTrace({"a"}, {{}, {}, {"a"}}, {0, 1, 2}),
                   {{0, true}, {1, true}, {2, true}});
    expectVerdicts("G(G[0, 1](G[0, 1](a)))", Utils::createTrace({"a"}, {{"a"}, {"a"}, {}}, {0, 1, 2}),
                   {{0, false}, {1, false}, {2, false}});
    expectVerdicts("G(O[0, 1](O[0, 1](a)))", Utils::createTrace({"a"}, {{"a"}, {}, {}}, {0, 1, 2}),
                   {{0, true}, {1, true}, {2, true}});
    expectVerdicts("G(H[0, 1](H[0, 1](a)))", Utils::createTrace({"a"}, {{}, {"a"}, {"a"}}, {0, 1, 2}),
                   {{0, false}, {1, false}, {2, false}});
}

// Absorption F(G(F(φ))) = G(F(φ)), G(F(G(φ))) = F(G(φ)), and H(O(H(φ))) = O(H(φ)) was applied to bounded operators.
TEST(TestHydra2Regression, absorptionOnlyWithoutInterval) {
    expectVerdicts("G(F(G[0, 1](F[0, 1](a))))", Utils::createTrace({"a"}, {{}, {}, {}, {"a"}}, {0, 1, 2, 3}),
                   {{0, true}, {1, true}, {2, true}, {3, true}});
    expectVerdicts("G(G(F[0, 1](G[0, 1](a))))", Utils::createTrace({"a"}, {{"a"}, {"a"}, {}, {}}, {0, 1, 2, 3}),
                   {{0, false}, {1, false}, {2, false}, {3, false}});
    expectVerdicts("G(H(O[0, 1](H[0, 1](a))))", Utils::createTrace({"a"}, {{}, {}, {"a"}, {"a"}}, {0, 1, 2, 3}),
                   {{0, false}, {1, false}, {2, false}, {3, false}});
}

// C=1 x. C=1 y. φ was merged to C=1 x, y. φ.
TEST(TestHydra2Regression, nestedCountingQuantifiersNotMerged) {
    expectVerdicts("G(C=1 x in D. C=1 y in D. (x | y))", Utils::createTrace({"a", "b"}, {{"a"}}, {0}), {{0, true}},
                   atomDomain());
}

// Unused variables of counting quantifiers were removed, e.g., C=1 x. a was rewritten to a.
TEST(TestHydra2Regression, unusedVariableOfCountingQuantifierNotRemoved) {
    expectVerdicts("G(C=1 x in D. a)", Utils::createTrace({"a", "b"}, {{"a"}, {}}, {0, 1}), {{0, false}, {1, false}},
                   atomDomain());
    expectVerdicts("G(C=2 x in D. a)", Utils::createTrace({"a", "b"}, {{"a"}, {}}, {0, 1}), {{0, true}, {1, false}},
                   atomDomain());
}

// Q[0.5, 1.0] x. Q[0.5, 1.0] y. φ was merged to Q[0.5, 1.0] x, y. φ.
TEST(TestHydra2Regression, nestedQuoteQuantifiersNotMerged) {
    expectVerdicts("G(Q[0.5, 1.0] x in D. Q[0.5, 1.0] y in D. (x & y))", Utils::createTrace({"a", "b"}, {{"a"}}, {0}),
                   {{0, true}}, atomDomain());
}

// All unused variables of quote quantifiers were removed, so that the quantifier evaluated to its sub-formula.
TEST(TestHydra2Regression, quoteQuantifierKeepsLastVariable) {
    expectVerdicts("G(Q[0.0, 0.5] x in D. a)", Utils::createTrace({"a", "b"}, {{}, {"a"}}, {0, 1}),
                   {{0, true}, {1, false}}, atomDomain());
}

// ---------------------------------------------------------------------------------------------------------------------
// Evaluation modes
// ---------------------------------------------------------------------------------------------------------------------

// In mode initial_step, the evaluation returned after the first formula, so the other formulas kept the placeholder
// verdict true.
TEST(TestHydra2Regression, initialStepEvaluatesAllFormulas) {
    const std::shared_ptr<Trace> trace = Utils::createTrace({"a", "b"}, {{"a"}, {"b"}}, {0, 1});
    std::vector<Formula> formulas{Parser::parse("a"), Parser::parse("b"), Parser::parse("F(b)"),
                                  Parser::parse("a & b")};
    formulas[0].setId("R1");
    formulas[1].setId("R2");
    formulas[2].setId("R3");
    formulas[3].setId("R4");
    for (const bool connectedEval : {false, true})
        for (const bool reordering : {false, true})
            for (const bool shortCircuiting : {false, true}) {
                auto monitor{Hydra2(formulas, connectedEval, reordering, shortCircuiting, initial_step)};
                for (size_t run{0}; run < 2; ++run) {
                    const auto result{monitor.evaluate(trace)};
                    ASSERT_EQ(result.size(), 4);
                    EXPECT_EQ(result.at("R1"), (std::vector<Verdict>{{0, true}}));
                    EXPECT_EQ(result.at("R2"), (std::vector<Verdict>{{0, false}}));
                    EXPECT_EQ(result.at("R3"), (std::vector<Verdict>{{0, true}}));
                    EXPECT_EQ(result.at("R4"), (std::vector<Verdict>{{0, false}}));
                }
                const auto filtered{monitor.evaluate(trace, {}, {"R2", "R4"})};
                ASSERT_EQ(filtered.size(), 2);
                EXPECT_EQ(filtered.at("R2"), (std::vector<Verdict>{{0, false}}));
                EXPECT_EQ(filtered.at("R4"), (std::vector<Verdict>{{0, false}}));
            }
}
