#pragma once

#include "domain.h"
#include "expression.h"
#include <memory>
#include <set>
#include <string>

/**
 * Class representing a variable used by a first-order operator.
 */
class Variable {
  public:
    /**
     * Constructor.
     *
     * @param name Name of variable.
     * @param domain Domain of variable.
     */
    explicit Variable(std::string name, Domain domain);

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
    [[nodiscard]] Domain getDomain() const;

  private:
    std::string name; //**< Name of variable. */
    Domain domain;    //**< Domain of variable. */
};

/**
 * Class representing a first-order operator.
 */
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
    void addVariable(const std::string &variable, const Domain &domain);

    /**
     * Removes variable.
     *
     * @param variable Variable.
     */
    void removeVariable(std::string variable);

    /**
     * Returns all variable objects bounded by operator.
     *
     * @return Bounded variables.
     */
    [[nodiscard]] std::vector<Variable> getVariables() const;

    /**
     * Returns all variable names bounded by operator.
     *
     * @return Bounded variable names.
     */
    [[nodiscard]] std::set<std::string> getVariableNames() const;

    /**
     * Creates string representation of first order expression.
     *
     * @return String representing of expression.
     */
    [[nodiscard]] std::string to_string() const;

  protected:
    std::vector<Variable> variables; //**< Variables used by first oder operator. */
};
