//
// Created by Sebastian Maierhofer on 20.01.21.
//

#include "RVMonitors.h"


std::ostream &operator<<(std::ostream &os, const RVAlgorithms::AlgorithmTypes &algorithm) {
    switch (algorithm) {
        case RVAlgorithms::dynamic:
            os << "DynamicProgramming";
            break;
        case RVAlgorithms::eventconsuming_ltl:
            os << "EventConsuming-LTL";
            break;
        case RVAlgorithms::mt_fsm:
            os << "MT-FSM*";
            break;
        case RVAlgorithms::btt_fsm:
            os << "BTT-FSM*";
            break;
        case RVAlgorithms::eventconsuming_mtl:
            os << "EventConsuming-MTL";
            break;
    }
    return os;
}

RVAlgorithms::AlgorithmTypes stringToAlgorithm(const std::string &algorithmString){
    if(algorithmString == "dynamicprogramming"){
        return RVAlgorithms::dynamic;
    }
    else if(algorithmString == "eventconsuming-ltl"){
        return RVAlgorithms::eventconsuming_ltl;
    }
    else if(algorithmString == "eventconsuming-mtl"){
        return RVAlgorithms::eventconsuming_mtl;
    }
    else if(algorithmString == "mt-fsm*"){
        return RVAlgorithms::mt_fsm;
    }
    else if(algorithmString == "btt-fsm*"){
        return RVAlgorithms::btt_fsm;
    }
    else{
        throw std::runtime_error("Algorithm::stringToAlgorithm Command line argument " + algorithmString + " is not a valid algorithm.\n"
                                                                                                           "Options are: dynamicprogramming, eventconsuming-ltl, eventconsuming-mtl, mt-fsm*, btt-fsm*");
    }
}