#include "hydra_test_utils.h"
#include <gtest/gtest.h>

size_t hydraTestUtils::executeHydraEval(const std::vector<Formula> &formulas,
                                        const std::vector<std::vector<Verdict>> &expSolution,
                                        const std::shared_ptr<Trace> &trace) {
    auto monitor{Hydra(formulas)};
    bool formulaWrong{evalHydra(formulas, expSolution, monitor, trace)};
    evalHydra(formulas, expSolution, monitor, trace);
    if (formulaWrong)
        return 1;
    else
        return 0;
}
bool hydraTestUtils::evalHydra(const std::vector<Formula> &formulas,
                               const std::vector<std::vector<Verdict>> &expSolution, Hydra &monitor,
                               const std::shared_ptr<Trace> &trace) {
    auto solution{monitor.evaluate(trace)};
    bool formulaWrong;
    for (int i{0}; i < formulas.size(); ++i) {
        if (solution.at(formulas[i].getId()).size() != expSolution[i].size())
            throw std::logic_error("Hydra Test / formula " + formulas[i].getId() +
                                   ": length of expected solution and solution does not match");

        for (size_t j{0}; j < expSolution[i].size(); ++j) {
            EXPECT_TRUE(expSolution[i][j] == solution.at(formulas[i].getId())[j])
                << " formula " + formulas[i].getId() + ": Mismatch at element index: " << j;
            if (!(expSolution[i][j] == solution.at(formulas[i].getId())[j]))
                formulaWrong = true;
        }
    }
    return formulaWrong;
}
