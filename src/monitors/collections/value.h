#pragma once

#include "string"

class Value {

    /// Value
    std::string  val;

    /// Weight
    float weight;

  public:

    /// First constructor
    Value(std::string val, float weight);

    /// Second constructor
    Value(std::string val);

    /**
     * Returns value.
     *
     * @return Value
     */
    [[nodiscard]] std::string &getVal();

    /**
     * Returns weight.
     *
     * @return Weight
     */
    [[nodiscard]] float getWeight() const;
};
