#include "test_hydra_creation.h"

#include "monitors/collections/domain.h"
#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra_2/hydra_creation_visitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/counting_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/existential_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/hydra_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/predicate_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/quote_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/universal_monitor.h"

#include <cmath>

void executeTest(Formula inputFormula, const std::string &expectedFormulaStringReordered,
                 const std::map<std::string, std::array<double, 3>> &predicateCosts,
                 const std::vector<std::shared_ptr<DomainStorage>> &domains = {}) {
    HydraCreationVisitor2 hcv{HydraCreationVisitor2(predicateCosts, false, true, domains)};
    std::shared_ptr<Hydra2Monitor> mon = hcv.createMonitor(inputFormula, 0, false);
    EXPECT_EQ(mon->to_string(), inputFormula.to_string());

    hcv = HydraCreationVisitor2(predicateCosts, false, true, domains);
    mon = hcv.createMonitor(inputFormula, 0, true);
    EXPECT_EQ(mon->to_string(), inputFormula.to_string());

    hcv = HydraCreationVisitor2(predicateCosts, true, true, domains);
    mon = hcv.createMonitor(inputFormula, 0, false);
    EXPECT_EQ(mon->to_string(), expectedFormulaStringReordered);

    hcv = HydraCreationVisitor2(predicateCosts, true, true, domains);
    mon = hcv.createMonitor(inputFormula, 0, true);
    EXPECT_EQ(mon->to_string(), expectedFormulaStringReordered);
}

