#include "value.h"

#include <utility>

Value::Value(std::string val, float weight) : val(std::move(val)), weight(weight) {}

Value::Value(std::string val) : val(std::move(val)), weight(100.) {}

std::string &Value::getVal() { return val; }

float Value::getWeight() const { return weight; }
