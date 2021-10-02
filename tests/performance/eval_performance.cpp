//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//

#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra/hydra.h"
#include "monitors/trace/trace.h"
#include <chrono>
#include <iostream>
#include <random>
#include <thread>

typedef std::mt19937 MyRNG;
uint32_t seed_val = 123456789;
MyRNG rng;
std::uniform_int_distribution<uint8_t> uint_dist2(0, 1);

bool a() {
    rng.seed(seed_val);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    return uint_dist2(rng) == 1;
}

bool b() {
    rng.seed(seed_val);
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    return uint_dist2(rng) == 1;
}

bool c() {
    rng.seed(seed_val);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    return uint_dist2(rng) == 1;
}

bool d() {
    rng.seed(seed_val);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
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
    const std::map<std::string, std::function<bool()>> atomicsAndFunctions{
        {"a", a}, {"b", b}, {"c", c}, {"d", d}};
    rng.seed(seed_val);

    for (; timeStepIdx < length; ++timeStepIdx) {
        std::map<std::string, lazy_bool> event;
        for (auto const &[atomic, valueFetcher] : atomicsAndFunctions)
            event[atomic] = lazy_bool(valueFetcher);
        eventList.emplace_back(event, timeStepIdx, timeStepIdx == length - 1);
        event.clear();
    }
    return Trace(eventList);
}

void evalHydraCombined(Trace &trace, const std::vector<Formula> &formulas) {
    auto monitor{Hydra(formulas)};
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    monitor.evaluate(trace);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
              << "[ms]" << std::endl;
    // std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()
    // << "[µs]" << std::endl;
    //  std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count()
    //  << "[ns]" << std::endl;
}

void evalHydraSingle(Trace &trace, const std::vector<Formula> &formulas) {
    std::vector<Hydra> monitors;
    monitors.reserve(formulas.size());
    for (const auto &form : formulas)
        monitors.push_back(Hydra({form}, false));
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    for (size_t i{0}; i < monitors.size(); ++i)
        monitors[i].evaluate(trace);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
              << "[ms]" << std::endl;
    // std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()
    // << "[µs]" << std::endl;
    //  std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count()
    //  << "[ns]" << std::endl;
}

int main() {
    Trace trace = createRandomTrace({"a", "b", "c", "d", "e", "f", "g", "i", "j"}, 200000);
    //  Trace trace = createRandomLazyTrace(10);
    //    std::vector<Formula> formulas = {Parser::parse("(a)S[0, 40](b)"), Parser::parse("(c)U[0, 40](d)"),
    //                                     Parser::parse("((e)U[0, 40](f)) and g"), Parser::parse("X(i) and P(j)")};
    //    std::vector<Formula> formulas = {Parser::parse("(a)S[0, 4](b)"), Parser::parse("(a)U[0, 4](b)"),
    //                                     Parser::parse("((a)U[0, 4](b)) and a"),
    //                                     Parser::parse("(a)S[0, 4](b) or (a)U[0, 4](b)")};
    std::vector<Formula> formulas = {Parser::parse("(a)S[0, 4](b) or (a)U[0, 4](b)")};
    formulas[0].setId("R1");
    //    formulas[1].setId("R2");
    //    formulas[2].setId("R3");
    //    formulas[3].setId("R4");
    evalHydraCombined(trace, formulas);
    evalHydraSingle(trace, formulas);
}
