//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2022 - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "supported_formula_type.h"

SupportedFormulaType::SupportedFormulaType(const TemoralFormulaType &formulaType, const bool interval)
    : formulaType(formulaType), interval(interval) {}
