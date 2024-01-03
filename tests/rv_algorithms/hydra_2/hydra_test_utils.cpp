//
// Created by sebastian on 11.01.22.
//

#include "hydra_test_utils.h"
#include <gtest/gtest.h>

#include <utility>

bool hydraTestUtils::higher_than_time_step(size_t timeStep, size_t val) { return val > timeStep; }

std::tuple<size_t, size_t> hydraTestUtils::twoSizeTArgs(std::vector<DomainValue> values) {
    return {std::any_cast<size_t>(values.at(0).getValue()), std::any_cast<size_t>(values.at(1).getValue())};
}

bool hydraTestUtils::equal_to_time_step(size_t timeStep, size_t value) { return timeStep == value; }

bool hydraTestUtils::higher_than_zero(size_t timeStep, size_t val) { return val > 0; }

bool hydraTestUtils::divisible_by_time_step(size_t timeStep, size_t val) {
    if (timeStep == 0)
        return false;
    return val % timeStep == 0;
}

bool hydraTestUtils::lower_than_time_step(size_t timeStep, size_t val) { return val < timeStep; }

Trace hydraTestUtils::createTraceWithVariable(size_t length, std::shared_ptr<IFunction<bool>> function) {
    std::vector<Event> eventList;
    for (size_t timeStepIdx{0}; timeStepIdx < length; ++timeStepIdx) {
        eventList.emplace_back(timeStepIdx, timeStepIdx == length - 1);
    }
    return Trace(eventList, std::move(function));
}

size_t hydraTestUtils::executeHydraEval(Trace trace, const std::vector<Formula> &formulas,
                                        const std::vector<std::vector<Verdict>> &expSolution,
                                        const std::vector<std::shared_ptr<DomainStorage>> &domains) {
    std::array<size_t, 9> numFailures;
    numFailures.fill(0);
    auto monitor1{Hydra2(formulas, false, false, false, false, domains)};
    numFailures[0] = evalHydra(formulas, expSolution, monitor1, trace, "fff");
    evalHydra(formulas, expSolution, monitor1, trace,
              "fff"); // execute trace on monitor again to check whether resetting of variables works
    auto monitor2{Hydra2(formulas, false, false, true, false, domains)};
    numFailures[1] = evalHydra(formulas, expSolution, monitor2, trace, "fft");
    evalHydra(formulas, expSolution, monitor2, trace, "fft");
    auto monitor3{Hydra2(formulas, false, true, false, false, domains)};
    numFailures[2] = evalHydra(formulas, expSolution, monitor3, trace, "ftf");
    evalHydra(formulas, expSolution, monitor3, trace, "ftf");
    auto monitor4{Hydra2(formulas, false, true, true, false, domains)};
    numFailures[3] = evalHydra(formulas, expSolution, monitor4, trace, "ftt");
    evalHydra(formulas, expSolution, monitor4, trace, "ftt");
    auto monitor5{Hydra2(formulas, true, false, false, false, domains)};
    numFailures[4] = evalHydra(formulas, expSolution, monitor5, trace, "tff");
    evalHydra(formulas, expSolution, monitor5, trace, "tff");
    auto monitor6{Hydra2(formulas, true, false, true, false, domains)};
    numFailures[5] = evalHydra(formulas, expSolution, monitor6, trace, "tft");
    evalHydra(formulas, expSolution, monitor6, trace, "tft");
    auto monitor7{Hydra2(formulas, true, true, false, false, domains)};
    numFailures[6] = evalHydra(formulas, expSolution, monitor7, trace, "ttf");
    evalHydra(formulas, expSolution, monitor7, trace, "ttf");
    auto monitor9{Hydra2(formulas, true, true, true, false, domains)};
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