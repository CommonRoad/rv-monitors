#pragma once

#include "function.h"
#include <map>

class FunctionCollection {
  public:

    /**
     * First constructor.
     */
    FunctionCollection() = default;

    /**
     * Second constructor.
     *
     * @param function Function.
     */
    FunctionCollection(const Function &function);

    /**
     * Third constructor.
     *
     * @param functions Functions.
     */
    FunctionCollection(const std::vector<Function> &functions);

    /**
     * Returns number of functions in collection.
     *
     * @return Number of stored functions.
     */
    [[nodiscard]] size_t size() const;

    /**
     * Adds function to collection.
     *
     * @param function Function.
     */
    void addFunction(const Function &function);

    /**
     * Removes function to collection.
     *
     * @param name Name of function.
     */
    void removeFunction(const std::string &name);

    /**
     * Returns a requested function by name. If function is not part of collection, a non-valued optional is returned.
     *
     * @param name Name of function.
     * @return Function.
     */
    Function &getFunction(const std::string &name);

  private:
    std::map<std::string, Function> functions; //**< Functions. */

};
