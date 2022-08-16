#pragma once

#include "string"
#include "value.h"
#include "vector"

class Domain {

    /// Name
    std::string name;

    /// Values
    std::vector<Value> values;

  public:

    /// First constructor
    Domain(std::string name, std::vector<Value> values);

    /// Second constructor
    Domain(std::string name, const std::vector<std::string> &values);

    /// Add operator
    [[nodiscard]] Domain operator+(const Domain &other);

    /**
     * Returns size of collections.
     *
     * @return Size
     */
    [[nodiscard]] size_t size() const;

    /**
     * Returns iterator pointing to beginning.
     *
     * @return Iterator
     */
    [[nodiscard]] std::vector<Value>::iterator begin();

    /**
     * Returns iterator pointing to ending.
     *
     * @return Iterator
     */
    [[nodiscard]] std::vector<Value>::iterator end();

    /**
     * Adds value to collections.
     *
     * @param val Value
     */
    void addValue(const std::string &val);

    /**
     * Adds value to collections.
     *
     * @param val Value
     */
    void addValue(const Value &val);

    /**
     * Adds values to collections.
     *
     * @param vals Values
     */
    void addValues(const std::vector<std::string> &vals);

    /**
     * Adds values to collections.
     *
     * @param values Values
     */
    void addValues(const std::vector<Value> &values);

    /**
     * Calculates average of weight in collections.
     *
     * @return Averaged weight
     */
    float calcWeightAverage() const;

    /**
     * Returns name of collections.
     *
     * @return Name
     */
    [[nodiscard]] std::string getName() const;

};
