#include "monitors/formula/supported_formula_type.h"

MonitoringProperties::MonitoringProperties(const FormulaType &formulaType, const bool interval,
                                           const bool equidistantTime)
    : formulaType(formulaType), interval(interval), equidistantTime(equidistantTime) {}
