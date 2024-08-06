#pragma once

#include "monitors/expression_tree/expression.h"
#include <utility>
#include <vector>

class Predicate : public Expression {
  public:
    /**
     * Constructor.
     * @param symbol Predicate name and arguments connected by "_".
     * @param name Name of predicate.
     * @param parameters All arguments of predicate.
     * @param parameterVariables Variable (domain) arguments of predicate.
     */
    Predicate(std::string symbol, std::string name, std::vector<std::string> parameters,
              std::vector<std::string> parameterVariables);

    /**
     * to_string() for a predicate.
     *
     * @return String representation of a predicate.
     */
    [[nodiscard]] std::string to_string() const override;

    /**
     * Clones predicate.
     *
     * @return New shared_ptr of this predicate.
     */
    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    /**
     * Accept function for visitor pattern for predicate.
     *
     * @param visitor Visitor.
     * @param trace Event.
     * @param index Current (max) index.
     *
     * @return Expression of visit predicate.
     */
    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, const std::shared_ptr<Trace> &trace,
                                                     size_t index) override;

    /**
     * Accept function for visitor pattern for Hydra2 monitor.
     *
     * @param visitor Visitor.
     * @param parentId ID of parent monitor.
     * @param connectedEval Boolean indicating whether connected evaluation should be performed.
     * @return Hydra2 predicate monitor.
     */
    [[nodiscard]] std::shared_ptr<Hydra2Monitor> accept(HydraCreationVisitor2 &visitor, size_t parentId,
                                                        bool connectedEval) override;

    /**
     * Accept function for visitor pattern for original Hydra monitor.
     *
     * @param visitor Visitor.
     * @return Hydra predicate monitor.
     */
    [[nodiscard]] std::shared_ptr<HydraMonitor> accept(HydraCreationVisitor &visitor) override;

    /**
     * Getter for predicate name.
     *
     * @return Predicate name.
     */
    [[nodiscard]] const std::string &getName() const;

    /**
     * Getter for variable domain value parameter names.
     *
     * @return List of names.
     */
    [[nodiscard]] const std::vector<std::string> &getVarParameters() const;

    /**
     * Getter for constant parameter names.
     *
     * @return List of names.
     */
    [[nodiscard]] const std::vector<std::string> &getConstParameters() const;

    /**
     * Getter for parameter names.
     *
     * @return List of parameter names.
     */
    [[nodiscard]] const std::vector<std::string> &getParameters() const;

  protected:
    std::string name;                            /**< Name of predicate */
    std::vector<std::string> parameters;         /**< All arguments of predicate */
    std::vector<std::string> parameterVariables; /**< Variable arguments of predicate */
    std::vector<std::string> parameterConstants; /**< Constant arguments of predicate */
};
