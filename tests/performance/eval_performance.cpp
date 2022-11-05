//
// Created by Sebastian Maierhofer and Evald Nexhipi.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//

#include "monitors/parser/parser.h"
#include "monitors/rv_algorithms/btt_fsm/btt_fsm.h"
#include "monitors/rv_algorithms/dynamic_programming/dynamic.h"
#include "monitors/rv_algorithms/hydra/hydra.h"
#include "monitors/rv_algorithms/hydra_2/hydra.h"
#include "monitors/rv_algorithms/ltl_event_consuming/event_consuming.h"
#include "monitors/rv_algorithms/mt_fsm/mt_fsm.h"
#include "monitors/rv_algorithms/mtl_event_consuming/mtl_event_consuming.h"
#include "monitors/trace/trace.h"
#include "monitors/utils/generators/computation_time_generator/computation_time_generator.h"
#include "monitors/utils/generators/formula_generator/formula_generator.h"
#include "monitors/utils/generators/trace_generator/trace_generator.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <thread>

typedef std::mt19937 MyRNG;
uint32_t seed_val = 123456789;
MyRNG rng;
std::uniform_int_distribution<uint8_t> uint_dist2(0, 1);

void print_predicates(std::vector<std::vector<std::shared_ptr<Expression>> *> &allPredicates) {
    size_t delay = 1;
    for (auto &predicates : allPredicates) {
        std::cout << "Predicates generated on delay [" << delay << "] : ";
        for (auto &pred : *predicates) {
            std::cout << pred->getSymbol() << " ";
        }
        delay++;
        std::cout << std::endl;
    }
}

void executeEval(const std::vector<Formula> &formulas, size_t numExecutions, size_t traceLength);
void evalFormula1(const std::vector<Formula> &formulas, size_t numExecutions, std::vector<size_t> &traceLength);
void evalFormula2(const std::vector<Formula> &formulas, size_t numExecutions, std::vector<size_t> &traceLength);
void evalFormula1Atomic(const std::vector<Formula> &formulas, size_t numExecutions, std::vector<size_t> &traceLength);
void evalFormula2Atomic(const std::vector<Formula> &formulas, size_t numExecutions, std::vector<size_t> &traceLength);
void evalUniversalOperator(size_t maxArgs, size_t maxDomain, size_t traceLength);
void evalRewriting(size_t maxDomain, size_t traceLength);

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
        event_map_t event;
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
        event_map_t event;
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
    auto monitor{Hydra2(formulas)};
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    monitor.evaluate(trace);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
}

double evalHydraSingleConnected(Trace &trace, const std::vector<Formula> &formulas) {
    std::vector<Hydra2> monitors;
    monitors.reserve(formulas.size());
    for (const auto &form : formulas)
        monitors.push_back(Hydra2({form}));
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    for (size_t i{0}; i < monitors.size(); ++i)
        monitors[i].evaluate(trace);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
}

