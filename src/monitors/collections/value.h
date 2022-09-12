#pragma once

#include "string"

/**
 * Value wraps a weighted string and can be stored in Domain.
 */
class Value {
  public:
    /**
     * First constructor.
     *
     * @param val Value.
     * @param weight Weight.
     */
    Value(std::string val, float weight);

    /**
     * Second constructor.
     *
     * @param val Value.
     */
    Value(std::string val);

    /**
     * Returns value.
     *
     * @return Value.
     */
    [[nodiscard]] std::string &getVal();

    /**
     * Returns weight.
     *
     * @return Weight.
     */
    [[nodiscard]] float getWeight() const;

  private:
    std::string  val; //**< Value. */
    float weight; //**< Weight. */
};
