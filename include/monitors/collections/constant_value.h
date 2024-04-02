#pragma once

#include <any>
#include <utility>

#include "string"
#include "vector"

/**
 * Class representing a constant value.
 */
class ConstantValue {
  public:
    /**
     * Constructor.
     * @param name Name of constant.
     * @param value Value of constant.
     */
    ConstantValue(std::string name, std::any value);

    /**
     * Getter for constant name.
     *
     * @return String representing name.
     */
    [[nodiscard]] std::string getName() const;

    /**
     * Getter for constant value.
     *
     * @return Value of constant. Can be any type. User has to cast to concrete type.
     */
    [[nodiscard]] std::any getValue() const;

  private:
    std::string name; //**< Name of constant. */
    std::any value;   //**< Value of constant. */
};
