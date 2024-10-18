#pragma once

#include "monitors/formula/formula.h"
#include "monitors/rv_algorithms/hydra_2/hydra.h"
#include "monitors/utils/utils.h"

namespace hydraTestUtils {
std::shared_ptr<Trace> createTraceWithVariable(size_t length,
                                               const std::vector<std::shared_ptr<IFunction<bool>>> &function,
                                               const ConstantCollection &constants = {},
                                               std::vector<Event> eventList = {});

size_t executeHydraEval(const std::shared_ptr<Trace> &trace, const std::vector<Formula> &formulas,
                        const std::vector<std::vector<Verdict>> &expSolution,
                        const std::vector<std::shared_ptr<DomainStorage>> &domains = {});

size_t evalHydra(const std::vector<Formula> &formulas, const std::vector<std::vector<Verdict>> &expSolution,
                 Hydra2 &monitor, const std::shared_ptr<Trace> &trac, const std::string &evaluation,
                 const std::vector<std::shared_ptr<DomainStorage>> &domains);

bool higher_than_time_step(size_t timeStep, size_t val);

std::tuple<size_t, size_t> twoSizeTArgs(std::vector<DomainValue> values);

std::tuple<size_t, size_t, size_t> threeSizeTArgs(std::vector<DomainValue> values);

bool higher_than_zero(size_t timeStep, size_t val);

bool equal_to_time_step(size_t timeStep, size_t value);

bool equal_to_time_step_with_unused_arg(size_t timeStep, size_t randomValue, size_t value);

bool equals_three(size_t timeStep, size_t val);

bool divisible_by_time_step(size_t timeStep, size_t val);

bool lower_than_time_step(size_t timeStep, size_t val);

bool raiseIfStepLessThanTwo(size_t timeStep, size_t randomValue);

} // namespace hydraTestUtils
