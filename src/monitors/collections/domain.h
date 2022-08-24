#pragma once

#include "string"
#include "value.h"
#include "vector"

class Domain {
  public:

    /**
     * First constructor.
     *
     * @param name Name of domain.
     * @param values Values of domain.
     */
    Domain(std::string name, std::vector<Value> values);

    /**
     * Second constructor.
     *
     * @param name Name of domain.
     * @param values Values of domain.
     */
    Domain(std::string name, const std::vector<std::string> &values);

    /**
     * Unifies two domains.
     *
     * @param other Other domain.
     * @return Unified domain.
     */
    [[nodiscard]] Domain operator+(const Domain &other);

    /**
     * Returns size of domain.
     *
     * @return Size.
     */
    [[nodiscard]] size_t size() const;

    /**
     * Returns iterator pointing to beginning.
     *
     * @return Iterator.
     */
    [[nodiscard]] std::vector<Value>::iterator begin();

    /**
     * Returns iterator pointing to ending.
     *
     * @return Iterator.
     */
    [[nodiscard]] std::vector<Value>::iterator end();

    /**
     * Adds value to domain.
     *
     * @param val Value.
     */
    void addValue(const std::string &val);

    /**
     * Adds value to domain.
     *
     * @param val Value.
     */
    void addValue(const Value &val);

    /**
     * Adds values to domain.
     *
     * @param vals Values.
     */
    void addValues(const std::vector<std::string> &vals);

    /**
     * Adds values to domain.
     *
     * @param values Values.
     */
    void addValues(const std::vector<Value> &values);

    /**
     * Calculates average of values weights.
     *
     * @return Averaged weight.
     */
    float calcWeightAverage() const;

    /**
     * Returns name of domain.
     *
     * @return Name.
     */
    [[nodiscard]] std::string getName() const;

  private:
    std::string name; //**< Name of domain. */
    std::vector<Value> values; //**< Values of domain. */

};
