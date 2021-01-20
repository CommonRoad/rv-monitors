//
// Created by Sebastian Maierhofer on 20.01.21.
//

#include "RVMonitors.h"

RVAlgorithms::std::ostream &operator<<(std::ostream &os, const AlgorithmTypes &algorithm) {
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

RVAlgorithms::AlgorithmTypes stringToAlgorithm(const std::string &algorithmString){
    if(algorithmString == "dynamicprogramming"){
        return dynamic;
    }
    else if(algorithmString == "eventconsuming-ltl"){
        return eventconsuming_ltl;
    }
    else if(algorithmString == "eventconsuming-mtl"){
        return eventconsuming_mtl;
    }
    else if(algorithmString == "mt-fsm*"){
        return mt_fsm;
    }
    else if(algorithmString == "btt-fsm*"){
        return btt_fsm;
    }
    else{
        throw std::runtime_error("Algorithm::stringToAlgorithm Command line argument " + algorithmString + " is not a valid algorithm.\n"
                                                                                                           "Options are: dynamicprogramming, eventconsuming-ltl, eventconsuming-mtl, mt-fsm*, btt-fsm*");
    }
}