//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "test_hydra_creation.h"

#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra_2/hydra_creation_visitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/hydra_monitor.h"
#include <utility>

void executeTest(Formula inputFormula, const std::string &expectedFormulaStringReordered,
                 const std::map<std::string, std::array<double, 2>> &predicateCosts) {
    HydraCreationVisitor hcv{HydraCreationVisitor(predicateCosts, false)};
    std::shared_ptr<HydraMonitor> mon = hcv.createMonitor(inputFormula, 0, false);
    EXPECT_EQ(mon->to_string(), inputFormula.to_string());

    hcv = HydraCreationVisitor(predicateCosts, false);
    mon = hcv.createMonitor(inputFormula, 0, true);
    EXPECT_EQ(mon->to_string(), inputFormula.to_string());

    hcv = HydraCreationVisitor(predicateCosts, true);
    mon = hcv.createMonitor(inputFormula, 0, false);
    EXPECT_EQ(mon->to_string(), expectedFormulaStringReordered);

    hcv = HydraCreationVisitor(predicateCosts, true);
    mon = hcv.createMonitor(inputFormula, 0, true);
    EXPECT_EQ(mon->to_string(), expectedFormulaStringReordered);
}

TEST(TestHydraCreation, testAnd) {
    Formula formula = Parser::parse("(d(test) & a(test) & b(test) & c(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 2>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(d(test) & a(test) & b(test) & c(test))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testAndReorder) {
    Formula formula = Parser::parse("(a(test) & b(test) & c(test) & d(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 2>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(d(test) & a(test) & b(test) & c(test))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testAndAndDuplicated) {
    Formula formula =
        Parser::parse("(!(a(test)) & P(b(test))) | X(!(a(test)) | (!(a(test)) & P(b(test))) | P(c(test)))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 2>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{
        "(X((!(a(test)) | (!(a(test)) & P(b(test))) | P(c(test)))) | (!(a(test)) & P(b(test))))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testPreviousAnd) {
    Formula formula = Parser::parse("P(d(test)) & P(a(test)) & P(b(test)) & P(c(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 2>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(P(d(test)) & P(a(test)) & P(b(test)) & P(c(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testAndOrDuplicatedPrevious) {
    Formula formula = Parser::parse("P(a(test)) & (P(a(test)) | P(b(test)) | P(c(test)))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 2>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(P(a(test)) & (P(a(test)) | P(c(test)) | P(b(test))))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testPreviousAndReorder) {
    Formula formula = Parser::parse("P(a(test)) & P(b(test)) & P(c(test)) & P(d(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 2>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(P(d(test)) & P(a(test)) & P(b(test)) & P(c(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testNextAnd) {
    Formula formula = Parser::parse("X(d(test)) & X(a(test)) & X(b(test)) & X(c(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 2>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    HydraCreationVisitor hcv{HydraCreationVisitor(predicateCosts)};
    std::string expectedFormula{"(X(d(test)) & X(a(test)) & X(b(test)) & X(c(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testNextAndReorder) {
    Formula formula = Parser::parse("X(a(test)) & X(b(test)) & X(c(test)) & X(d(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 2>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(X(d(test)) & X(a(test)) & X(b(test)) & X(c(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testAndOrDuplicatedNext) {
    Formula formula = Parser::parse("X(a(test)) & (X(a(test)) | X(b(test)) | X(c(test)))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 2>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(X(a(test)) & (X(a(test)) | X(c(test)) | X(b(test))))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testNotAnd) {
    Formula formula = Parser::parse("!(a(test)) & !(d(test)) & !(c(test)) & !(b(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 2>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(!(a(test)) & !(d(test)) & !(c(test)) & !(b(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testNotAndReorder) {
    Formula formula = Parser::parse("!(a(test)) & !(b(test)) & !(c(test)) & !(d(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 2>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(!(a(test)) & !(d(test)) & !(c(test)) & !(b(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testNotAndDuplicated) {
    Formula formula = Parser::parse("!(a(test)) & (!(a(test)) | P(b(test)) | P(c(test)))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 2>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(!(a(test)) & (!(a(test)) | P(c(test)) | P(b(test))))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testAndAnd) {
    Formula formula = Parser::parse("!(d(test) & a(test)) & !(b(test) & c(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 2>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(!((d(test) & a(test))) & !((b(test) & c(test))))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testAndAndReorder) {
    Formula formula = Parser::parse("!(b(test) & c(test)) & !(d(test) & a(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 2>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(!((d(test) & a(test))) & !((b(test) & c(test))))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testSinceAnd) {
    Formula formula = Parser::parse("(a(test))S(d(test)) & (c(test))S(b(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 2>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"((a(test) S d(test)) & (c(test) S b(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testSinceAndReorder) {
    Formula formula = Parser::parse("(c(test))S(b(test)) & (a(test))S(d(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 2>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"((a(test) S d(test)) & (c(test) S b(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testUntilAnd) {
    Formula formula = Parser::parse("(a(test))S(d(test)) & (c(test))S(b(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 2>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"((a(test) S d(test)) & (c(test) S b(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testUntilAndReorder) {
    Formula formula = Parser::parse("(c(test))U(b(test)) & (a(test))U(d(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 2>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"((a(test) U d(test)) & (c(test) U b(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testOr) {
    Formula formula = Parser::parse("a(test) | d(test) | c(test) | b(test)");
    formula.setId("R1");
    std::map<std::string, std::array<double, 2>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(a(test) | d(test) | c(test) | b(test))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testOrReorder) {
    Formula formula = Parser::parse("a(test) | b(test) | c(test) | d(test)");
    formula.setId("R1");
    std::map<std::string, std::array<double, 2>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(a(test) | d(test) | c(test) | b(test))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testOrOrDuplicated) {
    Formula formula =
        Parser::parse("(!(a(test)) | P(b(test))) & X(!(a(test)) & (!(a(test)) | P(b(test))) & P(c(test)))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 2>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{
        "((!(a(test)) | P(b(test))) & X((!(a(test)) & (!(a(test)) | P(b(test))) & P(c(test)))))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testPreviousOr) {
    Formula formula = Parser::parse("P(a(test)) | P(d(test)) | P(c(test)) | P(b(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 2>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(P(a(test)) | P(d(test)) | P(c(test)) | P(b(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testPreviousOrReorder) {
    Formula formula = Parser::parse("P(a(test)) | P(b(test)) | P(c(test)) | P(d(test)))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 2>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(P(a(test)) | P(d(test)) | P(c(test)) | P(b(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testNextOr) {
    Formula formula = Parser::parse("X(a(test)) | X(d(test)) | X(c(test)) | X(b(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 2>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(X(a(test)) | X(d(test)) | X(c(test)) | X(b(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testNextOrReorder) {
    Formula formula = Parser::parse("X(a(test)) | X(b(test)) | X(c(test)) | X(d(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 2>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(X(a(test)) | X(d(test)) | X(c(test)) | X(b(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testNotOr) {
    Formula formula = Parser::parse("!(a(test)) | !(d(test)) | !(c(test)) | !(b(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 2>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(!(d(test)) | !(a(test)) | !(b(test)) | !(c(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testNotOrReorder) {
    Formula formula = Parser::parse("!(a(test)) | !(b(test)) | !(c(test)) | !(d(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 2>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(!(d(test)) | !(a(test)) | !(b(test)) | !(c(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testOrOr) {
    Formula formula = Parser::parse("!(a(test) | d(test)) | !(c(test) | b(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 2>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(!((a(test) | d(test))) | !((c(test) | b(test))))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testOrOrReorder) {
    Formula formula = Parser::parse("!(c(test) | b(test)) | !(a(test) | d(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 2>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(!((a(test) | d(test))) | !((c(test) | b(test))))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testSinceOr) {
    Formula formula = Parser::parse("(a(test))S(d(test)) | (c(test))S(b(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 2>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"((a(test) S d(test)) | (c(test) S b(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testSinceOrReorder) {
    Formula formula = Parser::parse("(c(test))S(b(test)) | (a(test))S(d(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 2>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"((a(test) S d(test)) | (c(test) S b(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testUntilOr) {
    Formula formula = Parser::parse("(a(test))S(d(test)) | (c(test))S(b(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 2>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"((a(test) S d(test)) | (c(test) S b(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testUntilOrReorder) {
    Formula formula = Parser::parse("(b(test))U(c(test)) | (a(test))U(d(test))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 2>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"((a(test) U d(test)) | (b(test) U c(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}

TEST(TestHydraCreation, testAndOrDuplicatedPredicate) {
    Formula formula = Parser::parse("(a(test)) & ((a(test)) | (b(test)) | (c(test)))");
    formula.setId("R1");
    std::map<std::string, std::array<double, 2>> predicateCosts{
        {"a", {20.0, 0.80}}, {"b", {100.0, 0.20}}, {"c", {100.0, 0.80}}, {"d", {20.0, 0.20}}};
    std::string expectedFormula{"(a(test) & (a(test) | c(test) | b(test)))"};
    executeTest(formula, expectedFormula, predicateCosts);
}
