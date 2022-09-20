#pragma once

#include "monitors/expression_tree/expression.h"
#include "string"

/**
 * Class representing the equality operator.
 */
class EqualityOperator : public Expression {
  public:
    /**
     * Constructor.
     *
     * @param left Left constant or variable.
     * @param right Right constant or variable.
     * @param isLeftConst Boolean indicating whether left is constant.
     * @param isRightConst Boolean indicating whether right is constant.
     */
    EqualityOperator(std::string left, std::string right, bool isLeftConst = false, bool isRightConst = false);

    /**
     * Creates string representation of expression.
     *
     * @return String representing expression.
     */
    [[nodiscard]] std::string to_string() const override;

    /**
     * Clones the same function expression.
     *
     * @return Cloned expression.
     */
    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Hydra2Monitor> accept(HydraCreationVisitor2 &visitor, size_t parentId,
                                                       bool connectedEval) override;

    [[nodiscard]] std::shared_ptr<HydraMonitor> accept(HydraCreationVisitor &visitor) override;

    /**
     * Return the left value.
     *
     * @return Left value.
     */
    std::string getLeft() const;

    /**
     * Returns the right value.
     *
     * @return Right value.
     */
    std::string getRight() const;

    /**
     * Returns indication whether left is constant.
     *
     * @return Boolean indicating left is constant.
     */
    bool getLeftConst() const;

    /**
     * Returns indication whether right value is constant.
     *
     * @return Boolean indicating right is constant.
     */
    bool getRightConst() const;

  private:
    std::string left; //**< Left constant or variable. */
    std::string right; //**< Right constant or variable. */
    bool leftConst; //**< Boolean indicating left is constant. */
    bool rightConst; //**< Boolean indicating right is constant. */

};
