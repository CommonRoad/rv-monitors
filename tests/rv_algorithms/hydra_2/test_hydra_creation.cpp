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

TEST(TestHydraCreation, testAnd) {
    Formula formula = Parser::parse("d(test) and a(test) and b(test) and c(test)");
    formula.setId("R1");
    std::vector<std::tuple<std::string, double, double>> predicateCosts{
        {"a", 20.0, 80.0}, {"b", 100.0, 20.0}, {"c", 100.0, 80.0}, {"d", 20.0, 20.0}};
    HydraCreationVisitor hc{HydraCreationVisitor(predicateCosts)};
    std::shared_ptr<HydraMonitor> mon = hc.createMonitor(formula, 0, true);
    EXPECT_EQ(mon->to_string(), "d(test) and a(test) and b(test) and c(test)");
}

TEST(TestHydraCreation, testAndReorder) {
    Formula formula = Parser::parse("a(test) and b(test) and c(test) and d(test)");
    formula.setId("R1");
    std::vector<std::tuple<std::string, double, double>> predicateCosts{
        {"a", 20.0, 80.0}, {"b", 100.0, 20.0}, {"c", 100.0, 80.0}, {"d", 20.0, 20.0}};
    HydraCreationVisitor hc{HydraCreationVisitor(predicateCosts)};
    std::shared_ptr<HydraMonitor> mon = hc.createMonitor(formula, 0, true);
    EXPECT_EQ(mon->to_string(), "d(test) and a(test) and b(test) and c(test)");
}

TEST(TestHydraCreation, testPreviousAnd) {
    Formula formula = Parser::parse("P(d(test)) and P(a(test)) and P(b(test)) and P(c(test))");
    formula.setId("R1");
    std::vector<std::tuple<std::string, double, double>> predicateCosts{
        {"a", 20.0, 80.0}, {"b", 100.0, 20.0}, {"c", 100.0, 80.0}, {"d", 20.0, 20.0}};
    HydraCreationVisitor hc{HydraCreationVisitor(predicateCosts)};
    std::shared_ptr<HydraMonitor> mon = hc.createMonitor(formula, 0, true);
    EXPECT_EQ(mon->to_string(), "P(d(test)) and P(a(test)) and P(b(test)) and P(c(test))");
}

TEST(TestHydraCreation, testPreviousAndReorder) {
    Formula formula = Parser::parse("P(a(test)) and P(b(test)) and P(c(test)) and P(d(test))");
    formula.setId("R1");
    std::vector<std::tuple<std::string, double, double>> predicateCosts{
        {"a", 20.0, 80.0}, {"b", 100.0, 20.0}, {"c", 100.0, 80.0}, {"d", 20.0, 20.0}};
    HydraCreationVisitor hc{HydraCreationVisitor(predicateCosts)};
    std::shared_ptr<HydraMonitor> mon = hc.createMonitor(formula, 0, true);
    EXPECT_EQ(mon->to_string(), "P(d(test)) and P(a(test)) and P(b(test)) and P(c(test))");
}

TEST(TestHydraCreation, testNextAnd) {
    Formula formula = Parser::parse("X(d(test)) and X(a(test)) and X(b(test)) and X(c(test))");
    formula.setId("R1");
    std::vector<std::tuple<std::string, double, double>> predicateCosts{
        {"a", 20.0, 80.0}, {"b", 100.0, 20.0}, {"c", 100.0, 80.0}, {"d", 20.0, 20.0}};
    HydraCreationVisitor hc{HydraCreationVisitor(predicateCosts)};
    std::shared_ptr<HydraMonitor> mon = hc.createMonitor(formula, 0, true);
    EXPECT_EQ(mon->to_string(), "X(d(test)) and X(a(test)) and X(b(test)) and X(c(test))");
}

