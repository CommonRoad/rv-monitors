//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "rv_monitor.h"
#include "monitors/rv_algorithms/ltl_event_consuming/event_consuming.h"
#include "monitors/rv_algorithms/mt_fsm/mt_fsm.h"
#include "monitors/rv_algorithms/mtl_event_consuming/mtl_event_consuming.h"
#include "monitors/utils/utils.h"

std::vector<Verdict> RVMonitor::verify(Trace trace) {
    std::chrono::high_resolution_clock::time_point startTime;
    if (performanceMeasurement)
        startTime = ExecutionStatistic::start();

    auto verdict{monitor->evaluate(trace)};

    if (performanceMeasurement)
        executionStatistic.stop(startTime, trace.getLength());
    return verdict;
}

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
    if (algorithmString == "dynamic-programming-ltl") {
        return dynamic_programming_ltl;
    } else if (algorithmString == "event-consuming-ltl") {
        return event_consuming_ltl;
    } else if (algorithmString == "event-consuming-mtl") {
        return event_consuming_mtl;
    } else if (algorithmString == "mt-fsm*") {
        return mt_fsm;
    } else if (algorithmString == "btt-fsm*") {
        return btt_fsm;
    } else {
        throw std::runtime_error(
            "Algorithm::stringToAlgorithm Command line argument " + algorithmString +
            " is not a valid algorithm.\n Options are: dynamic-programming-ltl, event-consuming-ltl, "
            "event-consuming-mtl, mt-fsm*, btt-fsm*");
    }
}

RVMonitor::RVMonitor(AlgorithmType algorithm, const Formula &formula, bool past, bool measurement)
    : performanceMeasurement(measurement) {
    switch (algorithm) {
    case dynamic_programming_ltl:
        monitor = std::make_unique<DynamicProgrammingLTL>(formula, past);
        break;
    case event_consuming_ltl:
        monitor = std::make_unique<EventConsumingLTL>(formula, past);
        break;
    case mt_fsm:
        monitor = std::make_unique<MtFsm>(formula, past);
        break;
    case btt_fsm:
        monitor = std::make_unique<BttFsm>(formula, past);
        break;
    case event_consuming_mtl:
        monitor = std::make_unique<MTLEventConsuming>(formula);
        break;
    }
}
const ExecutionStatistic &RVMonitor::getExecutionStatistic() const { return executionStatistic; }
