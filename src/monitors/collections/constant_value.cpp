#include "monitors/collections/constant_value.h"

#include <utility>

std::any ConstantValue::getValue() const { return value; }

std::string ConstantValue::getName() const { return name; }

ConstantValue::ConstantValue(std::string name, std::any value) : name(std::move(name)), value(std::move(value)) {}
