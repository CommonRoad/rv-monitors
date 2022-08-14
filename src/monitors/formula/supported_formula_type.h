//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2022 - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//

#pragma once

#include <set>

enum TemoralFormulaType {
    futureOnly,    //**< Formula can only have future operators. */
    pastOnly,      //**< Formula can only have past operators. */
    safetyGeneral, //**< Formula must start with globally operator. Rest of formula can be arbitrary. */
    futureSafety,  //**< Formula must start with globally operator. Rest of formula must only contain future operators.
                   //*/
    pastSafety,    //**< Formula must start with globally operator. Rest of formula must only contain past operators. */
    futurePastCombined //**< Formula can consist of arbitrary combination of operators. */
};

struct SupportedFormulaType {
    const TemoralFormulaType formulaType;
    const bool interval; //**< The formula can have intervals. */
    SupportedFormulaType(const TemoralFormulaType &formulaType, bool interval = true);
};
