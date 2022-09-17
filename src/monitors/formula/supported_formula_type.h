//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2022 - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//

#pragma once

#include <set>

enum TemporalFormulaType {
    futureOnly,    //**< Formula can only have future operators. */
    pastOnly,      //**< Formula can only have past operators. */
    safetyGeneral, //**< Formula must start with globally operator. Rest of formula can be arbitrary. */
    futureSafety,  //**< Formula must start with globally operator. Rest of formula must only contain future operators.
                   //*/
    pastSafety,    //**< Formula must start with globally operator. Rest of formula must only contain past operators. */
    futurePastCombined //**< Formula can consist of arbitrary combination of operators. */
};

struct MonitoringProperties {
    const TemporalFormulaType formulaType; //**< Supported temporal formula type. */
    const bool interval;                   //**< Boolean indicating that a formula can have intervals. */
    const bool equidistantTime;            //**< Boolean indicating that the trace must have equidistant time steps. */

    /**
     *
     * @param formulaType Supported temporal formula type.
     * @param interval Boolean indicating that a formula can have intervals.
     * @param equidistantTime Boolean indicating that the trace must have equidistant time steps.
     */
    MonitoringProperties(const TemporalFormulaType &formulaType, bool interval = true, bool equidistantTime = false);
};
