//
// Created by yannick on 15.08.22.
//

#include "equality_operator.h"
EqualityOperator::EqualityOperator(std::string left, std::string right, bool isLeftConst, bool isRightConst)
    : Expression("=="), left(std::move(left)), right(std::move(right)), leftConst(isLeftConst),
      rightConst(isRightConst) {}

std::string EqualityOperator::to_string() const {
    std::string leftStr = leftConst ? "\'" + left + "\'" : left;
    std::string rightStr = rightConst ? "\'" + right + "\'" : right;
    return leftStr + " == " + rightStr;
}

std::shared_ptr<Expression> EqualityOperator::clone() { return std::make_shared<EqualityOperator>(*this); }

std::shared_ptr<Expression> EqualityOperator::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitEqualityOperator(*this, trace, index);
}

std::shared_ptr<HydraMonitor> EqualityOperator::accept(HydraCreationVisitor &visitor, size_t parentId, bool connectedEval) {
    return visitor.visitEqualityOperator(*this, parentId, connectedEval);
}

std::string EqualityOperator::getLeft() const { return left; }

std::string EqualityOperator::getRight() const { return right; }

bool EqualityOperator::getLeftConst() const { return leftConst; }

bool EqualityOperator::getRightConst() const { return rightConst; }