TEST(TestHydraCreation, testNextAndReorder) {
    Formula formula = Parser::parse("X(a(test)) and X(b(test)) and X(c(test)) and X(d(test))");
    formula.setId("R1");
    std::vector<std::tuple<std::string, double, double>> predicateCosts{
        {"a", 20.0, 80.0}, {"b", 100.0, 20.0}, {"c", 100.0, 80.0}, {"d", 20.0, 20.0}};
    HydraCreationVisitor hc{HydraCreationVisitor(predicateCosts)};
    std::shared_ptr<HydraMonitor> mon = hc.createMonitor(formula, 0, true);
    EXPECT_EQ(mon->to_string(), "X(d(test)) and X(a(test)) and X(b(test)) and X(c(test))");
}

TEST(TestHydraCreation, testNotAnd) {
    Formula formula = Parser::parse("!(a(test)) and !(d(test)) and !(c(test)) and !(b(test))");
    formula.setId("R1");
    std::vector<std::tuple<std::string, double, double>> predicateCosts{
        {"a", 20.0, 80.0}, {"b", 100.0, 20.0}, {"c", 100.0, 80.0}, {"d", 20.0, 20.0}};
    HydraCreationVisitor hc{HydraCreationVisitor(predicateCosts)};
    std::shared_ptr<HydraMonitor> mon = hc.createMonitor(formula, 0, true);
    EXPECT_EQ(mon->to_string(), "!(a(test)) and !(d(test)) and !(c(test)) and !(b(test))");
}

TEST(TestHydraCreation, testNotAndReorder) {
    Formula formula = Parser::parse("!(a(test)) and !(b(test)) and !(c(test)) and !(d(test))");
    formula.setId("R1");
    std::vector<std::tuple<std::string, double, double>> predicateCosts{
        {"a", 20.0, 80.0}, {"b", 100.0, 20.0}, {"c", 100.0, 80.0}, {"d", 20.0, 20.0}};
    HydraCreationVisitor hc{HydraCreationVisitor(predicateCosts)};
    std::shared_ptr<HydraMonitor> mon = hc.createMonitor(formula, 0, true);
    EXPECT_EQ(mon->to_string(), "!(a(test)) and !(d(test)) and !(c(test)) and !(b(test))");
}

TEST(TestHydraCreation, testAndAnd) {
    Formula formula = Parser::parse("!(d(test) and a(test)) and !(b(test) and c(test))");
    formula.setId("R1");
    std::vector<std::tuple<std::string, double, double>> predicateCosts{
        {"a", 20.0, 80.0}, {"b", 100.0, 20.0}, {"c", 100.0, 80.0}, {"d", 20.0, 20.0}};
    HydraCreationVisitor hc{HydraCreationVisitor(predicateCosts)};
    std::shared_ptr<HydraMonitor> mon = hc.createMonitor(formula, 0, true);
    EXPECT_EQ(mon->to_string(), "!(d(test) and a(test)) and !(b(test) and c(test))");
}

TEST(TestHydraCreation, testAndAndReorder) {
    Formula formula = Parser::parse("!(b(test) and c(test)) and !(d(test) and a(test))");
    formula.setId("R1");
    std::vector<std::tuple<std::string, double, double>> predicateCosts{
        {"a", 20.0, 80.0}, {"b", 100.0, 20.0}, {"c", 100.0, 80.0}, {"d", 20.0, 20.0}};
    HydraCreationVisitor hc{HydraCreationVisitor(predicateCosts)};
    std::shared_ptr<HydraMonitor> mon = hc.createMonitor(formula, 0, true);
    EXPECT_EQ(mon->to_string(), "!(d(test) and a(test)) and !(b(test) and c(test))");
}

TEST(TestHydraCreation, testSinceAnd) {
    Formula formula = Parser::parse("(a(test))S(d(test)) and (c(test))S(b(test))");
    formula.setId("R1");
    std::vector<std::tuple<std::string, double, double>> predicateCosts{
        {"a", 20.0, 80.0}, {"b", 100.0, 20.0}, {"c", 100.0, 80.0}, {"d", 20.0, 20.0}};
    HydraCreationVisitor hc{HydraCreationVisitor(predicateCosts)};
    std::shared_ptr<HydraMonitor> mon = hc.createMonitor(formula, 0, true);
    EXPECT_EQ(mon->to_string(), "(a(test))S(d(test)) and (c(test))S(b(test))");
}