TEST(TestHydraCreation, testAnd) {
    Formula formula = Parser::parse("(d(test) & a(test) & b(test) & c(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(d(test) & a(test) & b(test) & c(test))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testAndReorder) {
    Formula formula = Parser::parse("(a(test) & b(test) & c(test) & d(test))");
    formula.setId("R1");
    const std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(d(test) & a(test) & b(test) & c(test))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testAndReorderPriorityEnforcement) {
    Formula formula = Parser::parse("(a(test) & b(test) & c(test) & d(test))");
    formula.setId("R1");
    const std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80, 2}}, {"b", {100.0, 0.20, 1}}, {"c", {100.0, 0.80, 4}}, {"d", {20.0, 0.20, 3}}};
    const std::string expectedFormula{"(c(test) & d(test) & a(test) & b(test))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testAndAndDuplicated) {
    Formula formula =
        Parser::parse("(!(a(test)) & P(b(test))) | X(!(a(test)) | (!(a(test)) & P(b(test))) | P(c(test)))");
    formula.setId("R1");
    const std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80, 0.0}}, {"b", {100.0, 0.20, 0.0}}, {"c", {100.0, 0.80, 0.0}}, {"d", {20.0, 0.20, 0.0}}};
    std::string expectedFormula{
        "(X((!(a(test)) | P(c(test)) | (!(a(test)) & P(b(test))))) | (!(a(test)) & P(b(test))))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testPreviousAnd) {
    Formula formula = Parser::parse("P(d(test)) & P(a(test)) & P(b(test)) & P(c(test))");
    formula.setId("R1");
    const std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(P(d(test)) & P(a(test)) & P(b(test)) & P(c(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testAndOrDuplicatedPrevious) {
    Formula formula = Parser::parse("P(a(test)) & (P(a(test)) | P(b(test)) | P(c(test)))");
    formula.setId("R1");
    const std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(P(a(test)) & (P(a(test)) | P(c(test)) | P(b(test))))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testPreviousAndReorder) {
    Formula formula = Parser::parse("P(a(test)) & P(b(test)) & P(c(test)) & P(d(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(P(d(test)) & P(a(test)) & P(b(test)) & P(c(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testNextAnd) {
    Formula formula = Parser::parse("X(d(test)) & X(a(test)) & X(b(test)) & X(c(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(X(d(test)) & X(a(test)) & X(b(test)) & X(c(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testNextAndReorder) {
    Formula formula = Parser::parse("X(a(test)) & X(b(test)) & X(c(test)) & X(d(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(X(d(test)) & X(a(test)) & X(b(test)) & X(c(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testAndOrDuplicatedNext) {
    Formula formula = Parser::parse("X(a(test)) & (X(a(test)) | X(b(test)) | X(c(test)))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(X(a(test)) & (X(a(test)) | X(c(test)) | X(b(test))))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testNotAnd) {
    Formula formula = Parser::parse("!(a(test)) & !(d(test)) & !(c(test)) & !(b(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(!(a(test)) & !(d(test)) & !(c(test)) & !(b(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testNotAndReorder) {
    Formula formula = Parser::parse("!(a(test)) & !(b(test)) & !(c(test)) & !(d(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(!(a(test)) & !(d(test)) & !(c(test)) & !(b(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testNotAndDuplicated) {
    Formula formula = Parser::parse("!(a(test)) & (!(a(test)) | P(b(test)) | P(c(test)))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(!(a(test)) & (!(a(test)) | P(c(test)) | P(b(test))))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testAndAnd) {
    Formula formula = Parser::parse("!(d(test) & a(test)) & !(b(test) & c(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(!((d(test) & a(test))) & !((b(test) & c(test))))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testAndAndReorder) {
    Formula formula = Parser::parse("!(b(test) & c(test)) & !(d(test) & a(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(!((d(test) & a(test))) & !((b(test) & c(test))))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testSinceAnd) {
    Formula formula = Parser::parse("(a(test))S(d(test)) & (c(test))S(b(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"((a(test) S d(test)) & (c(test) S b(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testSinceAndReorder) {
    Formula formula = Parser::parse("(c(test))S(b(test)) & (a(test))S(d(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"((a(test) S d(test)) & (c(test) S b(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testUntilAnd) {
    Formula formula = Parser::parse("(a(test))S(d(test)) & (c(test))S(b(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"((a(test) S d(test)) & (c(test) S b(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testUntilAndReorder) {
    Formula formula = Parser::parse("(c(test))U(b(test)) & (a(test))U(d(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"((a(test) U d(test)) & (c(test) U b(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testOr) {
    Formula formula = Parser::parse("a(test) | d(test) | c(test) | b(test)");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(a(test) | d(test) | c(test) | b(test))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testOrReorder) {
    Formula formula = Parser::parse("a(test) | b(test) | c(test) | d(test)");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(a(test) | d(test) | c(test) | b(test))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testOrReorderPriorityEnforcement) {
    Formula formula = Parser::parse("a(test) | b(test) | c(test) | d(test)");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80, 2}}, {"b", {100.0, 0.20, 4}}, {"c", {100.0, 0.80, 3}}, {"d", {20.0, 0.20, 1}}};
    std::string expectedFormula{"(b(test) | c(test) | a(test) | d(test))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testOrOrDuplicated) {
    Formula formula =
        Parser::parse("(!(a(test)) | P(b(test))) & X(!(a(test)) & (!(a(test)) | P(b(test))) & P(c(test)))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{
        "((!(a(test)) | P(b(test))) & X((!(a(test)) & (!(a(test)) | P(b(test))) & P(c(test)))))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testPreviousOr) {
    Formula formula = Parser::parse("P(a(test)) | P(d(test)) | P(c(test)) | P(b(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(P(a(test)) | P(d(test)) | P(c(test)) | P(b(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testPreviousOrReorder) {
    Formula formula = Parser::parse("P(a(test)) | P(b(test)) | P(c(test)) | P(d(test)))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(P(a(test)) | P(d(test)) | P(c(test)) | P(b(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testNextOr) {
    Formula formula = Parser::parse("X(a(test)) | X(d(test)) | X(c(test)) | X(b(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(X(a(test)) | X(d(test)) | X(c(test)) | X(b(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testNextOrReorder) {
    Formula formula = Parser::parse("X(a(test)) | X(b(test)) | X(c(test)) | X(d(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(X(a(test)) | X(d(test)) | X(c(test)) | X(b(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testNotOr) {
    Formula formula = Parser::parse("!(a(test)) | !(d(test)) | !(c(test)) | !(b(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(!(d(test)) | !(a(test)) | !(b(test)) | !(c(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testNotOrReorder) {
    Formula formula = Parser::parse("!(a(test)) | !(b(test)) | !(c(test)) | !(d(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(!(d(test)) | !(a(test)) | !(b(test)) | !(c(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testOrOr) {
    Formula formula = Parser::parse("!(a(test) | d(test)) | !(c(test) | b(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(!((a(test) | d(test))) | !((c(test) | b(test))))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testOrOrReorder) {
    Formula formula = Parser::parse("!(c(test) | b(test)) | !(a(test) | d(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(!((a(test) | d(test))) | !((c(test) | b(test))))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testSinceOr) {
    Formula formula = Parser::parse("(a(test))S(d(test)) | (c(test))S(b(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"((a(test) S d(test)) | (c(test) S b(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testSinceOrReorder) {
    Formula formula = Parser::parse("(c(test))S(b(test)) | (a(test))S(d(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"((a(test) S d(test)) | (c(test) S b(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testUntilOr) {
    Formula formula = Parser::parse("(a(test))S(d(test)) | (c(test))S(b(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"((a(test) S d(test)) | (c(test) S b(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testUntilOrReorder) {
    Formula formula = Parser::parse("(b(test))U(c(test)) | (a(test))U(d(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"((a(test) U d(test)) | (b(test) U c(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testAndOrDuplicatedPredicates) {
    Formula formula = Parser::parse("(a(test)) & ((a(test)) | (b(test)) | (c(test)))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"((a(test) | c(test) | b(test)) & a(test))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testAndOrDuplicatedPredicatesWithPriority) {
    Formula formula = Parser::parse("(a(test)) & ((a(test)) | (b(test)) | (c(test)))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{
        {"a", {20.0, 0.80, 1}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(a(test) & (a(test) | c(test) | b(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testUniversalSimple) {
    Formula formula = Parser::parse("A x, y in D. p(x, y)");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{};
    std::string expectedFormula{"A x in D, y in D. p(x, y)"};
    DomainStorage dom1{"D", {"1", "2", "3"}, [](const std::any &val) { return std::any_cast<const char *>(val); }};
    std::vector<std::shared_ptr<DomainStorage>> domains{std::make_shared<DomainStorage>(dom1)};
    executeTest(formula, expectedFormula, predicateCosts, domains);
}

TEST(TestHydraCreation, testUniversalNested) {
    Formula formula = Parser::parse("A w in D0, x in D1. A y, z in D2. p(w, x, y, z)");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{};
    std::string expectedFormula{"A w in D0, x in D1. A y in D2, z in D2. p(w, x, y, z)"};
    DomainStorage dom1{"D0", {"0"}, [](const std::any &val) { return std::any_cast<const char *>(val); }};
    DomainStorage dom2{"D1", {"1"}, [](const std::any &val) { return std::any_cast<const char *>(val); }};
    DomainStorage dom3{"D2", {"2"}, [](const std::any &val) { return std::any_cast<const char *>(val); }};
    std::vector<std::shared_ptr<DomainStorage>> domains{std::make_shared<DomainStorage>(dom1),
                                                        std::make_shared<DomainStorage>(dom2),
                                                        std::make_shared<DomainStorage>(dom3)};
    executeTest(formula, expectedFormula, predicateCosts, domains);
}

TEST(TestHydraCreation, testExistentialSimple) {
    Formula formula = Parser::parse("E x in D0, y in D1, z in D2. p0(x, y) & p1(y, z)");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{};
    std::string expectedFormula{"E x in D0, y in D1, z in D2. (p0(x, y) & p1(y, z))"};
    DomainStorage dom1{"D0", {"1", "2", "3"}, [](const std::any &val) { return std::any_cast<const char *>(val); }};
    DomainStorage dom2{"D1", {"4", "5", "6"}, [](const std::any &val) { return std::any_cast<const char *>(val); }};
    DomainStorage dom3{"D2", {"7", "8", "9"}, [](const std::any &val) { return std::any_cast<const char *>(val); }};
    std::vector<std::shared_ptr<DomainStorage>> domains{std::make_shared<DomainStorage>(dom1),
                                                        std::make_shared<DomainStorage>(dom2),
                                                        std::make_shared<DomainStorage>(dom3)};
    executeTest(formula, expectedFormula, predicateCosts, domains);
}

TEST(TestHydraCreation, testQuoteSimple) {
    Formula formula = Parser::parse("Q[0.1, 0.5] x in D0, y in D1. p(x, y)");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{};
    std::string expectedFormula{"Q[0.100000, 0.500000] x in D0, y in D1. p(x, y)"};
    DomainStorage dom1{"D0", {"0"}, [](const std::any &val) { return std::any_cast<const char *>(val); }};
    DomainStorage dom2{"D1", {"1"}, [](const std::any &val) { return std::any_cast<const char *>(val); }};
    std::vector<std::shared_ptr<DomainStorage>> domains{std::make_shared<DomainStorage>(dom1),
                                                        std::make_shared<DomainStorage>(dom2)};
    executeTest(formula, expectedFormula, predicateCosts, domains);
}

TEST(TesthydraCreation, testCountingSimple) {
    Formula formula = Parser::parse("C<=3 x in D. p(x)");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{};
    std::string expectedFormula{"C<=3 x in D. p(x)"};
    DomainStorage dom1{"D", {"a", "b", "c"}, [](const std::any &val) { return std::any_cast<const char *>(val); }};
    std::vector<std::shared_ptr<DomainStorage>> domains{std::make_shared<DomainStorage>(dom1)};
    executeTest(formula, expectedFormula, predicateCosts, domains);
}

TEST(TestHydraCreation, testCountingNested) {
    Formula formula = Parser::parse("C=2 x in D0. C>=4 y in D1. x & y");
    formula.setId("R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{};
    std::string expectedFormula{"C=2 x in D0. C>=4 y in D1. (x & y)"};
    DomainStorage dom1{"D0", {"a"}, [](const std::any &val) { return std::any_cast<const char *>(val); }};
    DomainStorage dom2{"D1", {"b"}, [](const std::any &val) { return std::any_cast<const char *>(val); }};
    std::vector<std::shared_ptr<DomainStorage>> domains{std::make_shared<DomainStorage>(dom1),
                                                        std::make_shared<DomainStorage>(dom2)};
    executeTest(formula, expectedFormula, predicateCosts, domains);
}

TEST(TestHydraCreation, testInvalidPredicateCost) {
    Formula formula = Parser::parse("G(P[0, 1](c))", "R1");
    std::map<std::string, std::array<double, 3>> predicateCosts{{"test", {1, 2}}};
#ifdef NDEBUG
    GTEST_SKIP() << "assertions are disabled";
#endif
    EXPECT_DEATH(HydraCreationVisitor2(predicateCosts, false);, "Invalid predicate costs/probabilities");
}

namespace {
// Dynamic domain whose values are only known during the evaluation.
std::shared_ptr<DomainStorage> dynamicDomain(const std::string &name) {
    auto function{std::make_shared<Function<std::vector<std::any>, size_t>>(Function<std::vector<std::any>, size_t>(
        name, {"stepNumber"}, [](size_t) { return std::vector<std::any>{1ul, 2ul}; },
        [](const std::vector<DomainValue> &args) {
            return std::tuple<size_t>{std::any_cast<size_t>(args.at(0).getValue())};
        }))};
    return std::make_shared<DomainStorage>(
        name, function, [](const std::any &val) { return std::to_string(std::any_cast<size_t>(val)); },
        [](size_t, const std::any &) { return true; });
}
} // namespace

// The cost and satisfaction probability of quantifiers were computed when the monitor was created, i.e., while dynamic
// domains were still empty. Empty dynamic domains were skipped (factor 1), or the number of combinations was 0 if all
// domains were dynamic, so these quantifiers looked free and were evaluated first by the reordering.
TEST(TestHydraCreation, testQuantifierCostWithDynamicDomain) {
    const Formula predicateFormula{Parser::parse("p(x)")};
    const double predicateCost{20.0};
    const double predicateProbability{0.5};
    for (const size_t expectedSize : {1ul, 4ul}) {
        const auto domain{dynamicDomain("D")};
        EXPECT_EQ(domain->getExpectedSize(), 1);
        if (expectedSize != 1)
            domain->setExpectedSize(expectedSize);
        EXPECT_EQ(domain->getExpectedSize(), expectedSize);
        EXPECT_EQ(domain->estimatedSize(), expectedSize);
        std::map<std::string, std::shared_ptr<DomainStorage>> variableValues{{"x", domain}};
        const auto predicate{
            std::make_shared<PredicateMonitor>(predicateFormula, 1, predicateCost, predicateProbability)};
        const auto numEval{static_cast<double>(expectedSize)};

        const ExistentialMonitor existential{Parser::parse("E x in D. p(x)"), predicate, 0, variableValues, true};
        EXPECT_DOUBLE_EQ(existential.getEvalCost(), predicateCost * numEval);
        EXPECT_DOUBLE_EQ(existential.getSatisfactionProbability(), 1 - std::pow(1 - predicateProbability, numEval));

        const UniversalMonitor universal{Parser::parse("A x in D. p(x)"), predicate, 0, variableValues, true};
        EXPECT_DOUBLE_EQ(universal.getEvalCost(), predicateCost * numEval);
        EXPECT_DOUBLE_EQ(universal.getSatisfactionProbability(), std::pow(predicateProbability, numEval));

        const CountingMonitor counting{Parser::parse("C>=1 x in D. p(x)"),       predicate, 0,   variableValues,
                                       CountingQuantifier::Operator::GREATER_EQ, 1,         true};
        EXPECT_DOUBLE_EQ(counting.getEvalCost(), predicateCost * numEval);
        EXPECT_DOUBLE_EQ(counting.getSatisfactionProbability(), 1 - std::pow(1 - predicateProbability, numEval));

        const QuoteMonitor quote{
            Parser::parse("Q[0.0, 1.0] x in D. p(x)"), predicate, 0, variableValues, 0.0, 1.0, true};
        EXPECT_DOUBLE_EQ(quote.getEvalCost(), predicateCost * numEval);
        EXPECT_DOUBLE_EQ(quote.getSatisfactionProbability(), 1.0);
    }
}

// A quantifier over a dynamic domain with many expected values is evaluated after a quantifier over a small constant
// domain.
TEST(TestHydraCreation, testReorderQuantifierWithDynamicDomain) {
    Formula formula = Parser::parse("(E x in D0. p(x)) & (E y in D1. q(y))");
    formula.setId("R1");
    const std::map<std::string, std::array<double, 3>> predicateCosts{{"p", {20.0, 0.5}}, {"q", {20.0, 0.5}}};
    const auto dynamic{dynamicDomain("D0")};
    dynamic->setExpectedSize(10);
    const auto constant{std::make_shared<DomainStorage>("D1", std::vector<std::any>{1ul, 2ul}, [](const std::any &val) {
        return std::to_string(std::any_cast<size_t>(val));
    })};
    executeTest(formula, "(E y in D1. q(y) & E x in D0. p(x))", predicateCosts, {dynamic, constant});
}