double evalHydraSingle(Trace &trace, const std::vector<Formula> &formulas) {
    std::vector<Hydra2> monitors;
    monitors.reserve(formulas.size());
    for (const auto &form : formulas)
        monitors.push_back(Hydra2({form}, {}, {}, false));
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

double evaluateSingleHydra(const Formula &formula, Trace &trace) {
    auto monitor{Hydra({formula})};
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    monitor.evaluate(trace);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
}

double executeEvaluateHydra(const Formula &formulas, size_t numExecutions, Trace &trace, std::ofstream &out) {
    std::vector<double> avgCompTime;
    for (size_t i{0}; i < numExecutions; ++i) {
        avgCompTime.push_back(evaluateSingleHydra(formulas, trace));
    }
    double result = std::accumulate(avgCompTime.begin(), avgCompTime.end(), 0) / numExecutions;
    out << "[Hydra] Results: " << result << '\n';
    for (const auto &val : avgCompTime)
        out << val << "\t";
    out << '\n';

    return result;
}

double evaluateMTLSingle(const Formula &formula, Trace &trace) {
    auto monitor{MTLEventConsuming({formula})};
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    monitor.evaluate(trace);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
}

double executeEvaluateMTL(const Formula &formula, size_t numExecutions, Trace &trace, std::ofstream &out) {
    std::vector<double> avgCompTime;
    for (size_t i{0}; i < numExecutions; ++i) {
        avgCompTime.push_back(evaluateMTLSingle(formula, trace));
    }
    double result = std::accumulate(avgCompTime.begin(), avgCompTime.end(), 0) / numExecutions;
    out << "[MTL] Results: " << result << '\n';
    for (const auto &val : avgCompTime)
        out << val << "\t";
    out << '\n';

    return result;
}

double evaluateLTLSingle(const Formula &formula, Trace &trace) {
    auto monitor{EventConsumingLTL({formula})};
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    monitor.evaluate(trace);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
}

double executeEvaluateLTL(const Formula &formula, size_t numExecutions, Trace &trace, std::ofstream &out) {
    std::vector<double> avgCompTime;
    for (size_t i{0}; i < numExecutions; ++i) {
        avgCompTime.push_back(evaluateLTLSingle(formula, trace));
    }
    double result = std::accumulate(avgCompTime.begin(), avgCompTime.end(), 0) / numExecutions;

    out << "[LTL] Results: " << result << '\n';
    for (const auto &val : avgCompTime)
        out << val << "\t";
    out << '\n';

    return result;
}

double evaluateMTFsmSingle(const Formula &formula, Trace &trace) {
    auto monitor{MtFsm({formula})};
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    monitor.evaluate(trace);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
}

double executeEvaluateMTFsm(const Formula &formula, size_t numExecutions, Trace &trace, std::ofstream &out) {
    std::vector<double> avgCompTime;
    for (size_t i{0}; i < numExecutions; ++i) {
        avgCompTime.push_back(evaluateMTFsmSingle(formula, trace));
    }
    double result = std::accumulate(avgCompTime.begin(), avgCompTime.end(), 0) / numExecutions;
    out << "[MT-FSM] Results: " << result << '\n';
    for (const auto &val : avgCompTime)
        out << val << "\t";
    out << '\n';

    return result;
}

double evaluateBTTFsmSingle(const Formula &formula, Trace &trace) {
    auto monitor{BttFsm({formula})};
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    monitor.evaluate(trace);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
}

double executeEvaluateBTTFsm(const Formula &formulas, size_t numExecutions, Trace &trace, std::ofstream &out) {
    std::vector<double> avgCompTime;
    for (size_t i{0}; i < numExecutions; ++i) {
        avgCompTime.push_back(evaluateBTTFsmSingle(formulas, trace));
    }
    double result = std::accumulate(avgCompTime.begin(), avgCompTime.end(), 0) / numExecutions;
    out << "[BTT-FSM] Results: " << result << '\n';
    for (const auto &val : avgCompTime)
        out << val << "\t";
    out << '\n';

    return result;
}

double evaluateDynamicLTLSingle(const Formula &formula, Trace &trace) {
    auto monitor{DynamicProgrammingLTL({formula})};
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    monitor.evaluate(trace);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
}

double executeEvaluateDynamicLTL(const Formula &formula, size_t numExecutions, Trace &trace, std::ofstream &out) {
    std::vector<double> avgCompTime;
    for (size_t i{0}; i < numExecutions; ++i) {
        avgCompTime.push_back(evaluateDynamicLTLSingle(formula, trace));
    }
    double result = std::accumulate(avgCompTime.begin(), avgCompTime.end(), 0) / numExecutions;
    out << "[DynamicLTL] Results: " << result << '\n';
    for (const auto &val : avgCompTime)
        out << val << "\t";
    out << '\n';

    return result;
}

void createFormulasAndTracesFromPredicates(std::vector<std::vector<std::shared_ptr<Expression>> *> &allPredicates,
                                           FormulaGenerator &formulaGenerator, TraceGenerator &traceGenerator,
                                           std::vector<size_t> formulaSizes, std::vector<size_t> traceSizes,
                                           size_t maxPredicateFormulaWeight, size_t maxPredicateTraceWeight,
                                           std::vector<Formula> &generatedFormulas,
                                           std::vector<Trace> &generatedTraces) {

    if (formulaSizes.size() == allPredicates.size() and traceSizes.size() == allPredicates.size()) {
        srand(time(NULL));
        size_t index{0};
        for (auto &predicates : allPredicates) {
            for (auto &pred : *predicates) {
                size_t randomExpressionWeight = (rand() % maxPredicateFormulaWeight) + 1;
                size_t randomEventWeight = (rand() % maxPredicateTraceWeight) + 1;
                if (auto atomicExpr = std::dynamic_pointer_cast<Atomic>(pred)) {
                    formulaGenerator.addAtomic(atomicExpr, randomExpressionWeight);
                    traceGenerator.addAtomic(atomicExpr, randomEventWeight);
                } else if (auto predicateExpr = std::dynamic_pointer_cast<Predicate>(pred)) {
                    formulaGenerator.addPredicate(predicateExpr, randomExpressionWeight);
                    traceGenerator.addPredicate(predicateExpr, randomEventWeight);
                }
            }
            Formula formula_i = formulaGenerator.generateFormula(formulaSizes.at(index));
            formula_i.setId("F" + std::to_string(index));
            generatedFormulas.push_back(formula_i);
            generatedTraces.push_back(traceGenerator.generateTrace(traceSizes.at(index)));
            ++index;
        }
    } else {
        throw std::logic_error(
            "eval_performance::createFormulasAndTracesFromPredicates: "
            "The number of entries for the formula and trace sizes should match the number of delays!");
    }
};

void testFormulasOnTraces(std::vector<Formula> &formulas, std::vector<Trace> &traces, size_t numExecutions,
                          std::ofstream &out) {
    if (formulas.size() == traces.size()) {
        size_t iterations = formulas.size();

        std::vector<double> averageResults(5);
        for (size_t i{0}; i < 5; ++i)
            averageResults[i] = 0;

        /* Uncomment the monitors you'd like to use in the performance evaluation */
        for (size_t iterIndex{0}; iterIndex < iterations; ++iterIndex) {
            out << "Formula[" << iterIndex << "]: " << formulas.at(iterIndex).to_string() << std::endl;
            out << "Trace Length: " << traces.at(iterIndex).getLength() << std::endl;
            averageResults[0] += executeEvaluateMTL(formulas.at(iterIndex), numExecutions, traces.at(iterIndex), out);
            // averageResults[1] += executeEvaluateMTFsm(formulas.at(iterIndex), numExecutions, traces.at(iterIndex),
            // out); averageResults[2] += executeEvaluateDynamicLTL(formulas.at(iterIndex), numExecutions,
            // traces.at(iterIndex), out); averageResults[3] += executeEvaluateLTL(formulas.at(iterIndex),
            // numExecutions, traces.at(iterIndex), out);
            averageResults[4] += executeEvaluateHydra(formulas.at(iterIndex), numExecutions, traces.at(iterIndex), out);
            // averageResults[5] += executeEvaluateBTTFsm(formulas.at(iterIndex), numExecutions, traces.at(iterIndex),
            // out);
            out << std::endl;
        }

        for (size_t i{0}; i < 5; ++i) {
            averageResults[i] = averageResults[i] / iterations;
            switch (i) {
            case 0:
                out << "[MTL]";
                break;
            case 1:
                out << "[MTFsm]";
                break;
            case 2:
                out << "[DynamicLTL]";
                break;
            case 3:
                out << "[LTL]";
                break;
            case 4:
                out << "[Hydra]";
                break;
            case 5:
                out << "[BTTFsm]";
                break;
            default:
                out << "[Unkown RV Algorithm]";
            }
            out << " Averaged Running Time: " << averageResults[i] << std::endl;
        }

    } else {
        throw std::logic_error("eval_performance::testFormulasOnTraces: number of the given formulas"
                               " does not match the number of given traces");
    }
}

void test_0();

void test_1(std::ofstream &out);

void test_2(std::ofstream &out);

void test_3(std::ofstream &out);

void test_4(std::ofstream &out);

void test_5(std::ofstream &out);

void test_6(std::ofstream &out);

void test_7(std::ofstream &out);

void test_8(std::ofstream &out);

int main() {
    /*size_t testNumber;
    std::ofstream outStream;
    outStream.open("eval_performance_results.txt");

    std::cout << "Type in the number of the corresponding test" << std::endl;
    std::cout << "[1] Mixed Operators, Large Formula Sizes " << std::endl;
    std::cout << "[2] Simple Operators, Large Trace Sizes " << std::endl;
    std::cout << "[3] Mixed Operators, Large Trace Sizes " << std::endl;
    std::cout << "[4] Simple Operators, Large Formula Sizes " << std::endl;
    std::cout << "[5] Small Intervals, Small Trace Sizes " << std::endl;
    std::cout << "[6] Large Intervals, Small Trace Sizes " << std::endl;
    std::cout << "[7] Small Intervals, Large Trace Sizes " << std::endl;
    std::cout << "[8] Large Intervals, Large Trace Sizes " << std::endl;
    std::cin >> testNumber;

    switch (testNumber) {
    case 1:
        std::cout << "Test [1] running..." << std::endl;
        test_1(outStream);
        std::cout << "Test Finished! Check results in eval_performance_results.txt";
        break;
    case 2:
        std::cout << "Test [2] running..." << std::endl;
        test_2(outStream);
        std::cout << "Test Finished! Check results in eval_performance_results.txt";
        break;
    case 3:
        std::cout << "Test [3] running..." << std::endl;
        test_3(outStream);
        std::cout << "Test Finished! Check results in eval_performance_results.txt";
        break;
    case 4:
        std::cout << "Test [4] running..." << std::endl;
        test_4(outStream);
        std::cout << "Test Finished! Check results in eval_performance_results.txt";
        break;
    case 5:
        std::cout << "Test [5] running..." << std::endl;
        test_5(outStream);
        std::cout << "Test Finished! Check results in eval_performance_results.txt";
        break;
    case 6:
        std::cout << "Test [6] running..." << std::endl;
        test_6(outStream);
        std::cout << "Test Finished! Check results in eval_performance_results.txt";
        break;
    case 7:
        std::cout << "Test [7] running..." << std::endl;
        test_7(outStream);
        std::cout << "Test Finished! Check results in eval_performance_results.txt";
        break;
    case 8:
        std::cout << "Test [8] running..." << std::endl;
        test_8(outStream);
        std::cout << "Test Finished! Check results in eval_performance_results.txt";
        break;
    default:
        std::cout << "Unkown Test!";
    }
    outStream.close();*/
    evalUniversalOperator(3, 20, 1);
}
/*
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
*/

void test_1(std::ofstream &out) {
    /* Generators */
    FormulaGenerator formulaGenerator;
    FormulaGenerator::rSeed();
    TraceGenerator traceGenerator;
    TraceGenerator::rSeed();
    ComputationTimeGenerator ctGenerator;
    ComputationTimeGenerator::rSeed();

    /* Add Operators */
    formulaGenerator.addOperator(andSymbol, 3.0);
    formulaGenerator.addOperator(orSymbol, 3.0);
    formulaGenerator.addOperator(implicationSymbol, 2.0);
    formulaGenerator.addOperator(notSymbol, 1.0);
    formulaGenerator.addOperator(futureSymbol, 1.0);
    formulaGenerator.addOperatorWithInterval(untilSymbol, 2.0, 0, 3, 2, 4);

    std::vector<std::vector<std::shared_ptr<Expression>> *> allPredicates;

    /* All atoms and predicates */
    auto atomA = std::make_shared<Atomic>("a");
    auto atomB = std::make_shared<Atomic>("b");
    auto atomC = std::make_shared<Atomic>("c");
    auto atomD = std::make_shared<Atomic>("d");
    auto predicateP = std::make_shared<Predicate>("p", "p", std::vector<std::string>{"p_1", "p_2"});
    auto predicateQ = std::make_shared<Predicate>("q", "q", std::vector<std::string>{"q_1", "q_2", "q_3"});
    auto predicateT = std::make_shared<Predicate>("t", "q", std::vector<std::string>{"t_1"});
    auto predicateV = std::make_shared<Predicate>("v", "q", std::vector<std::string>{"v_1", "v_2"});

    /* Distribution 1 */
    std::vector<std::shared_ptr<Atomic>> atoms_1 = {atomA, atomB, atomC};
    std::vector<std::shared_ptr<Predicate>> predicates_1 = {predicateP, predicateQ};
    std::vector<double> atom_weights_1 = {1.0, 3.0, 1.0};
    std::vector<double> predicate_weights_1 = {2.0, 1.0};
    ctGenerator.addAtomsDistribution(atoms_1, atom_weights_1);
    ctGenerator.addPredicateDistribution(predicates_1, predicate_weights_1);

    /* Distribution 2 */
    std::vector<std::shared_ptr<Atomic>> atoms_2 = {atomA, atomB};
    std::vector<std::shared_ptr<Predicate>> predicates_2 = {predicateP, predicateQ, predicateT, predicateV};
    std::vector<double> atom_weights_2 = {3.0, 2.0};
    std::vector<double> predicate_weights_2 = {3.0, 1.0, 1.0, 2.0};
    ctGenerator.addAtomsDistribution(atoms_2, atom_weights_2);
    ctGenerator.addPredicateDistribution(predicates_2, predicate_weights_2);

    /* Distribution 3 */
    std::vector<std::shared_ptr<Atomic>> atoms_3 = {atomA, atomB, atomC, atomD};
    std::vector<std::shared_ptr<Predicate>> predicates_3 = {predicateP, predicateQ};
    std::vector<double> atom_weights_3 = {1.0, 3.0, 1.0, 1.0};
    std::vector<double> predicate_weights_3 = {2.0, 2.0};
    ctGenerator.addAtomsDistribution(atoms_3, atom_weights_3);
    ctGenerator.addPredicateDistribution(predicates_3, predicate_weights_3);

    /* Distribution 4 */
    std::vector<std::shared_ptr<Atomic>> atoms_4 = {atomA, atomB};
    std::vector<std::shared_ptr<Predicate>> predicates_4 = {predicateP, predicateQ};
    std::vector<double> atom_weights_4 = {1.0, 1.0};
    std::vector<double> predicate_weights_4 = {2.0, 1.0};
    ctGenerator.addAtomsDistribution(atoms_4, atom_weights_4);
    ctGenerator.addPredicateDistribution(predicates_4, predicate_weights_4);

    ctGenerator.createPredicatesOnDelays({500, 1, 3, 1000, 2, 6, 750, 1, 5, 2000, 1, 4}, allPredicates);
    std::vector<Formula> generatedFormulas{};
    std::vector<Trace> generatedTraces{};

    createFormulasAndTracesFromPredicates(allPredicates, formulaGenerator, traceGenerator, {5, 10, 15, 20},
                                          {10, 10, 10, 10}, 5, 5, generatedFormulas, generatedTraces);

    size_t numExecutions{5};
    testFormulasOnTraces(generatedFormulas, generatedTraces, numExecutions, out);
}

void test_2(std::ofstream &out) {
    /* Generators */
    FormulaGenerator formulaGenerator;
    FormulaGenerator::rSeed();
    TraceGenerator traceGenerator;
    TraceGenerator::rSeed();
    ComputationTimeGenerator ctGenerator;
    ComputationTimeGenerator::rSeed();

    /* Add Operators */
    formulaGenerator.addOperator(andSymbol, 3.0);
    formulaGenerator.addOperator(orSymbol, 3.0);
    formulaGenerator.addOperator(implicationSymbol, 2.0);
    formulaGenerator.addOperator(notSymbol, 2.0);

    std::vector<std::vector<std::shared_ptr<Expression>> *> allPredicates;

    /* All atoms and predicates */
    auto atomA = std::make_shared<Atomic>("a");
    auto atomB = std::make_shared<Atomic>("b");
    auto atomC = std::make_shared<Atomic>("c");
    auto atomD = std::make_shared<Atomic>("d");
    auto predicateP = std::make_shared<Predicate>("p", "p", std::vector<std::string>{"p_1", "p_2"});
    auto predicateQ = std::make_shared<Predicate>("q", "q", std::vector<std::string>{"q_1", "q_2", "q_3"});
    auto predicateT = std::make_shared<Predicate>("t", "q", std::vector<std::string>{"t_1"});
    auto predicateV = std::make_shared<Predicate>("v", "q", std::vector<std::string>{"v_1", "v_2"});

    /* Distribution 1 */
    std::vector<std::shared_ptr<Atomic>> atoms_1 = {atomA, atomB, atomC};
    std::vector<std::shared_ptr<Predicate>> predicates_1 = {predicateP, predicateQ};
    std::vector<double> atom_weights_1 = {1.0, 3.0, 1.0};
    std::vector<double> predicate_weights_1 = {2.0, 1.0};
    ctGenerator.addAtomsDistribution(atoms_1, atom_weights_1);
    ctGenerator.addPredicateDistribution(predicates_1, predicate_weights_1);

    /* Distribution 2 */
    std::vector<std::shared_ptr<Atomic>> atoms_2 = {atomA, atomB};
    std::vector<std::shared_ptr<Predicate>> predicates_2 = {predicateP, predicateQ, predicateT, predicateV};
    std::vector<double> atom_weights_2 = {3.0, 2.0};
    std::vector<double> predicate_weights_2 = {3.0, 1.0, 1.0, 2.0};
    ctGenerator.addAtomsDistribution(atoms_2, atom_weights_2);
    ctGenerator.addPredicateDistribution(predicates_2, predicate_weights_2);

    /* Distribution 3 */
    std::vector<std::shared_ptr<Atomic>> atoms_3 = {atomA, atomB, atomC, atomD};
    std::vector<std::shared_ptr<Predicate>> predicates_3 = {predicateP, predicateQ};
    std::vector<double> atom_weights_3 = {1.0, 3.0, 1.0, 1.0};
    std::vector<double> predicate_weights_3 = {2.0, 2.0};
    ctGenerator.addAtomsDistribution(atoms_3, atom_weights_3);
    ctGenerator.addPredicateDistribution(predicates_3, predicate_weights_3);

    ctGenerator.createPredicatesOnDelays({500, 1, 3, 1000, 2, 6, 750, 1, 5}, allPredicates);
    std::vector<Formula> generatedFormulas{};
    std::vector<Trace> generatedTraces{};

    createFormulasAndTracesFromPredicates(allPredicates, formulaGenerator, traceGenerator, {3, 3, 3}, {100, 200, 300},
                                          5, 5, generatedFormulas, generatedTraces);

    size_t numExecutions{3};
    testFormulasOnTraces(generatedFormulas, generatedTraces, numExecutions, out);
}

void test_3(std::ofstream &out) {
    /* Generators */
    FormulaGenerator formulaGenerator;
    FormulaGenerator::rSeed();
    TraceGenerator traceGenerator;
    TraceGenerator::rSeed();
    ComputationTimeGenerator ctGenerator;
    ComputationTimeGenerator::rSeed();

    /* Add Operators */
    formulaGenerator.addOperator(andSymbol, 3.0);
    formulaGenerator.addOperator(orSymbol, 3.0);
    formulaGenerator.addOperator(implicationSymbol, 2.0);
    formulaGenerator.addOperator(notSymbol, 2.0);
    formulaGenerator.addOperator(equivalenceSymbol, 1.0);
    formulaGenerator.addOperatorWithInterval(untilSymbol, 1.0, 0, 3, 2, 5);
    formulaGenerator.addOperatorWithInterval(futureSymbol, 1.0, 0, 3, 2, 5);

    std::vector<std::vector<std::shared_ptr<Expression>> *> allPredicates;

    /* All atoms and predicates */
    auto atomA = std::make_shared<Atomic>("a");
    auto atomB = std::make_shared<Atomic>("b");
    auto atomC = std::make_shared<Atomic>("c");
    auto atomD = std::make_shared<Atomic>("d");
    auto predicateP = std::make_shared<Predicate>("p", "p", std::vector<std::string>{"p_1", "p_2"});
    auto predicateQ = std::make_shared<Predicate>("q", "q", std::vector<std::string>{"q_1", "q_2", "q_3"});
    auto predicateT = std::make_shared<Predicate>("t", "q", std::vector<std::string>{"t_1"});
    auto predicateV = std::make_shared<Predicate>("v", "q", std::vector<std::string>{"v_1", "v_2"});

    /* Distribution 1 */
    std::vector<std::shared_ptr<Atomic>> atoms_1 = {atomA, atomB, atomC};
    std::vector<std::shared_ptr<Predicate>> predicates_1 = {predicateP, predicateQ};
    std::vector<double> atom_weights_1 = {1.0, 3.0, 1.0};
    std::vector<double> predicate_weights_1 = {2.0, 1.0};
    ctGenerator.addAtomsDistribution(atoms_1, atom_weights_1);
    ctGenerator.addPredicateDistribution(predicates_1, predicate_weights_1);

    /* Distribution 2 */
    std::vector<std::shared_ptr<Atomic>> atoms_2 = {atomA, atomB};
    std::vector<std::shared_ptr<Predicate>> predicates_2 = {predicateP, predicateQ, predicateT, predicateV};
    std::vector<double> atom_weights_2 = {3.0, 2.0};
    std::vector<double> predicate_weights_2 = {3.0, 1.0, 1.0, 2.0};
    ctGenerator.addAtomsDistribution(atoms_2, atom_weights_2);
    ctGenerator.addPredicateDistribution(predicates_2, predicate_weights_2);

    /* Distribution 3 */
    std::vector<std::shared_ptr<Atomic>> atoms_3 = {atomA, atomB, atomC, atomD};
    std::vector<std::shared_ptr<Predicate>> predicates_3 = {predicateP, predicateQ};
    std::vector<double> atom_weights_3 = {1.0, 3.0, 1.0, 1.0};
    std::vector<double> predicate_weights_3 = {2.0, 2.0};
    ctGenerator.addAtomsDistribution(atoms_3, atom_weights_3);
    ctGenerator.addPredicateDistribution(predicates_3, predicate_weights_3);

    ctGenerator.createPredicatesOnDelays({500, 1, 3, 1000, 2, 6, 750, 1, 5}, allPredicates);
    std::vector<Formula> generatedFormulas{};
    std::vector<Trace> generatedTraces{};

    createFormulasAndTracesFromPredicates(allPredicates, formulaGenerator, traceGenerator, {3, 3, 3}, {100, 200, 300},
                                          5, 5, generatedFormulas, generatedTraces);

    size_t numExecutions{3};
    testFormulasOnTraces(generatedFormulas, generatedTraces, numExecutions, out);
}

void test_4(std::ofstream &out) {
    /* Generators */
    FormulaGenerator formulaGenerator;
    FormulaGenerator::rSeed();
    TraceGenerator traceGenerator;
    TraceGenerator::rSeed();
    ComputationTimeGenerator ctGenerator;
    ComputationTimeGenerator::rSeed();

    /* Add Operators */
    formulaGenerator.addOperator(andSymbol, 3.0);
    formulaGenerator.addOperator(orSymbol, 3.0);
    formulaGenerator.addOperator(implicationSymbol, 2.0);

    std::vector<std::vector<std::shared_ptr<Expression>> *> allPredicates;

    /* All atoms and predicates */
    auto atomA = std::make_shared<Atomic>("a");
    auto atomB = std::make_shared<Atomic>("b");
    auto atomC = std::make_shared<Atomic>("c");
    auto atomD = std::make_shared<Atomic>("d");
    auto predicateP = std::make_shared<Predicate>("p", "p", std::vector<std::string>{"p_1", "p_2"});
    auto predicateQ = std::make_shared<Predicate>("q", "q", std::vector<std::string>{"q_1", "q_2", "q_3"});
    auto predicateT = std::make_shared<Predicate>("t", "q", std::vector<std::string>{"t_1"});
    auto predicateV = std::make_shared<Predicate>("v", "q", std::vector<std::string>{"v_1", "v_2"});

    /* Distribution 1 */
    std::vector<std::shared_ptr<Atomic>> atoms_1 = {atomA, atomB, atomC};
    std::vector<std::shared_ptr<Predicate>> predicates_1 = {predicateP, predicateQ};
    std::vector<double> atom_weights_1 = {1.0, 3.0, 1.0};
    std::vector<double> predicate_weights_1 = {2.0, 1.0};
    ctGenerator.addAtomsDistribution(atoms_1, atom_weights_1);
    ctGenerator.addPredicateDistribution(predicates_1, predicate_weights_1);

    /* Distribution 2 */
    std::vector<std::shared_ptr<Atomic>> atoms_2 = {atomA, atomB};
    std::vector<std::shared_ptr<Predicate>> predicates_2 = {predicateP, predicateQ, predicateT, predicateV};
    std::vector<double> atom_weights_2 = {3.0, 2.0};
    std::vector<double> predicate_weights_2 = {3.0, 1.0, 1.0, 2.0};
    ctGenerator.addAtomsDistribution(atoms_2, atom_weights_2);
    ctGenerator.addPredicateDistribution(predicates_2, predicate_weights_2);

    /* Distribution 3 */
    std::vector<std::shared_ptr<Atomic>> atoms_3 = {atomA, atomB, atomC, atomD};
    std::vector<std::shared_ptr<Predicate>> predicates_3 = {predicateP, predicateQ};
    std::vector<double> atom_weights_3 = {1.0, 3.0, 1.0, 1.0};
    std::vector<double> predicate_weights_3 = {2.0, 2.0};
    ctGenerator.addAtomsDistribution(atoms_3, atom_weights_3);
    ctGenerator.addPredicateDistribution(predicates_3, predicate_weights_3);

    ctGenerator.createPredicatesOnDelays({500, 1, 3, 1000, 2, 6, 750, 1, 5}, allPredicates);
    std::vector<Formula> generatedFormulas{};
    std::vector<Trace> generatedTraces{};

    createFormulasAndTracesFromPredicates(allPredicates, formulaGenerator, traceGenerator, {5, 10, 15}, {10, 10, 10}, 5,
                                          5, generatedFormulas, generatedTraces);

    size_t numExecutions{1};
    testFormulasOnTraces(generatedFormulas, generatedTraces, numExecutions, out);
}

void test_5(std::ofstream &out) {
    /* Generators */
    FormulaGenerator formulaGenerator;
    FormulaGenerator::rSeed();
    TraceGenerator traceGenerator;
    TraceGenerator::rSeed();
    ComputationTimeGenerator ctGenerator;
    ComputationTimeGenerator::rSeed();

    /* Add Operators */
    formulaGenerator.addOperator(andSymbol, 2.0);
    formulaGenerator.addOperator(orSymbol, 2.0);
    formulaGenerator.addOperator(implicationSymbol, 1.0);
    formulaGenerator.addOperator(notSymbol, 1.0);
    formulaGenerator.addOperatorWithInterval(untilSymbol, 3.0, 0, 1, 2, 3);
    // formulaGenerator.addOperatorWithInterval(sinceSymbol, 3.0, 0, 1, 2, 3);
    formulaGenerator.addOperatorWithInterval(futureSymbol, 1.0, 0, 1, 2, 3);
    // formulaGenerator.addOperatorWithInterval(onceSymbol, 1.0, 0, 1, 2, 3);

    std::vector<std::vector<std::shared_ptr<Expression>> *> allPredicates;

    /* All atoms and predicates */
    auto atomA = std::make_shared<Atomic>("a");
    auto atomB = std::make_shared<Atomic>("b");
    auto atomC = std::make_shared<Atomic>("c");
    auto atomD = std::make_shared<Atomic>("d");
    auto predicateP = std::make_shared<Predicate>("p", "p", std::vector<std::string>{"p_1", "p_2"});
    auto predicateQ = std::make_shared<Predicate>("q", "q", std::vector<std::string>{"q_1", "q_2", "q_3"});
    auto predicateT = std::make_shared<Predicate>("t", "q", std::vector<std::string>{"t_1"});
    auto predicateV = std::make_shared<Predicate>("v", "q", std::vector<std::string>{"v_1", "v_2"});

    /* Distribution 1 */
    std::vector<std::shared_ptr<Atomic>> atoms_1 = {atomA, atomB, atomC};
    std::vector<std::shared_ptr<Predicate>> predicates_1 = {predicateP, predicateQ};
    std::vector<double> atom_weights_1 = {1.0, 3.0, 1.0};
    std::vector<double> predicate_weights_1 = {2.0, 1.0};
    ctGenerator.addAtomsDistribution(atoms_1, atom_weights_1);
    ctGenerator.addPredicateDistribution(predicates_1, predicate_weights_1);

    /* Distribution 2 */
    std::vector<std::shared_ptr<Atomic>> atoms_2 = {atomA, atomB};
    std::vector<std::shared_ptr<Predicate>> predicates_2 = {predicateP, predicateQ, predicateT, predicateV};
    std::vector<double> atom_weights_2 = {3.0, 2.0};
    std::vector<double> predicate_weights_2 = {3.0, 1.0, 1.0, 2.0};
    ctGenerator.addAtomsDistribution(atoms_2, atom_weights_2);
    ctGenerator.addPredicateDistribution(predicates_2, predicate_weights_2);

    /* Distribution 3 */
    std::vector<std::shared_ptr<Atomic>> atoms_3 = {atomA, atomB, atomC, atomD};
    std::vector<std::shared_ptr<Predicate>> predicates_3 = {predicateP, predicateQ};
    std::vector<double> atom_weights_3 = {1.0, 3.0, 1.0, 1.0};
    std::vector<double> predicate_weights_3 = {2.0, 2.0};
    ctGenerator.addAtomsDistribution(atoms_3, atom_weights_3);
    ctGenerator.addPredicateDistribution(predicates_3, predicate_weights_3);

    ctGenerator.createPredicatesOnDelays({500, 1, 3, 1000, 2, 6, 750, 1, 5}, allPredicates);
    std::vector<Formula> generatedFormulas{};
    std::vector<Trace> generatedTraces{};

    createFormulasAndTracesFromPredicates(allPredicates, formulaGenerator, traceGenerator, {5, 4, 3}, {10, 30, 50}, 5,
                                          5, generatedFormulas, generatedTraces);

    size_t numExecutions{10};
    testFormulasOnTraces(generatedFormulas, generatedTraces, numExecutions, out);
}

void test_6(std::ofstream &out) {
    /* Generators */
    FormulaGenerator formulaGenerator;
    FormulaGenerator::rSeed();
    TraceGenerator traceGenerator;
    TraceGenerator::rSeed();
    ComputationTimeGenerator ctGenerator;
    ComputationTimeGenerator::rSeed();

    /* Add Operators */
    formulaGenerator.addOperator(andSymbol, 2.0);
    formulaGenerator.addOperator(orSymbol, 2.0);
    formulaGenerator.addOperator(implicationSymbol, 2.0);
    formulaGenerator.addOperator(notSymbol, 2.0);
    formulaGenerator.addOperatorWithInterval(untilSymbol, 3.0, 0, 1, 10, 30);
    formulaGenerator.addOperatorWithInterval(sinceSymbol, 3.0, 0, 1, 10, 30);
    formulaGenerator.addOperatorWithInterval(futureSymbol, 1.0, 0, 1, 10, 30);
    formulaGenerator.addOperatorWithInterval(onceSymbol, 1.0, 0, 1, 10, 30);

    std::vector<std::vector<std::shared_ptr<Expression>> *> allPredicates;

    /* All atoms and predicates */
    auto atomA = std::make_shared<Atomic>("a");
    auto atomB = std::make_shared<Atomic>("b");
    auto atomC = std::make_shared<Atomic>("c");
    auto atomD = std::make_shared<Atomic>("d");
    auto predicateP = std::make_shared<Predicate>("p", "p", std::vector<std::string>{"p_1", "p_2"});
    auto predicateQ = std::make_shared<Predicate>("q", "q", std::vector<std::string>{"q_1", "q_2", "q_3"});
    auto predicateT = std::make_shared<Predicate>("t", "q", std::vector<std::string>{"t_1"});
    auto predicateV = std::make_shared<Predicate>("v", "q", std::vector<std::string>{"v_1", "v_2"});

    /* Distribution 1 */
    std::vector<std::shared_ptr<Atomic>> atoms_1 = {atomA, atomB, atomC};
    std::vector<std::shared_ptr<Predicate>> predicates_1 = {predicateP, predicateQ};
    std::vector<double> atom_weights_1 = {1.0, 3.0, 1.0};
    std::vector<double> predicate_weights_1 = {2.0, 1.0};
    ctGenerator.addAtomsDistribution(atoms_1, atom_weights_1);
    ctGenerator.addPredicateDistribution(predicates_1, predicate_weights_1);

    /* Distribution 2 */
    std::vector<std::shared_ptr<Atomic>> atoms_2 = {atomA, atomB};
    std::vector<std::shared_ptr<Predicate>> predicates_2 = {predicateP, predicateQ, predicateT, predicateV};
    std::vector<double> atom_weights_2 = {3.0, 2.0};
    std::vector<double> predicate_weights_2 = {3.0, 1.0, 1.0, 2.0};
    ctGenerator.addAtomsDistribution(atoms_2, atom_weights_2);
    ctGenerator.addPredicateDistribution(predicates_2, predicate_weights_2);

    /* Distribution 3 */
    std::vector<std::shared_ptr<Atomic>> atoms_3 = {atomA, atomB, atomC, atomD};
    std::vector<std::shared_ptr<Predicate>> predicates_3 = {predicateP, predicateQ};
    std::vector<double> atom_weights_3 = {1.0, 3.0, 1.0, 1.0};
    std::vector<double> predicate_weights_3 = {2.0, 2.0};
    ctGenerator.addAtomsDistribution(atoms_3, atom_weights_3);
    ctGenerator.addPredicateDistribution(predicates_3, predicate_weights_3);

    ctGenerator.createPredicatesOnDelays({500, 1, 3, 1000, 2, 6, 750, 1, 5}, allPredicates);
    std::vector<Formula> generatedFormulas{};
    std::vector<Trace> generatedTraces{};

    createFormulasAndTracesFromPredicates(allPredicates, formulaGenerator, traceGenerator, {5, 4, 3}, {10, 30, 50}, 5,
                                          5, generatedFormulas, generatedTraces);
    size_t numExecutions{10};
    testFormulasOnTraces(generatedFormulas, generatedTraces, numExecutions, out);
}

void test_7(std::ofstream &out) {
    /* Generators */
    FormulaGenerator formulaGenerator;
    FormulaGenerator::rSeed();
    TraceGenerator traceGenerator;
    TraceGenerator::rSeed();
    ComputationTimeGenerator ctGenerator;
    ComputationTimeGenerator::rSeed();

    /* Add Operators */
    formulaGenerator.addOperator(andSymbol, 2.0);
    formulaGenerator.addOperator(orSymbol, 2.0);
    formulaGenerator.addOperator(implicationSymbol, 2.0);
    formulaGenerator.addOperator(notSymbol, 2.0);
    formulaGenerator.addOperatorWithInterval(untilSymbol, 3.0, 0, 1, 2, 3);
    formulaGenerator.addOperatorWithInterval(sinceSymbol, 3.0, 0, 1, 2, 3);
    formulaGenerator.addOperatorWithInterval(futureSymbol, 1.0, 0, 1, 2, 3);
    formulaGenerator.addOperatorWithInterval(onceSymbol, 1.0, 0, 1, 2, 3);

    std::vector<std::vector<std::shared_ptr<Expression>> *> allPredicates;

    /* All atoms and predicates */
    auto atomA = std::make_shared<Atomic>("a");
    auto atomB = std::make_shared<Atomic>("b");
    auto atomC = std::make_shared<Atomic>("c");
    auto atomD = std::make_shared<Atomic>("d");
    auto predicateP = std::make_shared<Predicate>("p", "p", std::vector<std::string>{"p_1", "p_2"});
    auto predicateQ = std::make_shared<Predicate>("q", "q", std::vector<std::string>{"q_1", "q_2", "q_3"});
    auto predicateT = std::make_shared<Predicate>("t", "q", std::vector<std::string>{"t_1"});
    auto predicateV = std::make_shared<Predicate>("v", "q", std::vector<std::string>{"v_1", "v_2"});

    /* Distribution 1 */
    std::vector<std::shared_ptr<Atomic>> atoms_1 = {atomA, atomB, atomC};
    std::vector<std::shared_ptr<Predicate>> predicates_1 = {predicateP, predicateQ};
    std::vector<double> atom_weights_1 = {1.0, 3.0, 1.0};
    std::vector<double> predicate_weights_1 = {2.0, 1.0};
    ctGenerator.addAtomsDistribution(atoms_1, atom_weights_1);
    ctGenerator.addPredicateDistribution(predicates_1, predicate_weights_1);

    /* Distribution 2 */
    std::vector<std::shared_ptr<Atomic>> atoms_2 = {atomA, atomB};
    std::vector<std::shared_ptr<Predicate>> predicates_2 = {predicateP, predicateQ, predicateT, predicateV};
    std::vector<double> atom_weights_2 = {3.0, 2.0};
    std::vector<double> predicate_weights_2 = {3.0, 1.0, 1.0, 2.0};
    ctGenerator.addAtomsDistribution(atoms_2, atom_weights_2);
    ctGenerator.addPredicateDistribution(predicates_2, predicate_weights_2);

    /* Distribution 3 */
    std::vector<std::shared_ptr<Atomic>> atoms_3 = {atomA, atomB, atomC, atomD};
    std::vector<std::shared_ptr<Predicate>> predicates_3 = {predicateP, predicateQ};
    std::vector<double> atom_weights_3 = {1.0, 3.0, 1.0, 1.0};
    std::vector<double> predicate_weights_3 = {2.0, 2.0};
    ctGenerator.addAtomsDistribution(atoms_3, atom_weights_3);
    ctGenerator.addPredicateDistribution(predicates_3, predicate_weights_3);

    ctGenerator.createPredicatesOnDelays({500, 1, 3, 1000, 2, 6, 750, 1, 5}, allPredicates);
    std::vector<Formula> generatedFormulas{};
    std::vector<Trace> generatedTraces{};

    createFormulasAndTracesFromPredicates(allPredicates, formulaGenerator, traceGenerator, {3, 3, 3}, {100, 200, 300},
                                          5, 5, generatedFormulas, generatedTraces);

    size_t numExecutions{3};
    testFormulasOnTraces(generatedFormulas, generatedTraces, numExecutions, out);
}

void test_8(std::ofstream &out) {
    /* Generators */
    FormulaGenerator formulaGenerator;
    FormulaGenerator::rSeed();
    TraceGenerator traceGenerator;
    TraceGenerator::rSeed();
    ComputationTimeGenerator ctGenerator;
    ComputationTimeGenerator::rSeed();

    /* Add Operators */
    formulaGenerator.addOperator(andSymbol, 2.0);
    formulaGenerator.addOperator(orSymbol, 2.0);
    formulaGenerator.addOperator(implicationSymbol, 2.0);
    formulaGenerator.addOperator(notSymbol, 2.0);
    formulaGenerator.addOperatorWithInterval(untilSymbol, 3.0, 0, 1, 5, 20);
    formulaGenerator.addOperatorWithInterval(sinceSymbol, 3.0, 0, 1, 5, 20);
    formulaGenerator.addOperatorWithInterval(futureSymbol, 1.0, 0, 1, 5, 20);
    formulaGenerator.addOperatorWithInterval(onceSymbol, 1.0, 0, 1, 5, 20);

    std::vector<std::vector<std::shared_ptr<Expression>> *> allPredicates;

    /* All atoms and predicates */
    auto atomA = std::make_shared<Atomic>("a");
    auto atomB = std::make_shared<Atomic>("b");
    auto atomC = std::make_shared<Atomic>("c");
    auto atomD = std::make_shared<Atomic>("d");
    auto predicateP = std::make_shared<Predicate>("p", "p", std::vector<std::string>{"p_1", "p_2"});
    auto predicateQ = std::make_shared<Predicate>("q", "q", std::vector<std::string>{"q_1", "q_2", "q_3"});
    auto predicateT = std::make_shared<Predicate>("t", "q", std::vector<std::string>{"t_1"});
    auto predicateV = std::make_shared<Predicate>("v", "q", std::vector<std::string>{"v_1", "v_2"});

    /* Distribution 1 */
    std::vector<std::shared_ptr<Atomic>> atoms_1 = {atomA, atomB, atomC};
    std::vector<std::shared_ptr<Predicate>> predicates_1 = {predicateP, predicateQ};
    std::vector<double> atom_weights_1 = {1.0, 3.0, 1.0};
    std::vector<double> predicate_weights_1 = {2.0, 1.0};
    ctGenerator.addAtomsDistribution(atoms_1, atom_weights_1);
    ctGenerator.addPredicateDistribution(predicates_1, predicate_weights_1);

    /* Distribution 2 */
    std::vector<std::shared_ptr<Atomic>> atoms_2 = {atomA, atomB};
    std::vector<std::shared_ptr<Predicate>> predicates_2 = {predicateP, predicateQ, predicateT, predicateV};
    std::vector<double> atom_weights_2 = {3.0, 2.0};
    std::vector<double> predicate_weights_2 = {3.0, 1.0, 1.0, 2.0};
    ctGenerator.addAtomsDistribution(atoms_2, atom_weights_2);
    ctGenerator.addPredicateDistribution(predicates_2, predicate_weights_2);

    /* Distribution 3 */
    std::vector<std::shared_ptr<Atomic>> atoms_3 = {atomA, atomB, atomC, atomD};
    std::vector<std::shared_ptr<Predicate>> predicates_3 = {predicateP, predicateQ};
    std::vector<double> atom_weights_3 = {1.0, 3.0, 1.0, 1.0};
    std::vector<double> predicate_weights_3 = {2.0, 2.0};
    ctGenerator.addAtomsDistribution(atoms_3, atom_weights_3);
    ctGenerator.addPredicateDistribution(predicates_3, predicate_weights_3);

    ctGenerator.createPredicatesOnDelays({500, 1, 3, 1000, 2, 6, 750, 1, 5}, allPredicates);
    std::vector<Formula> generatedFormulas{};
    std::vector<Trace> generatedTraces{};

    createFormulasAndTracesFromPredicates(allPredicates, formulaGenerator, traceGenerator, {3, 3, 3}, {100, 200, 300},
                                          5, 5, generatedFormulas, generatedTraces);

    size_t numExecutions{3};
    testFormulasOnTraces(generatedFormulas, generatedTraces, numExecutions, out);
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

void evalUniversalOperator(size_t maxArgs, size_t maxDomain, size_t traceLength) {
    for (size_t numArgs = 1; numArgs <= maxArgs; numArgs++) {
        for (size_t sizeDomain = 1; sizeDomain <= maxDomain; sizeDomain += 3) {
            std::vector<Value> values;
            for (size_t i = 0; i < sizeDomain; i++)
                values.emplace_back(std::to_string(i));
            DomainCollection domainCollection{Domain{"D", values}};
            std::map<std::string, std::string> variables;
            for (size_t i = 0; i < numArgs; i++)
                variables.insert(std::make_pair("x" + std::to_string(i), "D"));
            std::string universalPart{"A "};
            std::string andPart{};
            for (size_t i = 0; i < variables.size(); i++) {
                auto iter{variables.begin()};
                std::advance(iter, i);
                if (i == variables.size() - 1) {
                    universalPart += iter->first + " in " + iter->second + ". ";
                    andPart += iter->first;
                } else {
                    universalPart += iter->first + ", ";
                    andPart += iter->first + "& ";
                }
            }
            Formula formula = Parser::parse(universalPart + andPart);
            formula.setId("R1");
            if (sizeDomain == 1)
                std::cout << "Evaluation of formula: " << formula.to_string() << std::endl;
            std::set<std::string> atomics;
            for (Value &val : values)
                atomics.insert(val.getVal());
            std::vector<double> times;
            for (size_t i = 0; i < 1000; i++) {
                std::vector<std::vector<std::string>> trueVerdicts;
                for (size_t j = 0; j < traceLength; j++) {
                    std::vector<std::string> propositions;
                    for (auto &value : values) {
                        std::random_device randomDevice;
                        std::mt19937 twister(randomDevice());
                        std::uniform_int_distribution<int> dist(1, 101);

                        if (dist(twister) != 0)
                            propositions.push_back(value.getVal());
                    }
                    trueVerdicts.emplace_back(propositions);
                }
                Trace trace = Utils::createTrace(atomics, trueVerdicts);
                Hydra2 hydra{std::vector<Formula>{formula}, domainCollection, {}, false, false, false};
                auto start{std::chrono::system_clock::now()};
                hydra.evaluate(trace);
                auto end{std::chrono::system_clock::now()};
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
                times.push_back(elapsed.count());
            }
            double time{0.};
            for (double t : times)
                time += t;
            time = time / times.size();
            std::cout << "Num. Args: " << numArgs << " Size Domain: " << sizeDomain << " Trace Length: " << traceLength
                      << " Eval. Time: " << time << std::endl;
        }
        std::cout << std::endl;
    }
}

void evalRewriting(size_t maxDomain, size_t traceLength) {
    for (size_t sizeDomain = 1; sizeDomain <= maxDomain; sizeDomain += 3) {
        std::vector<Value> values;
        for (size_t i = 0; i < sizeDomain; i++)
            values.emplace_back(std::to_string(i));
        DomainCollection domainCollection{Domain{"D", values}};
        std::map<std::string, std::string> variables;
        for (size_t i = 0; i < 2; i++)
            variables.insert(std::make_pair("x" + std::to_string(i), "D"));
        Formula formula = Parser::parse("X(X(X(X(X(X(X(X(X(X(A x1, x2 in D. x1 & x2))))))))))");
        formula.setId("R1");
        if (sizeDomain == 1)
            std::cout << "Evaluation of formula: " << formula.to_string() << std::endl;
        Hydra2 hydra{std::vector<Formula>{formula}, domainCollection, {}, false, false, false};
        std::set<std::string> atomics;
        for (Value &val : values)
            atomics.insert(val.getVal());
        std::vector<std::vector<std::string>> trueVerdicts;
        for (size_t i = 0; i < traceLength; i++)
            trueVerdicts.emplace_back(std::vector<std::string>());
        Trace trace = Utils::createTrace(atomics, trueVerdicts);
        auto start{std::chrono::system_clock::now()};
        hydra.evaluate(trace);
        auto end{std::chrono::system_clock::now()};
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Num. Args: " << 2 << " Size Domain: " << sizeDomain << " Trace Length: " << traceLength
                  << " Eval. Time: " << elapsed.count() << std::endl;
    }
    std::cout << std::endl;
}