TEST(TestHydraCreation, testSinceAndReorder) {
    Formula formula = Parser::parse("(c(test))S(b(test)) and (a(test))S(d(test))");
    formula.setId("R1");
    std::vector<std::tuple<std::string, double, double>> predicateCosts{
        {"a", 20.0, 80.0}, {"b", 100.0, 20.0}, {"c", 100.0, 80.0}, {"d", 20.0, 20.0}};
    HydraCreationVisitor hc{HydraCreationVisitor(predicateCosts)};
    std::shared_ptr<HydraMonitor> mon = hc.createMonitor(formula, 0, true);
    EXPECT_EQ(mon->to_string(), "(a(test))S(d(test)) and (c(test))S(b(test))");
}

TEST(TestHydraCreation, testUntilAnd) {
    Formula formula = Parser::parse("(a(test))S(d(test)) and (c(test))S(b(test))");
    formula.setId("R1");
    std::vector<std::tuple<std::string, double, double>> predicateCosts{
        {"a", 20.0, 80.0}, {"b", 100.0, 20.0}, {"c", 100.0, 80.0}, {"d", 20.0, 20.0}};
    HydraCreationVisitor hc{HydraCreationVisitor(predicateCosts)};
    std::shared_ptr<HydraMonitor> mon = hc.createMonitor(formula, 0, true);
    EXPECT_EQ(mon->to_string(), "(a(test))S(d(test)) and (c(test))S(b(test))");
}

TEST(TestHydraCreation, testUntilAndReorder) {
    Formula formula = Parser::parse("(c(test))U(b(test)) and (a(test))U(d(test))");
    formula.setId("R1");
    std::vector<std::tuple<std::string, double, double>> predicateCosts{
        {"a", 20.0, 80.0}, {"b", 100.0, 20.0}, {"c", 100.0, 80.0}, {"d", 20.0, 20.0}};
    HydraCreationVisitor hc{HydraCreationVisitor(predicateCosts)};
    std::shared_ptr<HydraMonitor> mon = hc.createMonitor(formula, 0, true);
    EXPECT_EQ(mon->to_string(), "(a(test))U(d(test)) and (c(test))U(b(test))");
}

TEST(TestHydraCreation, testOr) {
    Formula formula = Parser::parse("a(test) or d(test) or c(test) or b(test)");
    formula.setId("R1");
    std::vector<std::tuple<std::string, double, double>> predicateCosts{
        {"a", 20.0, 80.0}, {"b", 100.0, 20.0}, {"c", 100.0, 80.0}, {"d", 20.0, 20.0}};
    HydraCreationVisitor hc{HydraCreationVisitor(predicateCosts)};
    std::shared_ptr<HydraMonitor> mon = hc.createMonitor(formula, 0, true);
    EXPECT_EQ(mon->to_string(), "a(test) or d(test) or c(test) or b(test)");
}

TEST(TestHydraCreation, testOrReorder) {
    Formula formula = Parser::parse("a(test) or b(test) or c(test) or d(test)");
    formula.setId("R1");
    std::vector<std::tuple<std::string, double, double>> predicateCosts{
        {"a", 20.0, 80.0}, {"b", 100.0, 20.0}, {"c", 100.0, 80.0}, {"d", 20.0, 20.0}};
    HydraCreationVisitor hc{HydraCreationVisitor(predicateCosts)};
    std::shared_ptr<HydraMonitor> mon = hc.createMonitor(formula, 0, true);
    EXPECT_EQ(mon->to_string(), "a(test) or d(test) or c(test) or b(test)");
}

TEST(TestHydraCreation, testPreviousOr) {
    Formula formula = Parser::parse("P(a(test)) or P(d(test)) or P(c(test)) or P(b(test))");
    formula.setId("R1");
    std::vector<std::tuple<std::string, double, double>> predicateCosts{
        {"a", 20.0, 80.0}, {"b", 100.0, 20.0}, {"c", 100.0, 80.0}, {"d", 20.0, 20.0}};
    HydraCreationVisitor hc{HydraCreationVisitor(predicateCosts)};
    std::shared_ptr<HydraMonitor> mon = hc.createMonitor(formula, 0, true);
    EXPECT_EQ(mon->to_string(), "P(a(test)) or P(d(test)) or P(c(test)) or P(b(test))");
}

