#include "expression_visitor.h"
#include "expression_tree/binary/binary.h"
#include "expression_tree/nary/bool/and.h"
#include "expression_tree/nary/bool/xor.h"
#include "expression_tree/nary/nary_interface.h"
#include "expression_tree/unary/unary.h"

void expression_visitor::visitExpression(Expression &expr, Trace &trace, int index) {
    // Unary
    if (auto *unary = dynamic_cast<Unary *>(&expr)) {
        unary->setLeft(unary->getLeft()->accept(*this, trace, index));
    }
    // Binary
    else if (auto *binary = dynamic_cast<Binary *>(&expr)) {
        binary->setLeft(binary->getLeft()->accept(*this, trace, index));
        binary->setRight(binary->getRight()->accept(*this, trace, index));
    }
    // Nary
    else if (auto *nary = dynamic_cast<NaryInterface *>(&expr)) {
        // Xor needs multiset
        if (dynamic_cast<Xor *>(nary)) {
            Xor::SetType_T newSet;
            for (const auto &itr : *nary) {
                newSet.emplace(itr->accept(*this, trace, index));
            }
            // Avoid Narys with only one child
            if (nary->size() == 1) {
                expr = **(newSet.begin());
            } else {
                nary->setExpressions(newSet.begin(), newSet.end());
            }

        } else {
            And::SetType_T newSet;
            for (const auto &itr : *nary) {
                newSet.emplace(itr->accept(*this, trace, index));
            }

            // Avoid Narys with only one child
            if (nary->size() == 1) {
                expr = **(newSet.begin());
            } else {
                nary->setExpressions(newSet.begin(), newSet.end());
            }
        }
    }
}
