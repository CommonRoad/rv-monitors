//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "monitors/rv_algorithms/rv_monitor.h"
#include "monitors/rv_algorithms/hydra/hydra.h"
#include "monitors/rv_algorithms/hydra_2/hydra.h"
#include "monitors/rv_algorithms/ltl_event_consuming/event_consuming.h"
#include "monitors/rv_algorithms/mt_fsm/mt_fsm.h"
#include "monitors/rv_algorithms/mtl_event_consuming/mtl_event_consuming.h"
#include "monitors/utils/utils.h"
#include <cassert>

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
    case hydra_2_mtl:
        stream << "Hydra_2-MTL";
        break;
    case event_consuming_mtl:
        stream << "EventConsuming-MTL";
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
    } else if (algorithmString == "hydra_2-mtl") {
        return hydra_2_mtl;
    } else if (algorithmString == "mt-fsm*") {
        return mt_fsm;
    } else if (algorithmString == "btt-fsm*") {
        return btt_fsm;
    } else if (algorithmString == "event-consuming-mtl") {
        return event_consuming_mtl;
    } else {
        throw std::runtime_error(
            "Algorithm::stringToAlgorithm Command line argument " + algorithmString +
            " is not a valid algorithm.\n Options are: dynamic-programming-ltl, event-consuming-ltl, "
            "event-consuming-mtl, mt-fsm*, btt-fsm*");
    }
}

RVMonitor::RVMonitor(const std::vector<Formula> &formulas, MonitorConfiguration configuration,
                     const std::map<std::string, std::array<double, 2>> &predicateCosts,
                     const std::vector<std::shared_ptr<DomainStorage>> &domains)
    : configuration(configuration) {
    for (const auto &pred : predicateCosts)
        assert(pred.second[0] >= 0 and pred.second[0] <= 100 and pred.second[1] >= 0 and pred.second[1] <= 1);
    switch (configuration.algorithm) {
    case dynamic_programming_ltl:
        monitor = std::make_unique<DynamicProgrammingLTL>(formulas);
        break;
    case event_consuming_ltl:
        monitor = std::make_unique<EventConsumingLTL>(formulas);
        break;
    case mt_fsm:
        monitor = std::make_unique<MtFsm>(formulas);
        break;
    case btt_fsm:
        monitor = std::make_unique<BttFsm>(formulas);
        break;
    case hydra_mtl:
        monitor = std::make_unique<Hydra>(formulas);
        break;
    case hydra_2_mtl:
        monitor = std::make_unique<Hydra2>(formulas, configuration.hydraConnectedEvaluation,
                                           configuration.hydraReordering, configuration.shortCircuiting,
                                           configuration.earlyTermination, domains, predicateCosts);
        break;
    case event_consuming_mtl:
        monitor = std::make_unique<MTLEventConsuming>(formulas);
        break;
    }
}

RVMonitor::RVMonitor(const Formula &formula, MonitorConfiguration configuration,
                     const std::map<std::string, std::array<double, 2>> &predicateCosts,
                     const std::vector<std::shared_ptr<DomainStorage>> &domains)
    : configuration(configuration) {
    for (const auto &pred : predicateCosts)
        assert(pred.second[0] >= 0 and pred.second[0] <= 100 and pred.second[1] >= 0 and pred.second[1] <= 1);
    switch (configuration.algorithm) {
    case dynamic_programming_ltl:
        monitor = std::make_unique<DynamicProgrammingLTL>(std::vector<Formula>{formula});
        break;
    case event_consuming_ltl:
        monitor = std::make_unique<EventConsumingLTL>(std::vector<Formula>{formula});
        break;
    case mt_fsm:
        monitor = std::make_unique<MtFsm>(std::vector<Formula>{formula});
        break;
    case btt_fsm:
        monitor = std::make_unique<BttFsm>(std::vector<Formula>{formula});
        break;
    case hydra_mtl:
        monitor = std::make_unique<Hydra>(std::vector<Formula>{formula});
        break;
    case hydra_2_mtl:
        monitor = std::make_unique<Hydra2>(std::vector<Formula>{formula}, configuration.hydraConnectedEvaluation,
                                           configuration.hydraReordering, configuration.shortCircuiting,
                                           configuration.earlyTermination, domains, predicateCosts);
        break;
    case event_consuming_mtl:
        monitor = std::make_unique<MTLEventConsuming>(std::vector<Formula>{formula});
        break;
    }
}

