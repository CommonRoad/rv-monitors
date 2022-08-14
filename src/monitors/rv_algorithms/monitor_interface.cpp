//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "monitor_interface.h"

#include <utility>

MonitorInterface::MonitorInterface(Formula formula, std::vector<SupportedFormulaType> supportedFormulaTypes,
                                   bool formulaCompatibilityCheck)
    : formulas({std::move(formula)}), supportedFormulaTypes(std::move(supportedFormulaTypes)) {
    if (formulaCompatibilityCheck and
        !std::any_of(this->supportedFormulaTypes.begin(), this->supportedFormulaTypes.end(),
                     [formula](SupportedFormulaType formulaType) { return formula.checkCompatibility(formulaType); }))
        throw std::logic_error("Formula is not supported by monitor!");
}

MonitorInterface::MonitorInterface(std::vector<Formula> formulas,
                                   std::vector<SupportedFormulaType> supportedFormulaTypes,
                                   bool formulaCompatibilityCheck)
    : formulas(std::move(formulas)), supportedFormulaTypes(std::move(supportedFormulaTypes)) {
    if (formulaCompatibilityCheck)
        for (const auto &formula : this->formulas)
            if (!std::any_of(
                    this->supportedFormulaTypes.begin(), this->supportedFormulaTypes.end(),
                    [formula](SupportedFormulaType formulaType) { return formula.checkCompatibility(formulaType); }))
                throw std::logic_error("Formula is not supported by monitor: " + formula.to_string());
}

const std::vector<Formula> &MonitorInterface::getFormulas() const { return formulas; }
