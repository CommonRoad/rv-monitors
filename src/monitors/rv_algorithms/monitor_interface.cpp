//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "monitor_interface.h"

#include <utility>

MonitorInterface::MonitorInterface(Formula formula, AlgorithmType algorithm, bool formulaCompatibilityCheck,
                                   bool earlyTermination)
    : formulas({std::move(formula)}), algorithm(algorithm), earlyTermination(earlyTermination) {
    if (formulaCompatibilityCheck and
        !std::any_of(supportedFormulaTypes.at(this->algorithm).begin(), supportedFormulaTypes.at(this->algorithm).end(),
                     [formula](MonitoringProperties formulaType) { return formula.checkCompatibility(formulaType); }))
        throw std::logic_error("Formula is not supported by monitor!");
}

MonitorInterface::MonitorInterface(std::vector<Formula> formulas, AlgorithmType algorithm,
                                   bool formulaCompatibilityCheck, bool earlyTermination)
    : formulas(std::move(formulas)), algorithm(algorithm), earlyTermination(earlyTermination) {
    if (formulaCompatibilityCheck)
        for (const auto &formula : this->formulas)
            if (!std::any_of(
                    supportedFormulaTypes.at(this->algorithm).begin(), supportedFormulaTypes.at(this->algorithm).end(),
                    [formula](MonitoringProperties formulaType) { return formula.checkCompatibility(formulaType); }))
                throw std::logic_error("Formula is not supported by monitor: " + formula.to_string());
}

const std::vector<Formula> &MonitorInterface::getFormulas() const { return formulas; }

const std::map<AlgorithmType, std::vector<MonitoringProperties>> supportedFormulaTypes{
    {AlgorithmType::hydra_mtl, {{TemporalFormulaType::safetyGeneral, true}}},
    {AlgorithmType::hydra_2_mtl, {{TemporalFormulaType::safetyGeneral, true}}},
    {AlgorithmType::btt_fsm, {{TemporalFormulaType::futureOnly, false}, {TemporalFormulaType::pastSafety, false}}},
    {AlgorithmType::dynamic_programming_ltl,
     {{TemporalFormulaType::futureOnly, false}, {TemporalFormulaType::pastSafety, false}}},
    {AlgorithmType::event_consuming_ltl,
     {{TemporalFormulaType::futureOnly, false}, {TemporalFormulaType::pastSafety, false}}},
    {AlgorithmType::mt_fsm, {{TemporalFormulaType::futureOnly, false}, {TemporalFormulaType::pastSafety, false}}},
    {AlgorithmType::event_consuming_mtl, {{TemporalFormulaType::safetyGeneral, true, true}}},

};
