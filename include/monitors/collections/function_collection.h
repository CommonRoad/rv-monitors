#pragma once

#include "function.h"
#include <map>

/**
 * Storage for a set of functions of same type.
 */
template <typename T> class FunctionCollection {
  public:
    /**
     * First constructor.
     */
    FunctionCollection() = default;

    /**
     * Second constructor.
     *
     * @param func Function.
     */
    FunctionCollection(const IFunction<T> &func);

    /**
     * Third constructor.
     *
     * @param funcs Functions.
     */
    FunctionCollection(const std::vector<IFunction<T>> &functions);

    /**
     * Returns number of stored functions.
     *
     * @return Size of collection.
     */
    [[nodiscard]] size_t size() const;

    /**
     * Adds function to collection.
     *
     * @param func Function.
     */
    void addFunction(const IFunction<T> &func);

    /**
     * Removes function from collection.
     *
     * @param name Name of function.
     */
    void removeFunction(const std::string &name);

    /**
     * Returns a requested function by name. If function is undefined, runtime error is thrown.
     *
     * @param name Name of function.
     * @return Function.
     */
    [[nodiscard]] IFunction<T> &getFunction(const std::string &name);

  private:
    std::map<std::string, IFunction<T>> functions; //**< Functions. */
};

template <typename T> FunctionCollection<T>::FunctionCollection(const IFunction<T> &function) {
    functions.insert(std::make_pair(function.getName(), function));
}

template <typename T> FunctionCollection<T>::FunctionCollection(const std::vector<IFunction<T>> &functions) {
    for (const IFunction<T> &function : functions)
        this->functions.insert(std::make_pair(function.getName(), function));
}

template <typename T> size_t FunctionCollection<T>::size() const { return functions.size(); }

template <typename T> void FunctionCollection<T>::addFunction(const IFunction<T> &function) {
    functions.insert(std::make_pair(function.getName(), function));
}

template <typename T> void FunctionCollection<T>::removeFunction(const std::string &name) { functions.erase(name); }

template <typename T> IFunction<T> &FunctionCollection<T>::getFunction(const std::string &name) {
    typename std::map<std::string, IFunction<T>>::iterator iter{functions.find(name)};
    if (iter != functions.end())
        return iter->second;
    else
        throw std::runtime_error("Undefined function " + name + "!");
}