TEST(TestHydraCreation, testPreviousOrReorder) {
    Formula formula = Parser::parse("P(a(test)) or P(b(test)) or P(c(test)) or P(d(test))");
    formula.setId("R1");
    std::vector<std::tuple<std::string, double, double>> predicateCosts{
        {"a", 20.0, 80.0}, {"b", 100.0, 20.0}, {"c", 100.0, 80.0}, {"d", 20.0, 20.0}};
    HydraCreationVisitor hc{HydraCreationVisitor(predicateCosts)};
    std::shared_ptr<HydraMonitor> mon = hc.createMonitor(formula, 0, true);
    EXPECT_EQ(mon->to_string(), "P(a(test)) or P(d(test)) or P(c(test)) or P(b(test))");
}

TEST(TestHydraCreation, testNextOr) {
    Formula formula = Parser::parse("X(a(test)) or X(d(test)) or X(c(test)) or X(b(test))");
    formula.setId("R1");
    std::vector<std::tuple<std::string, double, double>> predicateCosts{
        {"a", 20.0, 80.0}, {"b", 100.0, 20.0}, {"c", 100.0, 80.0}, {"d", 20.0, 20.0}};
    HydraCreationVisitor hc{HydraCreationVisitor(predicateCosts)};
    std::shared_ptr<HydraMonitor> mon = hc.createMonitor(formula, 0, true);
    EXPECT_EQ(mon->to_string(), "X(a(test)) or X(d(test)) or X(c(test)) or X(b(test))");
}

TEST(TestHydraCreation, testNextOrReorder) {
    Formula formula = Parser::parse("X(a(test)) or X(b(test)) or X(c(test)) or X(d(test))");
    formula.setId("R1");
    std::vector<std::tuple<std::string, double, double>> predicateCosts{
        {"a", 20.0, 80.0}, {"b", 100.0, 20.0}, {"c", 100.0, 80.0}, {"d", 20.0, 20.0}};
    HydraCreationVisitor hc{HydraCreationVisitor(predicateCosts)};
    std::shared_ptr<HydraMonitor> mon = hc.createMonitor(formula, 0, true);
    EXPECT_EQ(mon->to_string(), "X(a(test)) or X(d(test)) or X(c(test)) or X(b(test))");
}

TEST(TestHydraCreation, testNotOr) {
    Formula formula = Parser::parse("!(a(test)) or !(d(test)) or !(c(test)) or !(b(test))");
    formula.setId("R1");
    std::vector<std::tuple<std::string, double, double>> predicateCosts{
        {"a", 20.0, 80.0}, {"b", 100.0, 20.0}, {"c", 100.0, 80.0}, {"d", 20.0, 20.0}};
    HydraCreationVisitor hc{HydraCreationVisitor(predicateCosts)};
    std::shared_ptr<HydraMonitor> mon = hc.createMonitor(formula, 0, true);
    EXPECT_EQ(mon->to_string(), "!(d(test)) or !(a(test)) or !(b(test)) or !(c(test))");
}

TEST(TestHydraCreation, testNotOrReorder) {
    Formula formula = Parser::parse("!(a(test)) or !(b(test)) or !(c(test)) or !(d(test))");
    formula.setId("R1");
    std::vector<std::tuple<std::string, double, double>> predicateCosts{
        {"a", 20.0, 80.0}, {"b", 100.0, 20.0}, {"c", 100.0, 80.0}, {"d", 20.0, 20.0}};
    HydraCreationVisitor hc{HydraCreationVisitor(predicateCosts)};
    std::shared_ptr<HydraMonitor> mon = hc.createMonitor(formula, 0, true);
    EXPECT_EQ(mon->to_string(), "!(d(test)) or !(a(test)) or !(b(test)) or !(c(test))");
}