const ExecutionStatistic &RVMonitor::getExecutionStatistic() const { return executionStatistic; }

RVMonitor::RVMonitor(const RVMonitor &obj) : configuration(obj.configuration) {
    if (auto *dynLTL = dynamic_cast<DynamicProgrammingLTL *>(obj.monitor.get()))
        monitor = std::make_unique<DynamicProgrammingLTL>(dynLTL->getFormulas(), dynLTL->isPastEvaluation());
    else if (auto *hydra = dynamic_cast<Hydra *>(obj.monitor.get())) {
        monitor = std::make_unique<Hydra>(hydra->getFormulas());
    } else if (auto *ecltl = dynamic_cast<EventConsumingLTL *>(obj.monitor.get())) {
        monitor = std::make_unique<EventConsumingLTL>(ecltl->getFormulas());
    } else if (auto *mtfsm = dynamic_cast<MtFsm *>(obj.monitor.get())) {
        monitor = std::make_unique<MtFsm>(mtfsm->getFormulas());
    } else if (auto *bttfsm = dynamic_cast<BttFsm *>(obj.monitor.get())) {
        monitor = std::make_unique<BttFsm>(bttfsm->getFormulas());
    } else if (auto *ecmtl = dynamic_cast<MTLEventConsuming *>(obj.monitor.get())) {
        monitor = std::make_unique<MTLEventConsuming>(ecmtl->getFormulas());
    } else if (auto *hydra2 = dynamic_cast<Hydra2 *>(obj.monitor.get())) {
        std::vector<Formula> formulas;
        for (const auto &form : hydra2->getFormulas()) {
            auto formulaCopy{form.deepCopy()};
            formulaCopy.setId(form.getId());
            formulas.push_back(formulaCopy);
        }
        monitor =
            std::make_unique<Hydra2>(formulas, configuration.hydraConnectedEvaluation, configuration.hydraReordering,
                                     configuration.shortCircuiting, configuration.earlyTermination,
                                     hydra2->getDomains(), hydra2->getPredicateCost());
    }
    executionStatistic = obj.executionStatistic;
}

RVMonitor &RVMonitor::operator=(const RVMonitor &other) {
    if (this != &other) { // prevent self-assignment
        if (auto *dynLTL = dynamic_cast<DynamicProgrammingLTL *>(other.monitor.get()))
            monitor = std::make_unique<DynamicProgrammingLTL>(dynLTL->getFormulas(), dynLTL->isPastEvaluation());
        else if (auto *hydra = dynamic_cast<Hydra2 *>(other.monitor.get())) {
            std::vector<Formula> formulas;
            for (const auto &form : hydra->getFormulas()) {
                auto formulaCopy{form.deepCopy()};
                formulaCopy.setId(form.getId());
                formulas.push_back(formulaCopy);
            }

            monitor = std::make_unique<Hydra2>(formulas, configuration.hydraConnectedEvaluation,
                                               configuration.hydraReordering, configuration.shortCircuiting,
                                               configuration.earlyTermination, hydra->getDomains(),
                                               hydra->getPredicateCost());
        }
        // TODO other algorithms
        executionStatistic = other.executionStatistic;
    }
    return *this;
}

std::string RVMonitor::to_string() { return monitor->to_string(); }

MonitorConfiguration::MonitorConfiguration(AlgorithmType algorithm, bool measurement, bool hydraConnectedEvaluation,
                                           bool hydraReordering, bool shortCircuiting, bool earlyTermination)
    : algorithm(algorithm), performanceMeasurement(measurement), hydraConnectedEvaluation(hydraConnectedEvaluation),
      hydraReordering(hydraReordering), shortCircuiting(shortCircuiting), earlyTermination(earlyTermination) {}

MonitorConfiguration::MonitorConfiguration(AlgorithmType algorithm, bool measurement)
    : algorithm(algorithm), performanceMeasurement(measurement) {}

MonitorConfiguration RVMonitor::getConfiguration() { return configuration; }
