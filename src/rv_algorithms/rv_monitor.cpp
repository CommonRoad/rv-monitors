//
// Created by Sebastian Maierhofer on 20.01.21.
//

#include "rv_monitor.h"

#include "btt_fsm/btt_fsm_creation.h"
#include "btt_fsm/btt_fsm_evaluation.h"
#include "dynamic_programming/dynamic.h"
#include "event_consuming/event_consuming.h"
#include "mt_fsm/mt_fsm_creation.h"
#include "mt_fsm/mt_fsm_evaluation.h"
#include "mtl/mtl_event_consuming.h"
#include "utils/timer.h"
#include "utils/utils.h"
#include <utility>

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

    timer creationTimer, evaluationTimer;

    bool verdict = false;

    //    constexpr size_t textwidth = 15;
    //    if (consoleOutput) {
    //        std::cout << std::setw(textwidth) << std::left << "Algorithm" << ": " << algorithm << std::endl;
    //    }

    switch (algorithm) {
    case mt_fsm: {
        evaluationTimer.start();
        verdict = MT_FSMEvaluation::evaluateMT_FSM(*mtFSM, trace, past);
        evaluationTimer.stop();
        break;
    }
    case btt_fsm: {
        evaluationTimer.start();
        verdict = BTT_FSMEvaluation::evaluateBTT_FSM(*bttFSM, trace, past);
        evaluationTimer.stop();
        break;
    }
    case dynamic: {
        evaluationTimer.start();
        verdict = Dynamic::dynamic(formula, trace, past);
        evaluationTimer.stop();
        break;
    }
    case eventconsuming_ltl: {
        auto formulaCopy = formula.deepCopy();
        evaluationTimer.start();
        verdict = EventConsuming::eventConsuming(formulaCopy, trace, past);
        evaluationTimer.stop();
        break;
    }
    case eventconsuming_mtl:
        auto formulaCopy = formula.deepCopy();
        evaluationTimer.start();
        verdict = MTLEventConsuming::eventConsuming(formulaCopy, trace);
        evaluationTimer.stop();
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

RVMonitor::RVMonitor(bool past, AlgorithmType algorithm, bool consoleOutput, const Formula &formula)
    : past(past), algorithm(algorithm), consoleOutput(consoleOutput), formula(formula) {

    if (algorithm == mt_fsm)
        mtFSM = std::make_unique<MtFsm>(MT_FSMCreation::formulaToMT_FSM(formula, past));
    else if (algorithm == btt_fsm)
        bttFSM = std::make_unique<BttFsm>(BTT_FSMCreation::formulaToBTT_FSM(formula, past));
}
