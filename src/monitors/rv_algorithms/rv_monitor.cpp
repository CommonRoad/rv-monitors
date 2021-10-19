//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "rv_monitor.h"
#include "monitors/rv_algorithms/hydra/hydra.h"
#include "monitors/rv_algorithms/ltl_event_consuming/event_consuming.h"
#include "monitors/rv_algorithms/mt_fsm/mt_fsm.h"
#include "monitors/utils/utils.h"

std::map<std::string, std::vector<Verdict>> RVMonitor::verify(Trace trace) {
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
    case hydra_mtl:
        os << "Hydra-MTL";
        break;
    }
    return os;
}

AlgorithmType stringToAlgorithm(const std::string &algorithmString) {
    if (algorithmString == "dynamic-programming-ltl") {
        return dynamic_programming_ltl;
    } else if (algorithmString == "event-consuming-ltl") {
        return event_consuming_ltl;
    } else if (algorithmString == "hydra-mtl") {
        return hydra_mtl;
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

RVMonitor::RVMonitor(AlgorithmType algorithm, const std::vector<Formula> &formulas, bool past, bool measurement)
    : performanceMeasurement(measurement) {
    switch (algorithm) {
    case dynamic_programming_ltl:
        monitor = std::make_unique<DynamicProgrammingLTL>(formulas, past);
        break;
    case event_consuming_ltl:
        monitor = std::make_unique<EventConsumingLTL>(formulas, past);
        break;
    case mt_fsm:
        monitor = std::make_unique<MtFsm>(formulas, past);
        break;
    case btt_fsm:
        monitor = std::make_unique<BttFsm>(formulas, past);
        break;
    case hydra_mtl:
        monitor = std::make_unique<Hydra>(formulas);
        break;
    }
}

RVMonitor::RVMonitor(AlgorithmType algorithm, const Formula &formula, bool past, bool measurement)
    : performanceMeasurement(measurement) {
    switch (algorithm) {
    case dynamic_programming_ltl:
        monitor = std::make_unique<DynamicProgrammingLTL>(std::vector<Formula>{formula}, past);
        break;
    case event_consuming_ltl:
        monitor = std::make_unique<EventConsumingLTL>(std::vector<Formula>{formula}, past);
        break;
    case mt_fsm:
        monitor = std::make_unique<MtFsm>(std::vector<Formula>{formula}, past);
        break;
    case btt_fsm:
        monitor = std::make_unique<BttFsm>(std::vector<Formula>{formula}, past);
        break;
    case hydra_mtl:
        monitor = std::make_unique<Hydra>(std::vector<Formula>{formula});
        break;
    }
}

const ExecutionStatistic &RVMonitor::getExecutionStatistic() const { return executionStatistic; }

RVMonitor::RVMonitor(const RVMonitor &obj) {
    if (auto *dynLTL = dynamic_cast<DynamicProgrammingLTL *>(obj.monitor.get()))
        monitor = std::make_unique<DynamicProgrammingLTL>(dynLTL->getFormulas(), dynLTL->isPastEvaluation());
    else if (auto *hydra = dynamic_cast<Hydra *>(obj.monitor.get())) {
        std::vector<Formula> formulas;
        for (const auto &form : hydra->getFormulas()) {
            auto formulaCopy{form.deepCopy()};
            formulaCopy.setId(form.getId());
            formulas.push_back(formulaCopy);
        }
        monitor = std::make_unique<Hydra>(formulas);
    }
    // TODO other algorithms
    executionStatistic = obj.executionStatistic;
    performanceMeasurement = obj.performanceMeasurement;
}

RVMonitor &RVMonitor::operator=(const RVMonitor &other) {
    if (this != &other) { // prevent self-assignment{
        if (auto *dynLTL = dynamic_cast<DynamicProgrammingLTL *>(other.monitor.get()))
            monitor = std::make_unique<DynamicProgrammingLTL>(dynLTL->getFormulas(), dynLTL->isPastEvaluation());
        else if (auto *hydra = dynamic_cast<Hydra *>(other.monitor.get())) {
            std::vector<Formula> formulas;
            for (const auto &form : hydra->getFormulas()) {
                auto formulaCopy{form.deepCopy()};
                formulaCopy.setId(form.getId());
                formulas.push_back(formulaCopy);
            }

            monitor = std::make_unique<Hydra>(formulas);
        }
        // TODO other algorithms
        executionStatistic = other.executionStatistic;
        performanceMeasurement = other.performanceMeasurement;
    }
    return *this;
}
