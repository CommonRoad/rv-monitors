//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "rv_monitor.h"
#include "monitors/rv_algorithms/event_consuming/event_consuming.h"
#include "monitors/rv_algorithms/mt_fsm/mt_fsm.h"
#include "monitors/rv_algorithms/mtl/mtl_event_consuming.h"
#include "monitors/utils/utils.h"

/**
 * Executes algorithm on a given trace and with given parameters.
 * @param formula Formula.
 * @param algorithm Monitoring algorithm.
 * @param trace Trace.
 * @param past True if formula is in PLTLf.
 * @param consoleOutput True if console output is desired.
 */
bool RVMonitor::verify(Trace trace) {
    if (algorithm > event_consuming_mtl) {
        throw std::runtime_error("main::verify() No valid argument for algorithm given. Try MT_FSM, BTT_FSM, DYNAMIC, "
                                 "or EVENTCONSUMING-LTL, EVENTCONSUMING-MTL.)");
    }
    auto predicates = Utils::getAtomics(formula);
    bool verdict = false;
    std::chrono::high_resolution_clock::time_point startTime;
    if (performanceMeasurement)
        startTime = ExecutionStatistic::start();
    switch (algorithm) {
    case mt_fsm: {
        verdict = mtFSM->evaluate(trace).at(0).b;
        break;
    }
    case btt_fsm: {
        verdict = bttFSM->evaluate(trace).at(0).b;
        break;
    }
    case dynamic_programming_ltl: {
        verdict = dynamic_programming->evaluate(trace).at(0).b;
        break;
    }
    case event_consuming_ltl: {
        auto formulaCopy = formula.deepCopy();
        verdict = ltl_event_consuming->evaluate(trace).at(0).b;
        break;
    }
    case event_consuming_mtl:
        auto formulaCopy = formula.deepCopy();
        verdict = MTLEventConsuming::eventConsuming(formulaCopy, trace);
        break;
    }
    if (performanceMeasurement)
        executionStatistic.stop(startTime, trace.getLength());
    return verdict;
}
/**
 * Easier output for algorithms.
 * @param os
 * @param algorithm
 * @return osstream for algorithm.
 */
std::ostream &operator<<(std::ostream &os, const AlgorithmType &algorithm) {
    switch (algorithm) {
    case dynamic_programming_ltl:
        os << "DynamicProgramming";
        break;
    case event_consuming_ltl:
        os << "EventConsuming-LTL";
        break;
    case mt_fsm:
        os << "MT-FSM*";
        break;
    case btt_fsm:
        os << "BTT-FSM*";
        break;
    case event_consuming_mtl:
        os << "EventConsuming-MTL";
        break;
    }
    return os;
}

AlgorithmType stringToAlgorithm(const std::string &algorithmString) {
    if (algorithmString == "dynamicprogramming") {
        return dynamic_programming_ltl;
    } else if (algorithmString == "eventconsuming-ltl") {
        return event_consuming_ltl;
    } else if (algorithmString == "eventconsuming-mtl") {
        return event_consuming_mtl;
    } else if (algorithmString == "mt-fsm*") {
        return mt_fsm;
    } else if (algorithmString == "btt-fsm*") {
        return btt_fsm;
    } else {
        throw std::runtime_error("Algorithm::stringToAlgorithm Command line argument " + algorithmString +
                                 " is not a valid algorithm.\n Options are: dynamicprogramming, eventconsuming-ltl, "
                                 "eventconsuming-mtl, mt-fsm*, btt-fsm*");
    }
}

RVMonitor::RVMonitor(AlgorithmType algorithm, const Formula &formula, bool past, bool measurement): algorithm(algorithm), formula(formula), performanceMeasurement(measurement) {

    if (algorithm == mt_fsm)
        mtFSM = std::make_unique<MtFsm>(formula, past);
    else if (algorithm == btt_fsm)
        bttFSM = std::make_unique<BttFsm>(formula, past);
    else if (algorithm == dynamic_programming_ltl)
        dynamic_programming = std::make_unique<DynamicProgrammingLTL>(formula, past);
    else if (algorithm == event_consuming_mtl)
        ltl_event_consuming = std::make_unique<EventConsumingLTL>(formula, past);
}
const ExecutionStatistic &RVMonitor::getExecutionStatistic() const { return executionStatistic; }
