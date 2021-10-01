//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//

#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/hydra/hydra.h"
#include "monitors/trace/trace.h"
#include "monitors/utils/utils.h"
#include <chrono>
#include <iostream>
#include <thread>

bool predicate1ms(){
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    return true;
}

bool predicate2ms(){
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    return true;
}

bool predicate5ms(){
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    return true;
}

bool predicate10ms(){
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    return true;
}

void evalHydraCombined(Trace &trace, const std::vector<Formula> &formulas) {
    auto monitor{Hydra(formulas)};
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    monitor.evaluate(trace);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
   // std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
  //  std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;

}

void evalHydraSingle(Trace &trace, const std::vector<Formula> &formulas) {
    std::vector<Hydra> monitors;
    monitors.reserve(formulas.size());
    for(const auto &form : formulas)
        monitors.push_back(Hydra({form}));
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    for(size_t i{0}; i < monitors.size(); ++i)
        monitors[i].evaluate(trace);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
    // std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
    //  std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;

}

int main(){
    Trace trace = Utils::createRandomTrace({"a", "b", "c", "d", "e", "f", "g", "i", "j"}, 200000);
    std::vector<Formula> formulas = {Parser::parse("(a)S[0, 40](b)"),
                                     Parser::parse("(c)U[0, 40](d)"),
                                     Parser::parse("((e)U[0, 40](f)) and g"),
                                     Parser::parse("X(i) and P(j)")};
    formulas[0].setId("R1");
    formulas[1].setId("R2");
    formulas[2].setId("R3");
    formulas[3].setId("R4");
    evalHydraCombined(trace, formulas);
    evalHydraSingle(trace, formulas);
}

