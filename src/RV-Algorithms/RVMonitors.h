//
// Created by Sebastian Maierhofer on 18.01.21.
//

#ifndef RV_MONITORS_RVMONITORS_H
#define RV_MONITORS_RVMONITORS_H

#include <iostream>
#include <vector>


namespace RVAlgorithms{
    enum AlgorithmTypes {
        dynamic,
        eventconsuming_ltl,
        mt_fsm,
        btt_fsm,
        eventconsuming_mtl
    };

    /**
     * Easier output for algorithms.
     * @param os
     * @param algorithm
     * @return osstream for algorithm.
     */
    std::ostream &operator<<(std::ostream &os, const AlgorithmTypes &algorithm) {
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

    /**
     * Converts a string into the corresponding algorithm.
     * @param algorithmString
     * @return
     */
    AlgorithmTypes stringToAlgorithm(const std::string &algorithmString){
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

}

class RVMonitors {
private:
    bool past{false};
    size_t runs{1};
    std::vector<size_t> delays{0};
    std::vector<bool> lazyModes{false};
    std::vector<AlgorithmTypes> algorithms{dynamic};
    bool rebuildFSMsEveryRun{false};
    bool consoleOutput{true};
    std::string outputFile{"Evaluation.csv"};
};


#endif //RV_MONITORS_RVMONITORS_H
