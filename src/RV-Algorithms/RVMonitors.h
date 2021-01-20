//
// Created by Sebastian Maierhofer on 18.01.21.
//

#ifndef RV_MONITORS_RVMONITORS_H
#define RV_MONITORS_RVMONITORS_H

#include <iostream>
#include <vector>


namespace RVAlgorithms {
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
    std::ostream &operator<<(std::ostream &os, const AlgorithmTypes &algorithm);

    /**
     * Converts a string into the corresponding algorithm.
     * @param algorithmString
     * @return
     */
    AlgorithmTypes stringToAlgorithm(const std::string &algorithmString);

}
class RVMonitors {
private:
    bool past{false};
    size_t runs{1};
    std::vector<size_t> delays{0};
    std::vector<bool> lazyModes{false};
    std::vector<RVAlgorithms::AlgorithmTypes> algorithms{RVAlgorithms::dynamic};
    bool rebuildFSMsEveryRun{false};
    bool consoleOutput{true};
    std::string outputFile{"Evaluation.csv"};
};


#endif //RV_MONITORS_RVMONITORS_H
