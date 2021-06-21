#include "nff.h"

#include "monitors/expression_tree/atomic/atomic.h"
#include "monitors/expression_tree/atomic/constant.h"
#include "monitors/expression_tree/binary/bool/implication.h"
#include "monitors/expression_tree/binary/temporal/ltlf/release.h"
#include "monitors/expression_tree/binary/temporal/ltlf/until.h"
#include "monitors/expression_tree/nary/bool/and.h"
#include "monitors/expression_tree/nary/bool/or.h"
#include "monitors/expression_tree/nary/bool/xor.h"
#include "monitors/expression_tree/unary/bool/not.h"
#include "monitors/expression_tree/unary/temporal/ltlf/future.h"
#include "monitors/expression_tree/unary/temporal/ltlf/globally.h"
#include "monitors/expression_tree/unary/temporal/ltlf/next_strong.h"
#include "monitors/expression_tree/unary/temporal/ltlf/next_weak.h"
#include "monitors/expression_tree/unary/temporal/ptlf/once.h"
#include "monitors/expression_tree/unary/temporal/ptlf/previously.h"

std::shared_ptr<Expression> NNF::removeDoubleNot(const std::shared_ptr<Not> &expr) {
    // Remove two Nots
    if (auto notExpr = std::dynamic_pointer_cast<Not>(expr->getLeft())) {
        return notExpr->getLeft();
    }
    return expr;
}

std::shared_ptr<Expression> NNF::applyDeMorgan(const std::shared_ptr<Not> &expr) {
    // And
    if (auto andExpr = std::dynamic_pointer_cast<And>(expr->getLeft())) {
        Or::SetType_T newOrExpressions;
        for (const auto &itr : *andExpr) {
            newOrExpressions.emplace(std::make_shared<Not>(itr));
        }
        return std::make_shared<Or>(newOrExpressions);
    }
    // Or
    if (auto orExpr = std::dynamic_pointer_cast<Or>(expr->getLeft())) {
        And::SetType_T newAndExpressions;
        for (const auto &itr : *orExpr) {
            newAndExpressions.emplace(std::make_shared<Not>(itr));
        }
        return std::make_shared<And>(newAndExpressions);
    }

    //    // Xor
    //    if (auto xorExpr = std::dynamic_pointer_cast<Xor>(expr->getLeft())) {
    //        std::set<std::shared_ptr<Expression>> newExpressions;
    //        for (const auto &itr : *xorExpr){
    //            newExpressions.emplace(std::make_shared<Not>(itr));
    //            newExpressions.emplace(itr);
    //        }
    //        return std::make_shared<And>(newExpressions);
    //    }
    return expr;
}

std::shared_ptr<Expression> NNF::removeImplication(const std::shared_ptr<Implication> &expr) {
    auto newLeft = std::make_shared<Not>(expr->getLeft());
    auto newRight = expr->getRight();
    Or::SetType_T orSet{newLeft, newRight};
    return std::make_shared<Or>(orSet);
}

std::shared_ptr<Expression> NNF::moveNotTemporal(const std::shared_ptr<Not> &expr) {
    // Future
    if (auto *node = dynamic_cast<Future *>(expr->getLeft().get())) {
        return std::make_shared<Globally>(std::make_shared<Not>(node->getLeft()));
    }
    // Once
    if (auto *node = dynamic_cast<Once *>(expr->getLeft().get())) {
        return std::make_shared<Globally>(std::make_shared<Not>(node->getLeft()));
    }
    // Globally
    if (auto *node = dynamic_cast<Globally *>(expr->getLeft().get())) {
        return std::make_shared<Future>(std::make_shared<Not>(node->getLeft()));
    }
    // Weak Next
    if (auto *node = dynamic_cast<NextWeak *>(expr->getLeft().get())) {
        return std::make_shared<NextWeak>(std::make_shared<Not>(node->getLeft()));
    }
    // Strong Next
    if (auto *node = dynamic_cast<NextStrong *>(expr->getLeft().get())) {
        return std::make_shared<NextStrong>(std::make_shared<Not>(node->getLeft()));
    }
    // Previously
    if (auto *node = dynamic_cast<Previously *>(expr->getLeft().get())) {
        return std::make_shared<Previously>(std::make_shared<Not>(node->getLeft()));
    }
    // Until
    if (auto *node = dynamic_cast<Until *>(expr->getLeft().get())) {
        return std::make_shared<Release>(std::make_shared<Not>(node->getLeft()),
                                         std::make_shared<Not>(node->getRight()));
    }
    // Release
    if (auto *node = dynamic_cast<Release *>(expr->getLeft().get())) {
        return std::make_shared<Until>(std::make_shared<Not>(node->getLeft()), std::make_shared<Not>(node->getRight()));
    }

    return expr;
}

