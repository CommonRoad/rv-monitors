#pragma once

#include <any>
#include <utility>

#include "string"
#include "vector"

/**
 * Class representing domain value.
 */
class DomainValue {
  public:
    /**
     * Constructor.
     *
     * @param variable Name of domain value variable.
     * @param value Value of domain value.
     * @param constVar Boolean indicating whether it is a constant domain value.
     * @param idFunction Function to extract unique ID of domain value.
     * @param validityCondition Boolean functions which checks whether dynamic domain value is valid at step number.
     */
    DomainValue(
        std::string variable, std::any value, bool constVar, std::function<std::string(std::any)> idFunction,
        std::function<bool(size_t, std::any)> validityCondition = [](size_t, const std::any &) { return true; });

    /**
     * Getter for domain value variable name.
     *
     * @return String representing name.
     */
    [[nodiscard]] std::string getVariable() const;

    /**
     * Getter for domain value. User has to cast to concrete type.
     *
     * @return Domain value.
     */
    [[nodiscard]] std::any getValue() const;

    /**
     * Checks validity of domain value at given step number.
     *
     * @param stepNumber Step number to check.
     * @return Boolean indicating validity.
     */
    [[nodiscard]] bool checkValidity(size_t stepNumber) const;

    /**
     * Checks whether domain value is constant.
     *
     * @return Boolean indicating whether domain value is constant.
     */
    [[nodiscard]] bool isConst() const;

    /**
     * Getter for unique ID of domain value.
     *
     * @return String representing unique ID.
     */
    [[nodiscard]] std::string getUniqueID() const;

  private:
    mutable std::string variable;                                    //**< Name of domain value variable. */
    mutable std::any value;                                          //**< Value. */
    mutable std::function<bool(size_t, std::any)> validityCondition; //**< Validity function. */
    mutable bool constVar; //**< Boolean indicating whether it is a constant domain value. */
    mutable std::function<std::string(std::any)> idFunction; //**< ID extraction function. */
};
