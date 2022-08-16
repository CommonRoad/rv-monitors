//
// Created by yannick on 15.08.22.
//

#include "same_func.h"
SameFunc::SameFunc(std::string left, std::string right, bool isLeftConst, bool isRightConst)
    : Expression("=="), left(std::move(left)), right(std::move(right)), leftConst(isLeftConst),
      rightConst(isRightConst) {}

std::string SameFunc::to_string() const {
    std::string leftStr = leftConst ? "\'" + left + "\'" : left;
    std::string rightStr = rightConst ? "\'" + right + "\'" : right;
    return leftStr + " == " + rightStr;
}

std::shared_ptr<Expression> SameFunc::clone() { return std::make_shared<SameFunc>(*this); }

std::shared_ptr<Expression> SameFunc::accept(ExpressionVisitor &visitor, Trace &trace, size_t index) {
    return visitor.visitSameFunc(*this, trace, index);
}

std::shared_ptr<HydraMonitor> SameFunc::accept(HydraCreationVisitor &visitor, size_t parentId, bool connectedEval) {
    return visitor.visitSameFunc(*this, parentId, connectedEval);
}

std::string SameFunc::getLeft() const { return left; }

std::string SameFunc::getRight() const { return right; }

bool SameFunc::getLeftConst() const { return leftConst; }

bool SameFunc::getRightConst() const { return rightConst; }
