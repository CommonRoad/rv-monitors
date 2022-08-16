#pragma once

#include <functional>

class Function {

    /// Name
    std::string name;

    /// Number of args
    size_t argc;

    /// Function
    std::function<bool(int, char *[])> function;

    /// Support of time step argument
    bool withTimeStepArgument;

    /// Costs
    float costs;

  public:

    /// Constructor
    Function(std::string name, size_t argc, std::function<bool(int, char *[])> function, bool withTimeStepArgument = false, float costs = 0.);

    /**
     * Returns name of function.
     *
     * @return Name
     */
    [[nodiscard]] std::string getName() const;

    /**
     * Returns number of arguments.
     *
     * @return Number
     */
    [[nodiscard]] size_t getArgc() const;

    /**
     * Returns function.
     *
     * @return Function
     */
    [[nodiscard]] std::function<bool(int, char *[])> getFunction();

    /**
     * Returns support of time step argument.
     *
     * @return Supported or not
     */
    [[nodiscard]] bool getWithTimeStepArgument() const;

    /**
     * Returns costs of function.
     *
     * @return Costs
     */
    [[nodiscard]] float getCosts() const;

};
