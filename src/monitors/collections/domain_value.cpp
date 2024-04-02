#include "monitors/collections/domain_value.h"

#include <utility>

std::any DomainValue::getValue() const { return value; }

std::string DomainValue::getVariable() const { return variable; }

DomainValue::DomainValue(std::string variable, std::any value, bool constVar,
                         std::function<std::string(std::any)> idFunction,
                         std::function<bool(size_t, std::any)> validityCondition)
    : variable(std::move(variable)), value(std::move(value)), validityCondition(std::move(validityCondition)),
      constVar(constVar), idFunction(std::move(idFunction)) {}

bool DomainValue::checkValidity(size_t stepNumber) const { return validityCondition(stepNumber, value); }

bool DomainValue::isConst() const { return constVar; }

std::string DomainValue::getUniqueID() const { return variable + idFunction(value); }
