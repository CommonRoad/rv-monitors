//
// Created by Sebastian Maierhofer and Evald Nexhipi.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//

#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra_2/hydra.h"
#include "monitors/trace/trace.h"
#include "monitors/utils/generators/formula_generator/formula_generator.h"
#include "monitors/utils/generators/trace_generator/trace_generator.h"
#include <chrono>
#include <iostream>
#include <random>
#include <thread>

typedef std::mt19937 MyRNG;
uint32_t seed_val = 123456789;
MyRNG rng;
std::uniform_int_distribution<uint8_t> uint_dist2(0, 1);

void executeEval(const std::vector<Formula> &formulas, size_t numExecutions, size_t traceLength);
void evalFormula1(const std::vector<Formula> &formulas, size_t numExecutions, std::vector<size_t> &traceLength);
void evalFormula2(const std::vector<Formula> &formulas, size_t numExecutions, std::vector<size_t> &traceLength);
void evalFormula1Atomic(const std::vector<Formula> &formulas, size_t numExecutions, std::vector<size_t> &traceLength);
void evalFormula2Atomic(const std::vector<Formula> &formulas, size_t numExecutions, std::vector<size_t> &traceLength);

bool a(size_t dummy) {
    rng.seed(seed_val);
    std::this_thread::sleep_for(std::chrono::microseconds(10));
    return uint_dist2(rng) == 1;
}

bool b(size_t dummy) {
    rng.seed(seed_val);
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    return uint_dist2(rng) == 1;
}

bool c(size_t dummy) {
    rng.seed(seed_val);
    std::this_thread::sleep_for(std::chrono::microseconds(500));
    return uint_dist2(rng) == 1;
}

bool d(size_t dummy) {
    rng.seed(seed_val);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    return uint_dist2(rng) == 1;
}

bool e(size_t dummy) {
    rng.seed(seed_val);
    std::this_thread::sleep_for(std::chrono::microseconds(10));
    return uint_dist2(rng) == 1;
}

bool f(size_t dummy) {
    rng.seed(seed_val);
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    return uint_dist2(rng) == 1;
}

Trace createRandomTrace(const std::set<std::string> &atomics, size_t length) {
    std::vector<Event> eventList;
    size_t timeStepIdx{0};
    rng.seed(seed_val);

    for (; timeStepIdx < length; ++timeStepIdx) {
        std::map<std::string, lazy_bool> event;
        for (const auto &atom : atomics) {
            bool atomicValue{uint_dist2(rng) == 1};
            event[atom] = lazy_bool(atomicValue);
        }
        eventList.emplace_back(event, timeStepIdx, timeStepIdx == length - 1);
        event.clear();
    }
    return Trace(eventList);
}

Trace createRandomLazyTrace(size_t length) {
    std::vector<Event> eventList;
    size_t timeStepIdx{0};
    const std::map<std::string, std::function<bool(size_t)>> atomicsAndFunctions{
        {"atest", a}, {"btest", b}, {"ctest", c}, {"dtest", d}, {"etest", e}, {"ftest", f},
        {"a", a},     {"b", b},     {"c", c},     {"d", d},     {"e", e},     {"f", f}};
    rng.seed(seed_val);

    for (; timeStepIdx < length; ++timeStepIdx) {
        std::map<std::string, lazy_bool> event;
        for (auto const &[atomic, valueFetcher] : atomicsAndFunctions) {
            auto valueFetcherBound = std::bind(valueFetcher, 0);
            event[atomic] = lazy_bool(valueFetcherBound);
        }
        eventList.emplace_back(event, timeStepIdx, timeStepIdx == length - 1);
        event.clear();
    }
    return Trace(eventList);
}

double evalHydraCombined(Trace &trace, const std::vector<Formula> &formulas) {
    auto monitor{Hydra(formulas)};
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    monitor.evaluate(trace);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
}

double evalHydraSingleConnected(Trace &trace, const std::vector<Formula> &formulas) {
    std::vector<Hydra> monitors;
    monitors.reserve(formulas.size());
    for (const auto &form : formulas)
        monitors.push_back(Hydra({form}));
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    for (size_t i{0}; i < monitors.size(); ++i)
        monitors[i].evaluate(trace);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
}

double evalHydraSingle(Trace &trace, const std::vector<Formula> &formulas) {
    std::vector<Hydra> monitors;
    monitors.reserve(formulas.size());
    for (const auto &form : formulas)
        monitors.push_back(Hydra({form}, false));
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    for (size_t i{0}; i < monitors.size(); ++i)
        monitors[i].evaluate(trace);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
}

