#ifndef RVMONITORS_FIRST_ORDER_OPERATOR_H
#define RVMONITORS_FIRST_ORDER_OPERATOR_H

#include <string>
#include <set>
#include <memory>
#include "expression.h"

class Variable {
  public:
    /**
     * Constructor.
     *
     * @param name Name of variable.
     * @param domain Domain of variable.
     */
    explicit Variable(std::string &name, std::string &domain);

    /**
     * Returns name of variable.
     *
     * @return Name.
     */
    [[nodiscard]] std::string getName() const;

    /**
     * Returns name of collections.
     *
     * @return Name of collections.
     */
    [[nodiscard]] std::string getDomain() const;

  private:
    std::string name; //**< Name of variable. */
    std::string domain; //**< Domain of variable. */

};

class FirstOrderOperator {
  public:
    /**
     * First constructor.
     *
     * @param variables Variables used by first order operator.
     */
    explicit FirstOrderOperator(std::vector<Variable> &variables);

    /**
     * Second constructor.
     *
     * @param variable Variable used by first order operator.
     */
    explicit FirstOrderOperator(Variable &variable);

    /**
     * Destructor.
     */
    virtual ~FirstOrderOperator() = default;

    /**
     * Adds variable.
     *
     * @param variable Variable.
     */
    void addVariable(std::string variable, std::string domain);

    /**
     * Removes variable.
     *
     * @param variable Variable.
     */
     void removeVariable(std::string variable);

    /**
     * Returns all variables bounded by operator.
     *
     * @return Bounded variables.
     */
    [[nodiscard]] const std::vector<Variable> &getVariables() const;

    /**
     * Creates string representation of first order expression.
     *
     * @return String representing of expression.
     */
    [[nodiscard]] std::string to_string() const;

  private:
    std::vector<Variable> variables; //**< Variables used by first oder operator. */

};

#endif // RVMONITORS_FIRST_ORDER_OPERATOR_H