TEST(TestHydraCreation, testOrOr) {
    Formula formula = Parser::parse("!(a(test) or d(test)) or !(c(test) or b(test))");
    formula.setId("R1");
    std::vector<std::tuple<std::string, double, double>> predicateCosts{
        {"a", 20.0, 80.0}, {"b", 100.0, 20.0}, {"c", 100.0, 80.0}, {"d", 20.0, 20.0}};
    HydraCreationVisitor hc{HydraCreationVisitor(predicateCosts)};
    std::shared_ptr<HydraMonitor> mon = hc.createMonitor(formula, 0, true);
    EXPECT_EQ(mon->to_string(), "!(a(test) or d(test)) or !(c(test) or b(test))");
}

TEST(TestHydraCreation, testOrOrReorder) {
    Formula formula = Parser::parse("!(c(test) or b(test)) or !(a(test) or d(test))");
    formula.setId("R1");
    std::vector<std::tuple<std::string, double, double>> predicateCosts{
        {"a", 20.0, 80.0}, {"b", 100.0, 20.0}, {"c", 100.0, 80.0}, {"d", 20.0, 20.0}};
    HydraCreationVisitor hc{HydraCreationVisitor(predicateCosts)};
    std::shared_ptr<HydraMonitor> mon = hc.createMonitor(formula, 0, true);
    EXPECT_EQ(mon->to_string(), "!(a(test) or d(test)) or !(c(test) or b(test))");
}

TEST(TestHydraCreation, testSinceOr) {
    Formula formula = Parser::parse("(a(test))S(d(test)) or (c(test))S(b(test))");
    formula.setId("R1");
    std::vector<std::tuple<std::string, double, double>> predicateCosts{
        {"a", 20.0, 80.0}, {"b", 100.0, 20.0}, {"c", 100.0, 80.0}, {"d", 20.0, 20.0}};
    HydraCreationVisitor hc{HydraCreationVisitor(predicateCosts)};
    std::shared_ptr<HydraMonitor> mon = hc.createMonitor(formula, 0, true);
    EXPECT_EQ(mon->to_string(), "(a(test))S(d(test)) or (c(test))S(b(test))");
}

TEST(TestHydraCreation, testSinceOrReorder) {
    Formula formula = Parser::parse("(c(test))S(b(test)) or (a(test))S(d(test))");
    formula.setId("R1");
    std::vector<std::tuple<std::string, double, double>> predicateCosts{
        {"a", 20.0, 80.0}, {"b", 100.0, 20.0}, {"c", 100.0, 80.0}, {"d", 20.0, 20.0}};
    HydraCreationVisitor hc{HydraCreationVisitor(predicateCosts)};
    std::shared_ptr<HydraMonitor> mon = hc.createMonitor(formula, 0, true);
    EXPECT_EQ(mon->to_string(), "(a(test))S(d(test)) or (c(test))S(b(test))");
}

TEST(TestHydraCreation, testUntilOr) {
    Formula formula = Parser::parse("(a(test))S(d(test)) or (c(test))S(b(test))");
    formula.setId("R1");
    std::vector<std::tuple<std::string, double, double>> predicateCosts{
        {"a", 20.0, 80.0}, {"b", 100.0, 20.0}, {"c", 100.0, 80.0}, {"d", 20.0, 20.0}};
    HydraCreationVisitor hc{HydraCreationVisitor(predicateCosts)};
    std::shared_ptr<HydraMonitor> mon = hc.createMonitor(formula, 0, true);
    EXPECT_EQ(mon->to_string(), "(a(test))S(d(test)) or (c(test))S(b(test))");
}

TEST(TestHydraCreation, testUntilOrReorder) {
    Formula formula = Parser::parse("(b(test))U(c(test)) or (a(test))U(d(test))");
    formula.setId("R1");
    std::vector<std::tuple<std::string, double, double>> predicateCosts{
        {"a", 20.0, 80.0}, {"b", 100.0, 20.0}, {"c", 100.0, 80.0}, {"d", 20.0, 20.0}};
    HydraCreationVisitor hc{HydraCreationVisitor(predicateCosts)};
    std::shared_ptr<HydraMonitor> mon = hc.createMonitor(formula, 0, true);
    EXPECT_EQ(mon->to_string(), "(a(test))U(d(test)) or (b(test))U(c(test))");
}