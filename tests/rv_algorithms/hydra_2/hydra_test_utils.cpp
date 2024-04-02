#include "hydra_test_utils.h"
#include <gtest/gtest.h>

#include <utility>

bool hydraTestUtils::higher_than_time_step(size_t timeStep, size_t val) { return val > timeStep; }

std::tuple<size_t, size_t> hydraTestUtils::twoSizeTArgs(std::vector<DomainValue> values) {
    return {std::any_cast<size_t>(values.at(0).getValue()), std::any_cast<size_t>(values.at(1).getValue())};
}

bool hydraTestUtils::equal_to_time_step(size_t timeStep, size_t value) { return timeStep == value; }

std::tuple<size_t, size_t, size_t> hydraTestUtils::threeSizeTArgs(std::vector<DomainValue> values) {
    return {std::any_cast<size_t>(values.at(0).getValue()), std::any_cast<size_t>(values.at(1).getValue()),
            std::any_cast<size_t>(values.at(2).getValue())};
}

bool hydraTestUtils::equal_to_time_step_with_unused_arg(size_t timeStep, size_t randomValue, size_t value) {
    return timeStep == value;
}

bool hydraTestUtils::higher_than_zero(size_t timeStep, size_t val) { return val > 0; }

bool hydraTestUtils::divisible_by_time_step(size_t timeStep, size_t val) {
    if (timeStep == 0)
        return false;
    return val % timeStep == 0;
}

bool hydraTestUtils::lower_than_time_step(size_t timeStep, size_t val) { return val < timeStep; }

bool hydraTestUtils::equals_three(size_t timeStep, size_t val) { return val == 3; }

Trace hydraTestUtils::createTraceWithVariable(size_t length,
                                              const std::vector<std::shared_ptr<IFunction<bool>>> &function,
                                              const ConstantCollection &constants) {
    std::vector<Event> eventList;
    for (size_t timeStepIdx{0}; timeStepIdx < length; ++timeStepIdx) {
        eventList.emplace_back(timeStepIdx, timeStepIdx == length - 1);
    }
    return Trace(eventList, std::move(function), constants);
}

size_t hydraTestUtils::executeHydraEval(Trace trace, const std::vector<Formula> &formulas,
                                        const std::vector<std::vector<Verdict>> &expSolution,
                                        const std::vector<std::shared_ptr<DomainStorage>> &domains) {
    std::array<size_t, 9> numFailures{};
    numFailures.fill(0);
    auto monitor1{Hydra2(formulas, false, false, false, false)};
    numFailures[0] = evalHydra(formulas, expSolution, monitor1, trace, "fff", domains);
    evalHydra(formulas, expSolution, monitor1, trace, "fff",
              domains); // execute trace on monitor again to check whether resetting of variables works
    auto monitor2{Hydra2(formulas, false, false, true, false)};
    numFailures[1] = evalHydra(formulas, expSolution, monitor2, trace, "fft", domains);
    evalHydra(formulas, expSolution, monitor2, trace, "fft", domains);
    auto monitor3{Hydra2(formulas, false, true, false, false)};
    numFailures[2] = evalHydra(formulas, expSolution, monitor3, trace, "ftf", domains);
    evalHydra(formulas, expSolution, monitor3, trace, "ftf", domains);
    auto monitor4{Hydra2(formulas, false, true, true, false)};
    numFailures[3] = evalHydra(formulas, expSolution, monitor4, trace, "ftt", domains);
    evalHydra(formulas, expSolution, monitor4, trace, "ftt", domains);
    auto monitor5{Hydra2(formulas, true, false, false, false)};
    numFailures[4] = evalHydra(formulas, expSolution, monitor5, trace, "tff", domains);
    evalHydra(formulas, expSolution, monitor5, trace, "tff", domains);
    auto monitor6{Hydra2(formulas, true, false, true, false)};
    numFailures[5] = evalHydra(formulas, expSolution, monitor6, trace, "tft", domains);
    evalHydra(formulas, expSolution, monitor6, trace, "tft", domains);
    auto monitor7{Hydra2(formulas, true, true, false, false)};
    numFailures[6] = evalHydra(formulas, expSolution, monitor7, trace, "ttf", domains);
    evalHydra(formulas, expSolution, monitor7, trace, "ttf", domains);
    auto monitor9{Hydra2(formulas, true, true, true, false)};
    numFailures[7] = evalHydra(formulas, expSolution, monitor9, trace, "ttt", domains);
    evalHydra(formulas, expSolution, monitor9, trace, "ttt", domains);
    return *std::max_element(numFailures.begin(), numFailures.end());
}

size_t hydraTestUtils::evalHydra(const std::vector<Formula> &formulas,
                                 const std::vector<std::vector<Verdict>> &expSolution, Hydra2 &monitor, Trace &trace,
                                 const std::string &evaluation,
                                 const std::vector<std::shared_ptr<DomainStorage>> &domains) {
    size_t failure{0};
    auto solution{monitor.evaluate(trace, domains)};
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