void executeEval(const std::vector<Formula> &formulas1, size_t numExecutions, size_t traceLength) {
    std::vector<double> avgCompTimeCombined;
    std::vector<double> avgCompTimeConnected;
    std::vector<double> avgCompTimeSingle;

    for (size_t i{0}; i < numExecutions; ++i) {
        Trace trace = createRandomLazyTrace(traceLength);
        avgCompTimeCombined.push_back(evalHydraCombined(trace, formulas1));
        avgCompTimeConnected.push_back(evalHydraSingleConnected(trace, formulas1));
        avgCompTimeSingle.push_back(evalHydraSingle(trace, formulas1));
    }
    std::cout << "Combined: "
              << std::accumulate(avgCompTimeCombined.begin(), avgCompTimeCombined.end(), 0) / numExecutions << '\n';
    std::cout << "Connected: "
              << std::accumulate(avgCompTimeConnected.begin(), avgCompTimeConnected.end(), 0) / numExecutions << '\n';
    std::cout << "Single: " << std::accumulate(avgCompTimeSingle.begin(), avgCompTimeSingle.end(), 0) / numExecutions
              << '\n';
    for (const auto &val : avgCompTimeCombined)
        std::cout << val << ", ";
    std::cout << '\n';
    for (const auto &val : avgCompTimeConnected)
        std::cout << val << ", ";
    std::cout << '\n';
    for (const auto &val : avgCompTimeSingle)
        std::cout << val << ", ";
    std::cout << '\n';
}

void executeEvalAtomic(const std::vector<Formula> &formulas1, size_t numExecutions, size_t traceLength) {
    std::vector<double> avgCompTimeCombined;
    std::vector<double> avgCompTimeConnected;
    std::vector<double> avgCompTimeSingle;

    for (size_t i{0}; i < numExecutions; ++i) {
        Trace trace = createRandomTrace({"a", "b", "c", "d", "e", "f", "g", "i", "j"}, traceLength);
        avgCompTimeCombined.push_back(evalHydraCombined(trace, formulas1));
        avgCompTimeConnected.push_back(evalHydraSingleConnected(trace, formulas1));
        avgCompTimeSingle.push_back(evalHydraSingle(trace, formulas1));
    }
    std::cout << "Combined: "
              << std::accumulate(avgCompTimeCombined.begin(), avgCompTimeCombined.end(), 0) / numExecutions << '\n';
    std::cout << "Connected: "
              << std::accumulate(avgCompTimeConnected.begin(), avgCompTimeConnected.end(), 0) / numExecutions << '\n';
    std::cout << "Single: " << std::accumulate(avgCompTimeSingle.begin(), avgCompTimeSingle.end(), 0) / numExecutions
              << '\n';
    for (const auto &val : avgCompTimeCombined)
        std::cout << val << ", ";
    std::cout << '\n';
    for (const auto &val : avgCompTimeConnected)
        std::cout << val << ", ";
    std::cout << '\n';
    for (const auto &val : avgCompTimeSingle)
        std::cout << val << ", ";
    std::cout << '\n';
}

void executeEvaluate2(const std::vector<Formula> &formulas1, size_t numExecutions, Trace trace) {
    std::vector<double> avgCompTimeCombined;
    std::vector<double> avgCompTimeConnected;
    std::vector<double> avgCompTimeSingle;

    for (size_t i{0}; i < numExecutions; ++i) {
        avgCompTimeCombined.push_back(evalHydraCombined(trace, formulas1));
        avgCompTimeConnected.push_back(evalHydraSingleConnected(trace, formulas1));
        avgCompTimeSingle.push_back(evalHydraSingle(trace, formulas1));
    }
    std::cout << "Combined: "
              << std::accumulate(avgCompTimeCombined.begin(), avgCompTimeCombined.end(), 0) / numExecutions << '\n';
    std::cout << "Connected: "
              << std::accumulate(avgCompTimeConnected.begin(), avgCompTimeConnected.end(), 0) / numExecutions << '\n';
    std::cout << "Single: " << std::accumulate(avgCompTimeSingle.begin(), avgCompTimeSingle.end(), 0) / numExecutions
              << '\n';
    for (const auto &val : avgCompTimeCombined)
        std::cout << val << ", ";
    std::cout << '\n';
    for (const auto &val : avgCompTimeConnected)
        std::cout << val << ", ";
    std::cout << '\n';
    for (const auto &val : avgCompTimeSingle)
        std::cout << val << ", ";
    std::cout << '\n';
}

void test_0();

void test_1();

void test_2();

int main() { test_1(); }

