#pragma once

#include "function.h"
#include <map>

class FunctionCollection {

    std::map<std::string, Function> functions;

  public:

    /// First constructor
    FunctionCollection() = default;

    /// Second constructor
    FunctionCollection(const Function &function);

    /// Third constructor
    FunctionCollection(const std::vector<Function> &functions);

    /**
     * Returns number of functions in collection.
     *
     * @return Number
     */
    [[nodiscard]] size_t size() const;

    /**
     * Adds function to collection.
     *
     * @param function Function
     */
    void addFunction(const Function &function);

    /**
     * Removes function to collection.
     *
     * @param name
     */
    void removeFunction(const std::string &name);

    /**
     * Returns a requested function by name. If function is not part of collection, a non-valued optional is returned.
     *
     * @param name
     * @return
     */
    Function &getFunction(const std::string &name);

};
