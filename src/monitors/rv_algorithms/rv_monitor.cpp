//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "rv_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra.h"
#include "monitors/rv_algorithms/ltl_event_consuming/event_consuming.h"
#include "monitors/rv_algorithms/mt_fsm/mt_fsm.h"
#include "monitors/utils/utils.h"

std::map<std::string, std::vector<Verdict>> RVMonitor::verify(Trace trace) {
    std::chrono::high_resolution_clock::time_point startTime;
    if (configuration.performanceMeasurement)
        startTime = ExecutionStatistic::start();

    auto verdict{monitor->evaluate(trace)};

    if (configuration.performanceMeasurement)
        executionStatistic.stop(startTime, trace.getLength());
    return verdict;
}

std::ostream &operator<<(std::ostream &stream, const AlgorithmType &algorithm) {
    switch (algorithm) {
    case dynamic_programming_ltl:
        stream << "DynamicProgramming";
        break;
    case event_consuming_ltl:
        stream << "EventConsuming-LTL";
        break;
    case mt_fsm:
        stream << "MT-FSM*";
        break;
    case btt_fsm:
        stream << "BTT-FSM*";
        break;
    case hydra_mtl:
        stream << "Hydra-MTL";
        break;
    }
    return stream;
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

RVMonitor::RVMonitor(const std::vector<Formula> &formulas, MonitorConfiguration configuration,
                     const std::map<std::string, std::array<double, 2>> &predicateCosts)
    : configuration(configuration) {
    switch (configuration.algorithm) {
    case dynamic_programming_ltl:
        monitor = std::make_unique<DynamicProgrammingLTL>(formulas, configuration.past);
        break;
    case event_consuming_ltl:
        monitor = std::make_unique<EventConsumingLTL>(formulas, configuration.past);
        break;
    case mt_fsm:
        monitor = std::make_unique<MtFsm>(formulas, configuration.past);
        break;
    case btt_fsm:
        monitor = std::make_unique<BttFsm>(formulas, configuration.past);
        break;
    case hydra_mtl:
        DomainCollection domainCollection; // TODO: Transfer domain and function collection
        FunctionCollection functionCollection;
        monitor =
            std::make_unique<Hydra>(formulas, domainCollection, functionCollection, configuration.hydraConnectedEvaluation, configuration.hydraReordering,
                                    configuration.hydraShortCircuiting, predicateCosts);
        break;
    }
}

RVMonitor::RVMonitor(const Formula &formula, MonitorConfiguration configuration,
                     const std::map<std::string, std::array<double, 2>> &predicateCosts)
    : configuration(configuration) {
    switch (configuration.algorithm) {
    case dynamic_programming_ltl:
        monitor = std::make_unique<DynamicProgrammingLTL>(std::vector<Formula>{formula}, configuration.past);
        break;
    case event_consuming_ltl:
        monitor = std::make_unique<EventConsumingLTL>(std::vector<Formula>{formula}, configuration.past);
        break;
    case mt_fsm:
        monitor = std::make_unique<MtFsm>(std::vector<Formula>{formula}, configuration.past);
        break;
    case btt_fsm:
        monitor = std::make_unique<BttFsm>(std::vector<Formula>{formula}, configuration.past);
        break;
    case hydra_mtl:
        DomainCollection domainCollection; // TODO: Transfer domain and function collection
        FunctionCollection functionCollection;
        monitor =
            std::make_unique<Hydra>(std::vector<Formula>{formula}, domainCollection, functionCollection, configuration.hydraConnectedEvaluation,
                                    configuration.hydraReordering, configuration.hydraShortCircuiting, predicateCosts);
        break;
    }
}

const ExecutionStatistic &RVMonitor::getExecutionStatistic() const { return executionStatistic; }

RVMonitor::RVMonitor(const RVMonitor &obj) : configuration(obj.configuration) {
    if (auto *dynLTL = dynamic_cast<DynamicProgrammingLTL *>(obj.monitor.get()))
        monitor = std::make_unique<DynamicProgrammingLTL>(dynLTL->getFormulas(), dynLTL->isPastEvaluation());
    else if (auto *hydra = dynamic_cast<Hydra *>(obj.monitor.get())) {
        std::vector<Formula> formulas;
        for (const auto &form : hydra->getFormulas()) {
            auto formulaCopy{form.deepCopy()};
            formulaCopy.setId(form.getId());
            formulas.push_back(formulaCopy);
        }
        DomainCollection domainCollection; // TODO: Transfer domain and function collection
        FunctionCollection functionCollection;
        monitor = std::make_unique<Hydra>(formulas, domainCollection, functionCollection, configuration.hydraConnectedEvaluation,
                                          configuration.hydraReordering, configuration.hydraShortCircuiting);
    }
    // TODO other algorithms
    executionStatistic = obj.executionStatistic;
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
            DomainCollection domainCollection; // TODO: Transfer domain and function collection
            FunctionCollection functionCollection;
            monitor = std::make_unique<Hydra>(formulas, domainCollection, functionCollection, configuration.hydraConnectedEvaluation,
                                              configuration.hydraReordering, configuration.hydraShortCircuiting);
        }
        // TODO other algorithms
        executionStatistic = other.executionStatistic;
    }
    return *this;
}

std::string RVMonitor::to_string() { return monitor->to_string(); }

MonitorConfiguration::MonitorConfiguration(AlgorithmType algorithm, bool past, bool measurement,
                                           bool hydraConnectedEvaluation, bool hydraReordering,
                                           bool hydraShortCircuiting)
    : algorithm(algorithm), past(past), performanceMeasurement(measurement),
      hydraConnectedEvaluation(hydraConnectedEvaluation), hydraReordering(hydraReordering),
      hydraShortCircuiting(hydraShortCircuiting) {}

MonitorConfiguration::MonitorConfiguration(AlgorithmType algorithm, bool past, bool measurement)
    : algorithm(algorithm), past(past), performanceMeasurement(measurement) {}
