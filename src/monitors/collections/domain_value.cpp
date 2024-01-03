#include "monitors/collections/domain_value.h"

#include <utility>

std::any DomainValue::getValue() const { return value; }

std::string DomainValue::getVariable() const { return variable; }

DomainValue::DomainValue(std::string variable, std::any value)
    : variable(std::move(variable)), value(std::move(value)) {}
