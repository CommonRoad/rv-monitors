//
// Created by sebastian on 11.01.22.
//

#include "hydra_test_utils.h"
#include <gtest/gtest.h>

size_t hydraTestUtils::executeHydraEval(Trace trace, const std::vector<Formula> &formulas,
                                        const std::vector<std::vector<Verdict>> &expSolution,
                                        const DomainCollection &domainCollection,
                                        const FunctionCollection &functionCollection) {
    std::array<size_t, 9> numFailures;
    numFailures.fill(0);
    auto monitor1{Hydra2(formulas, domainCollection, functionCollection, false, false, false)};
    numFailures[0] = evalHydra(formulas, expSolution, monitor1, trace, "fff");
    evalHydra(formulas, expSolution, monitor1, trace, "fff"); // execute trace on monitor again to check whether resetting of variables works
    auto monitor2{Hydra2(formulas, domainCollection, functionCollection, false, false, true)};
    numFailures[1] = evalHydra(formulas, expSolution, monitor2, trace, "fft");
    evalHydra(formulas, expSolution, monitor2, trace, "fft");
    auto monitor3{Hydra2(formulas, domainCollection, functionCollection, false, true, false)};
    numFailures[2] = evalHydra(formulas, expSolution, monitor3, trace, "ftf");
    evalHydra(formulas, expSolution, monitor3, trace, "ftf");
    auto monitor4{Hydra2(formulas, domainCollection, functionCollection, false, true, true)};
    numFailures[3] = evalHydra(formulas, expSolution, monitor4, trace, "ftt");
    evalHydra(formulas, expSolution, monitor4, trace, "ftt");
    auto monitor5{Hydra2(formulas, domainCollection, functionCollection, true, false, false)};
    numFailures[4] = evalHydra(formulas, expSolution, monitor5, trace, "tff");
    evalHydra(formulas, expSolution, monitor5, trace, "tff");
    auto monitor6{Hydra2(formulas, domainCollection, functionCollection, true, false, true)};
    numFailures[5] = evalHydra(formulas, expSolution, monitor6, trace, "tft");
    evalHydra(formulas, expSolution, monitor6, trace, "tft");
    auto monitor7{Hydra2(formulas, domainCollection, functionCollection, true, true, false)};
    numFailures[6] = evalHydra(formulas, expSolution, monitor7, trace, "ttf");
    evalHydra(formulas, expSolution, monitor7, trace, "ttf");
    auto monitor9{Hydra2(formulas, domainCollection, functionCollection, true, true, true)};
    numFailures[7] = evalHydra(formulas, expSolution, monitor9, trace, "ttt");
    evalHydra(formulas, expSolution, monitor9, trace, "ttt");
    return *std::max_element(numFailures.begin(), numFailures.end());
}

size_t hydraTestUtils::evalHydra(const std::vector<Formula> &formulas,
                                 const std::vector<std::vector<Verdict>> &expSolution, Hydra2 &monitor, Trace &trace,
                                 const std::string &evaluation) {
    size_t failure{0};
    auto solution{monitor.evaluate(trace)};
    for (int i{0}; i < formulas.size(); ++i) {
        if (solution.at(formulas[i].getId()).size() != expSolution[i].size())
            throw std::logic_error("Hydra Test /" + evaluation + " formula " + formulas[i].getId() +
                                   ": length of expected solution and solution does not match");
        bool formulaWrong{false};
        for (size_t j{0}; j < expSolution[i].size(); ++j) {
            EXPECT_TRUE(expSolution[i][j] == solution.at(formulas[i].getId())[j])
                << evaluation + " formula " + formulas[i].getId() + ": Mismatch at element index: " << j;
            if (!(expSolution[i][j] == solution.at(formulas[i].getId())[j]))
                formulaWrong = true;
        }
        if (formulaWrong)
            failure++;
    }
    return failure;
}