//
// Created by Sebastian Maierhofer on 20.01.21.
//

#include "rv_monitor.h"

#include "monitors/rv_algorithms/btt_fsm/btt_fsm_creation.h"
#include "monitors/rv_algorithms/btt_fsm/btt_fsm_evaluation.h"
#include "monitors/rv_algorithms/dynamic_programming/dynamic.h"
#include "monitors/rv_algorithms/event_consuming/event_consuming.h"
#include "monitors/rv_algorithms/mt_fsm/mt_fsm_creation.h"
#include "monitors/rv_algorithms/mt_fsm/mt_fsm_evaluation.h"
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
    if (algorithm > eventconsuming_mtl) {
        throw std::runtime_error("main::verify() No valid argument for algorithm given. Try MT_FSM, BTT_FSM, DYNAMIC, "
                                 "or EVENTCONSUMING-LTL, EVENTCONSUMING-MTL.)");
    }
    auto predicates = Utils::getAtomics(formula);

    bool verdict = false;

    switch (algorithm) {
    case mt_fsm: {
        executionStatistic.start();
        verdict = MT_FSMEvaluation::evaluateMT_FSM(*mtFSM, trace, past);
        executionStatistic.stop(trace.getLength());
        break;
    }
    case btt_fsm: {
        executionStatistic.start();
        verdict = BTT_FSMEvaluation::evaluateBTT_FSM(*bttFSM, trace, past);
        executionStatistic.stop(trace.getLength());
        break;
    }
    case dynamic: {
        executionStatistic.start();
        verdict = Dynamic::dynamic(formula, trace, past);
        executionStatistic.stop(trace.getLength());
        break;
    }
    case eventconsuming_ltl: {
        auto formulaCopy = formula.deepCopy();
        executionStatistic.start();
        verdict = EventConsuming::eventConsuming(formulaCopy, trace, past);
        executionStatistic.stop(trace.getLength());
        break;
    }
    case eventconsuming_mtl:
        auto formulaCopy = formula.deepCopy();
        executionStatistic.start();
        verdict = MTLEventConsuming::eventConsuming(formulaCopy, trace);
        executionStatistic.stop(trace.getLength());
        break;
    }
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
    case dynamic:
        os << "DynamicProgramming";
        break;
    case eventconsuming_ltl:
        os << "EventConsuming-LTL";
        break;
    case mt_fsm:
        os << "MT-FSM*";
        break;
    case btt_fsm:
        os << "BTT-FSM*";
        break;
    case eventconsuming_mtl:
        os << "EventConsuming-MTL";
        break;
    }
    return os;
}

AlgorithmType stringToAlgorithm(const std::string &algorithmString) {
    if (algorithmString == "dynamicprogramming") {
        return dynamic;
    } else if (algorithmString == "eventconsuming-ltl") {
        return eventconsuming_ltl;
    } else if (algorithmString == "eventconsuming-mtl") {
        return eventconsuming_mtl;
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

RVMonitor::RVMonitor(AlgorithmType algorithm, bool consoleOutput, const Formula &formula, bool past)
    : past(past), algorithm(algorithm), consoleOutput(consoleOutput), formula(formula) {

    if (algorithm == mt_fsm)
        mtFSM = std::make_unique<MtFsm>(MT_FSMCreation::formulaToMT_FSM(formula, past));
    else if (algorithm == btt_fsm)
        bttFSM = std::make_unique<BttFsm>(BTT_FSMCreation::formulaToBTT_FSM(formula, past));
}
const ExecutionStatistic &RVMonitor::getExecutionStatistic() const { return executionStatistic; }