void test_0() {
    std::vector<Formula> formulas1 = {Parser::parse("(a(test))S[0, 4](b(test))"),
                                      Parser::parse("(a(test))U[0, 4](b(test))"),
                                      Parser::parse("(a(test))S[0, 4](b(test)) or (a(test))U[0, 4](b(test))")};
    std::vector<Formula> formulas2 = {Parser::parse("P(c(test))"), Parser::parse("X(d(test))"),
                                      Parser::parse("(a(test))S[0, 4](b(test)) or (e(test))U[0, 4](f(test))")};

    std::vector<Formula> formulas3 = {Parser::parse("(a S[0, 4] b)"), Parser::parse("(a U[0, 4] b)"),
                                      Parser::parse("(a S[0, 4] b) or (a U[0, 4] b)")};
    std::vector<Formula> formulas4 = {Parser::parse("P(c)"), Parser::parse("X(d)"),
                                      Parser::parse("(a S[0, 4] b) or (e U[0, 4] f)")};

    formulas1[0].setId("R1");
    formulas1[1].setId("R2");
    formulas1[2].setId("R3");
    formulas2[0].setId("R1");
    formulas2[1].setId("R2");
    formulas2[2].setId("R3");
    formulas3[0].setId("R1");
    formulas3[1].setId("R2");
    formulas3[2].setId("R3");
    formulas4[0].setId("R1");
    formulas4[1].setId("R2");
    formulas4[2].setId("R3");

    size_t numExecutions{1000};
    std::vector<size_t> traceLength{60, 300};
    evalFormula1(formulas1, numExecutions, traceLength);
    evalFormula2(formulas2, numExecutions, traceLength);

    //    traceLength = {50000, 100000, 200000};
    //    evalFormula1Atomic(formulas3, numExecutions, traceLength);
    //    evalFormula2Atomic(formulas4, numExecutions, traceLength);
}

void test_1() {
    FormulaGenerator fg;
    FormulaGenerator::rSeed();
    TraceGenerator tg;
    TraceGenerator::rSeed();

    auto a = std::make_shared<Atomic>("a");
    auto b = std::make_shared<Atomic>("b");
    auto c = std::make_shared<Atomic>("c");
    auto p = std::make_shared<Predicate>("p", "p", std::vector<std::string>{"params1", "params2"});
    auto q = std::make_shared<Predicate>("q", "q", std::vector<std::string>{"params1", "params2", "params3"});

    fg.addAtomic(a, 1.0);
    tg.addAtomic(a, 2.0);
    fg.addAtomic(b, 2.0);
    tg.addAtomic(b, 1.0);
    fg.addAtomic(c, 3.0);
    tg.addAtomic(c, 3.0);
    fg.addPredicate(p, 3.0);
    tg.addPredicate(p, 1.0);
    fg.addPredicate(q, 2.0);
    tg.addPredicate(q, 2.0);

    fg.addOperator(notSymbol, 1.0);
    fg.addOperator(implicationSymbol, 5.0);
    fg.addOperator(andSymbol, 3.0);
    fg.addOperator(orSymbol, 2.0);
    fg.addOperator(sinceSymbol, 1.0);
    fg.addOperator(globallySymbol, 1.0);

    Formula f1 = fg.generateFormula(7);
    f1.setId("R1");
    Formula f2 = fg.generateFormula(3);
    f2.setId("R2");
    Formula f3 = fg.generateFormula(5);
    f3.setId("R3");
    std::vector<Formula> formulas = {f1, f2, f3};

    Trace trace = tg.generateTrace(60);
    size_t numExecutions{1000};

    // call the evaluator
    std::cout << "Test Running ... " << std::endl;
    executeEvaluate2(formulas, numExecutions, trace);
    std::cout << "Test Finished" << std::endl;
}

void evalFormula1(const std::vector<Formula> &formulas, size_t numExecutions, std::vector<size_t> &traceLength) {
    std::cout << "FormulaSet1: " << '\n';
    for (const auto &length : traceLength) {
        std::cout << "Trace Length: " << length << '\n';
        executeEval(formulas, numExecutions, length);
        std::cout << "Assumption: " << 0.01 * length * 4 + 0.1 * length * 4 << '\n';
    }
}

void evalFormula1Atomic(const std::vector<Formula> &formulas, size_t numExecutions, std::vector<size_t> &traceLength) {
    std::cout << "FormulaSet1: " << '\n';
    for (const auto &length : traceLength) {
        std::cout << "Trace Length: " << length << '\n';
        executeEval(formulas, numExecutions, length);
        std::cout << "Assumption: " << 0.01 * length * 4 + 0.1 * length * 4 << '\n';
    }
}

void evalFormula2(const std::vector<Formula> &formulas, size_t numExecutions, std::vector<size_t> &traceLength) {
    std::cout << "FormulaSet2: " << '\n';
    for (const auto &length : traceLength) {
        std::cout << "Trace Length: " << length << '\n';
        executeEval(formulas, numExecutions, length);
        std::cout << "Assumption: " << 0.01 * length * 2 + 0.1 * length * 2 + 0.5 * length + length << '\n';
    }
}

void evalFormula2Atomic(const std::vector<Formula> &formulas, size_t numExecutions, std::vector<size_t> &traceLength) {
    std::cout << "FormulaSet2 Atomic: " << '\n';
    for (const auto &length : traceLength) {
        std::cout << "Trace Length: " << length << '\n';
        executeEval(formulas, numExecutions, length);
    }
}