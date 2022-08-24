#pragma once

#include <functional>

class Function {
  public:
    /**
     * Constructor.
     *
     * @param name Name of function.
     * @param argc Number of arguments.
     * @param function Function.
     * @param withTimeStepArgument Boolean indicating whether the first argument is occupied by time step.
     * @param costs Costs for evaluation.
     */
    Function(std::string name, size_t argc, std::function<bool(int, char *[])> function, bool withTimeStepArgument = false, float costs = 0.);

    /**
     * Returns name of function.
     *
     * @return Name.
     */
    [[nodiscard]] std::string getName() const;

    /**
     * Returns number of arguments.
     *
     * @return Number.
     */
    [[nodiscard]] size_t getArgc() const;

    /**
     * Returns function.
     *
     * @return Function.
     */
    [[nodiscard]] std::function<bool(int, char *[])> getFunction();

    /**
     * Returns support of time step argument.
     *
     * @return Supported or not.
     */
    [[nodiscard]] bool getWithTimeStepArgument() const;

    /**
     * Returns costs of function.
     *
     * @return Costs.
     */
    [[nodiscard]] float getCosts() const;

  private:
    std::string name; //**< Name of function. */
    size_t argc; //**< Number of arguments. */
    std::function<bool(int, char *[])> function; //**< Function. */
    bool withTimeStepArgument; //**< Boolean indicating whether the first argument is occupied by time step. */
    float costs; //**< Costs for evaluation. */

};
