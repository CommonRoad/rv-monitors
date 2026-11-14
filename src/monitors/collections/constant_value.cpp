#include "monitors/collections/constant_value.h"

#include <utility>

std::any ConstantValue::getValue() const { return value; }

std::string ConstantValue::getName() const { return name; }

ConstantValue::ConstantValue(std::string name, std::any value, std::function<std::string(std::any)> idFunction)
    : name(std::move(name)), value(std::move(value)), idFunction(std::move(idFunction)) {}

const std::function<std::string(std::any)> &ConstantValue::getIdFunction() const { return idFunction; }