std::shared_ptr<Expression> NNF::removeFutureAndGlobally(const std::shared_ptr<Unary> &expr) {
    // Future
    if (auto *node = dynamic_cast<Future *>(expr.get())) {
        return std::make_shared<Until>(std::make_shared<Constant>(true), node->getLeft());
    }
    // Globally
    if (auto *node = dynamic_cast<Globally *>(expr.get())) {
        return std::make_shared<Release>(std::make_shared<Constant>(false), node->getLeft());
    }
    return expr;
}

std::shared_ptr<Expression> NNF::evaluate(const std::shared_ptr<Expression> &expr) {
    if (expr == nullptr) {
        return nullptr;
    }

    // Implication
    if (auto node = std::dynamic_pointer_cast<Implication>(expr)) {
        auto ret = removeImplication(node);

        // Result is Or (n-ary), evaluate all expressions in it
        auto orExpr = std::dynamic_pointer_cast<Or>(ret);
        std::set<std::shared_ptr<Expression>> newSet;
        for (const auto &itr : *orExpr) {
            newSet.emplace(evaluate(itr));
        }
        orExpr->setExpressions(newSet.begin(), newSet.end());
        return orExpr;
    }

    // Binary
    if (auto binary = std::dynamic_pointer_cast<Binary>(expr)) {
        binary->setLeft(evaluate(binary->getLeft()));
        binary->setRight(evaluate(binary->getRight()));
        return binary;
    }

    // N-ary
    if (auto nary = std::dynamic_pointer_cast<NaryInterface>(expr)) {
        if (std::dynamic_pointer_cast<Xor>(nary)) {
            std::multiset<std::shared_ptr<Expression>> newSet;
            for (const auto &itr : *nary) {
                newSet.emplace(evaluate(itr));
            }
            nary->setExpressions(newSet.begin(), newSet.end());
            return nary;
        } else {
            std::set<std::shared_ptr<Expression>> newSet;
            for (const auto &itr : *nary) {
                newSet.emplace(evaluate(itr));
            }
            nary->setExpressions(newSet.begin(), newSet.end());
            return nary;
        }
    }

    {
        // Future and Globally
        std::shared_ptr<Unary> unary = std::dynamic_pointer_cast<Globally>(expr);
        if (!unary) {
            unary = std::dynamic_pointer_cast<Future>(expr);
        }
        if (unary) {
            // Return Release or Until
            auto ret = removeFutureAndGlobally(unary);
            auto binary = std::dynamic_pointer_cast<Binary>(ret);
            binary->setLeft(evaluate(binary->getLeft()));
            binary->setRight(evaluate(binary->getRight()));
            return binary;
        }
    }

    // Not
    if (auto notExpr = std::dynamic_pointer_cast<Not>(expr)) {
        auto ret = moveNegationInwards(notExpr);

        // Unary
        if (auto unary = std::dynamic_pointer_cast<Unary>(ret)) {
            unary->setLeft(evaluate(unary->getLeft()));
            return unary;
        }
        // Binary
        if (auto binary = std::dynamic_pointer_cast<Binary>(ret)) {
            binary->setLeft(evaluate(binary->getLeft()));
            binary->setRight(evaluate(binary->getRight()));
            return binary;
        }
        // Nary
        if (auto nary = std::dynamic_pointer_cast<NaryInterface>(ret)) {
            return nary;
        }
        return ret;
    }
    return expr;
}

std::shared_ptr<Expression> NNF::moveNegationInwards(const std::shared_ptr<Not> &notExpr) {
    std::shared_ptr<Expression> expr = moveNotTemporal(notExpr);
    if (auto node = std::dynamic_pointer_cast<Not>(expr)) {
        expr = removeDoubleNot(node);
    }
    if (auto node = std::dynamic_pointer_cast<Not>(expr)) {
        expr = applyDeMorgan(node);
    }

    return expr;
}
