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
    ConstantValue(std::string name, std::any value, std::function<std::string(std::any)> idFunction);

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

    /**
     * Function which returns ID of of constant object.
     *
     * @return String representing ID.
     */
    const std::function<std::string(std::any)> &getIdFunction() const;

  private:
    mutable std::string name;                                //**< Name of constant. */
    mutable std::any value;                                  //**< Value of constant. */
    mutable std::function<std::string(std::any)> idFunction; //**< ID extraction function. */
};
