#include "ExpressionVisitor.h"
#include "ExpressionTree/Unary/Unary.h"
#include "ExpressionTree/Binary/Binary.h"
#include "ExpressionTree/Nary/NaryInterface.h"
#include "ExpressionTree/Nary/Bool/And.h"
#include "ExpressionTree/Nary/Bool/Xor.h"


void ExpressionVisitor::visitExpression(Expression &expr, Trace &trace, int index) {
    // Unary
    if (auto unary = dynamic_cast<Unary *>(&expr)) {
        unary->setLeft(unary->getLeft()->accept(*this, trace, index));
    }
        // Binary
    else if (auto binary = dynamic_cast<Binary *>(&expr)) {
        binary->setLeft(binary->getLeft()->accept(*this, trace, index));
        binary->setRight(binary->getRight()->accept(*this, trace, index));
    }
        // Nary
    else if (auto nary = dynamic_cast<NaryInterface *>(&expr)) {
        // Xor needs multiset
        if (dynamic_cast<Xor *>(nary)) {
            Xor::SetType_T newSet;
            for (const auto &itr : *nary) {
                newSet.emplace(itr->accept(*this, trace, index));
            }
            // Avoid Narys with only one child
            if (nary->size() == 1) {
                expr = **(newSet.begin());
            }
            else {
                nary->setExpressions(newSet.begin(), newSet.end());
            }

        }
        else {
            And::SetType_T newSet;
            for (const auto &itr : *nary) {
                newSet.emplace(itr->accept(*this, trace, index));
            }

            // Avoid Narys with only one child
            if (nary->size() == 1) {
                expr = **(newSet.begin());
            }
            else {
                nary->setExpressions(newSet.begin(), newSet.end());
            }
        }
    }
}

