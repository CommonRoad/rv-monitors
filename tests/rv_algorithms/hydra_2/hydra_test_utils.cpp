//
// Created by sebastian on 11.01.22.
//

#include "hydra_test_utils.h"
#include <gtest/gtest.h>

void hydraTestUtils::executeHydraEval(Trace &trace, const std::vector<Formula> &formulas,
                                      const std::vector<std::vector<Verdict>> &expSolution) {
    //    auto monitor1{Hydra(formulas, false, false, false)};
    //    evalHydra(formulas, expSolution, monitor1, trace, "fff");
    //    auto monitor2{Hydra(formulas, false, false, true)};
    //    evalHydra(formulas, expSolution, monitor2, trace, "fft");
    //    auto monitor3{Hydra(formulas, false, true, false)};
    //    evalHydra(formulas, expSolution, monitor3, trace, "ftf");
    //    auto monitor4{Hydra(formulas, false, true, true)};
    //    evalHydra(formulas, expSolution, monitor4, trace, "ftt");
    auto monitor5{Hydra(formulas, true, false, false)};
    evalHydra(formulas, expSolution, monitor5, trace, "tff");
    //    auto monitor6{Hydra(formulas, true, false, true)};
    //    evalHydra(formulas, expSolution, monitor6, trace, "tft");
    //    auto monitor7{Hydra(formulas, true, true, false)};
    //    evalHydra(formulas, expSolution, monitor7, trace, "ttf");
    //    auto monitor9{Hydra(formulas, true, true, true)};
    //    evalHydra(formulas, expSolution, monitor9, trace, "ttt");
}

void hydraTestUtils::evalHydra(const std::vector<Formula> &formulas,
                               const std::vector<std::vector<Verdict>> &expSolution, Hydra &monitor, Trace &trace,
                               const std::string &evaluation) {
    auto solution{monitor.evaluate(trace)};
    for (int i{0}; i < formulas.size(); ++i) {
        if (solution.at(formulas[i].getId()).size() != expSolution[i].size())
            throw std::logic_error("Hydra Test /" + evaluation +
                                   ": length of expected solution and solution does not match");
        for (size_t j{0}; j < expSolution[i].size(); ++j)
            EXPECT_TRUE(expSolution[i][j] == solution.at(formulas[i].getId())[j])
                << evaluation + ": Mismatch at element index: " << j;
    }
}