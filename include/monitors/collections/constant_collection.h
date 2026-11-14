#pragma once

#include "constant_value.h"
#include <map>

/**
 * Storage for a set of constants.
 */
class ConstantCollection {
  public:
    /**
     * First constructor.
     */
    ConstantCollection() = default;

    /**
     * Third constructor.
     *
     * @param constants List of constants.
     */
    explicit ConstantCollection(const std::vector<ConstantValue> &constants);

    /**
     * Returns number of stored constants.
     *
     * @return Size of collection.
     */
    [[nodiscard]] size_t size() const;

    /**
     * Adds constant to collection.
     *
     * @param constant Constant value.
     */
    void addConstant(const ConstantValue &constant);

    /**
     * Returns a requested domain by name. If domain is undefined, runtime error is thrown.
     *
     * @param name Name of constant.
     * @return Constant value.
     */
    [[nodiscard]] ConstantValue &getConstant(const std::string &name);

    /**
     * Returns a requested domain by name. If domain is undefined, runtime error is thrown.
     *
     * @param name Name of constant.
     * @return Constant value.
     */
    [[nodiscard]] bool contains(const std::string &name);

  private:
    std::map<std::string, ConstantValue> constants; //**< Constants. */
};
