//
// Created by yannick on 15.08.22.
//

#ifndef RVMONITORS_SAME_FUNC_H
#define RVMONITORS_SAME_FUNC_H

#include "monitors/expression_tree/expression.h"
#include "string"

class SameFunc : public Expression {

  private:
    /// Left value
    std::string left;

    /// Right value
    std::string right;

    /// Left constant or not
    bool leftConst;

    /// Right constant or not
    bool rightConst;

  public:
    /// Constructor
    SameFunc(std::string left, std::string right, bool isLeftConst = false, bool isRightConst = false);

    [[nodiscard]] std::string to_string() const override;

    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<HydraMonitor> accept(HydraCreationVisitor &visitor, size_t parentId,
                                                       bool connectedEval) override;

    /**
     * Return the left value.
     *
     * @return Left value
     */
    std::string getLeft() const;

    /**
     * Returns the right value.
     *
     * @return Right value
     */
    std::string getRight() const;

    /**
     * Returns indication whether left value is constant.
     *
     * @return Constant or not
     */
    bool getLeftConst() const;

    /**
     * Returns indication whether right value is constant.
     *
     * @return Constant or not
     */
    bool getRightConst() const;

};

#endif // RVMONITORS_SAME_FUNC_H
