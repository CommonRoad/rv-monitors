#include "monitors/collections/domain_value.h"

#include <any>
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

bool DomainValue::isTimeStep() const { return variable == timeStepVariable; }

DomainValue DomainValue::makeTimeStep(size_t timeStep) {
    return {timeStepVariable, std::make_any<size_t>(timeStep), true,
            [](std::any val) { return std::to_string(std::any_cast<int>(val)); }};
}

std::optional<size_t> DomainValue::getTimeStep() const {
    if (isTimeStep()) {
        return std::any_cast<size_t>(value);
    } else {
        return std::nullopt;
    }
}