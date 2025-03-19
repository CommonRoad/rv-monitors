#pragma once

#include "monitors/collections/domain_value.h"
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>

/**
 * Template class representing a function with arbitrary return value and arguments.
 */
template <typename T> class IFunction {
  public:
    /**
     * Default constructor.
     */
    IFunction() = default;

    /**
     * Constructor.
     *
     * @param name Name of function.
     * @param argumentNames Names of function arguments.
     * @param cost Cost for evaluation of function.
     */
    IFunction(std::string name, std::vector<std::string> argumentNames, float cost = 0.0);

    /**
     * Returns costs of function.
     *
     * @return Costs.
     */
    [[nodiscard]] float getCosts() const;

    /**
     * Returns name of function.
     *
     * @return Name.
     */
    [[nodiscard]] std::string getName() const;

    /**
     * Returns names of function arguments.
     *
     * @return List of names in correct order.
     */
    [[nodiscard]] std::vector<std::string> getArgumentNames() const;

    /**
     * Getter for concrete function.
     *
     * @return Function object.
     */
    virtual std::function<T()> getFunction();

    /**
     *  Getter for function where domain values are assigned dynamically.
     * @param args Domain value arguments.
     * @return Function object.
     */
    [[nodiscard]] virtual std::function<T()> getFunction(const std::vector<DomainValue> &args);

    /**
     * Evaluates function.
     *
     * @return Function return value.
     */
    virtual T evaluateFunction();

  private:
    std::string name;                       //**< Name of function. */
    std::vector<std::string> argumentNames; //**< List of argument names. */
    float costs;                            //**< Costs for evaluation. */
};
template <typename T> std::function<T()> IFunction<T>::getFunction(const std::vector<DomainValue> &args) {
    throw std::runtime_error("IFunction<T>::getFunction: This call should not happen");
    return {};
}
template <typename T> std::function<T()> IFunction<T>::getFunction() {
    throw std::runtime_error("IFunction<T>::getFunction: This call should not happen");
    return {};
}
template <typename T> T IFunction<T>::evaluateFunction() {
    throw std::runtime_error("IFunction<T>::getFunction: This call should not happen");
    return {};
}

template <typename T> IFunction<T>::IFunction(std::string name, std::vector<std::string> argumentNames, float cost)
    : name(std::move(name)), argumentNames(std::move(argumentNames)), costs(cost) {}

template <typename T> std::string IFunction<T>::getName() const { return name; }

template <typename T> std::vector<std::string> IFunction<T>::getArgumentNames() const { return argumentNames; }

template <typename T> float IFunction<T>::getCosts() const { return costs; }

/**
 * Function is used to evaluate a predicate and can also be time dependent.
 */
template <typename T, typename... Args> class Function : public IFunction<T> {
  public:
    /**
     * Constructor.
     *
     * @param name Name of function.
     * @param function Function.
     * @param costs Costs for evaluation.
     */
    Function(std::string name, std::vector<std::string> argumentNames, std::function<T(Args... args)> function,
             std::function<std::tuple<Args...>(std::vector<DomainValue>)> arguments, float costs = 0.0);

    /**
     * Default constructor.
     */
    Function() = default;

    /**
     * Evaluation of function.
     * @return
     */
    operator T() const;

    /**
     * Returns function.
     *
     * @return Function.
     */
    [[nodiscard]] std::function<T()> getFunction(const std::vector<DomainValue> &args);

    /**
     * Evaluation of function with variable argument list.
     *
     * @param args List of arguments.
     * @return Return value of function.
     */
    [[nodiscard]] T evaluateFunction(Args... args);

  private:
    std::function<T(Args... args)> function; //**< Function. */

    std::function<std::tuple<Args...>(std::vector<DomainValue>)> arguments; //**< Function. */
};

template <typename T, typename... Args>
std::function<T()> Function<T, Args...>::getFunction(const std::vector<DomainValue> &args) {
    return [this, args]() { return std::apply(this->function, this->arguments(args)); };
}

template <typename T, typename... Args> Function<T, Args...>::operator T() const { function(); }

template <typename T, typename... Args> T Function<T, Args...>::evaluateFunction(Args... args) {
    return function(args...);
}

template <typename T, typename... Args>
Function<T, Args...>::Function(std::string name, std::vector<std::string> argumentNames,
                               std::function<T(Args...)> function,
                               std::function<std::tuple<Args...>(std::vector<DomainValue>)> arguments, float costs)
    : IFunction<T>(std::move(name), std::move(argumentNames), costs), function(function), arguments(arguments) {}
