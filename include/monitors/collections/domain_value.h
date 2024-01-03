#pragma once

#include <any>
#include <utility>

#include "string"
#include "vector"

class DomainValue {
  public:
    DomainValue(std::string variable, std::any value);

    DomainValue() = default;

    std::string getVariable() const;

    std::any getValue() const;

  protected:
    std::string variable;
    std::any value;
};
