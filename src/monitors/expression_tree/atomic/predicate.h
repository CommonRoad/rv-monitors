//
// Created by Sebastian Maierhofer on 08.06.21.
//

#ifndef RV_MONITORS_SRC_MONITORS_EXPRESSION_TREE_ATOMIC_PREDICATE_H_
#define RV_MONITORS_SRC_MONITORS_EXPRESSION_TREE_ATOMIC_PREDICATE_H_

#include "monitors/expression_tree/expression.h"
#include <vector>

class Predicate : public Expression {
  public:
    Predicate(std::string symbol, std::string na, std::vector<std::string> params);

    [[nodiscard]] std::string to_string() const override;

    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, int index) override;

    const std::string &getName() const;

    const std::vector<std::string> &getParameters() const;

  private:
    std::string name;
    std::vector<std::string> parameters;
    // std::function<bool(size_t)> implementation;
};

#endif // RV_MONITORS_SRC_MONITORS_EXPRESSION_TREE_ATOMIC_PREDICATE_H_
