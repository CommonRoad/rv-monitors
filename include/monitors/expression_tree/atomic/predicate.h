#pragma once

#include "monitors/expression_tree/expression.h"
#include <utility>
#include <vector>

class Predicate : public Expression {
  public:
    Predicate(std::string symbol, std::string name, std::vector<std::string> parameters,
              std::vector<std::string> parameterVariables);

    [[nodiscard]] std::string to_string() const override;

    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Hydra2Monitor> accept(HydraCreationVisitor2 &visitor, size_t parentId,
                                                        bool connectedEval) override;

    [[nodiscard]] std::shared_ptr<HydraMonitor> accept(HydraCreationVisitor &visitor) override;

    const std::string &getName() const;

    const std::vector<std::string> &getVarParameters() const;

    const std::vector<std::string> &getConstParameters() const;

    const std::vector<std::string> &getParameters() const;

  protected:
    std::string name;
    std::vector<std::string> parameters;
    std::vector<std::string> parameterVariables;
    std::vector<std::string> parameterConstants;
};
