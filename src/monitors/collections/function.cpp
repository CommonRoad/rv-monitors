#include "function.h"

#include "functional"
#include <utility>

Function::Function(std::string name, size_t argc, std::function<bool(int, char **)> function, bool withTimeStepArgument,
                   float costs)
    : name(std::move(name)), argc(argc), function(std::move(function)), withTimeStepArgument(withTimeStepArgument),
      costs(costs) {}

std::string Function::getName() const { return name; }

size_t Function::getArgc() const { return argc; }

std::function<bool(int, char *[])> Function::getFunction() { return function; }

bool Function::getWithTimeStepArgument() const { return withTimeStepArgument; }

float Function::getCosts() const { return costs; }
