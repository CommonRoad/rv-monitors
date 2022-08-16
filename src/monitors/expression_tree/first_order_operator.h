#ifndef RVMONITORS_FIRST_ORDER_OPERATOR_H
#define RVMONITORS_FIRST_ORDER_OPERATOR_H

#include <string>
#include <set>
#include <memory>
#include "expression.h"

class Variable {

  private:
    /// Name of variable
    std::string name;

    /// Domain of variable
    std::string domain;

  public:
    /// Constructor
    explicit Variable(std::string &name, std::string &domain);

    /**
     * Returns name of variable.
     *
     * @return Name
     */
    [[nodiscard]] std::string getName() const;

    /**
     * Returns name of collections.
     *
     * @return Name of collections
     */
    [[nodiscard]] std::string getDomain() const;

};

class FirstOrderOperator {

  private:

    /// Bounded variables
    std::vector<Variable> variables;

  public:
    /// First constructor
    explicit FirstOrderOperator(std::vector<Variable> &variables);

    /// Second constructor
    explicit FirstOrderOperator(Variable &variable);

    /// Destructor
    virtual ~FirstOrderOperator() = default;

    /**
     * Adds variable.
     *
     * @param variable Variable
     */
    void addVariable(std::string variable, std::string domain);

    /**
     * Removes variable.
     *
     * @param variable Variable
     */
     void removeVariable(std::string variable);

    /**
     * Returns all variables bounded by operator.
     *
     * @return Bounded variables
     */
    [[nodiscard]] const std::vector<Variable> &getVariables() const;

    [[nodiscard]] std::string to_string() const;

};

#endif // RVMONITORS_FIRST_ORDER_OPERATOR_H
