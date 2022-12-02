#pragma once

#include "monitors/expression_tree/atomic/atomic.h"
#include "monitors/expression_tree/atomic/constant.h"
#include "monitors/expression_tree/atomic/predicate.h"
#include "monitors/expression_tree/binary/bool/equivalence.h"
#include "monitors/expression_tree/binary/bool/implication.h"
#include "monitors/expression_tree/binary/temporal/release.h"
#include "monitors/expression_tree/binary/temporal/since.h"
#include "monitors/expression_tree/binary/temporal/until.h"
#include "monitors/expression_tree/interval.h"
#include "monitors/expression_tree/nary/bool/and.h"
#include "monitors/expression_tree/nary/bool/or.h"
#include "monitors/expression_tree/nary/bool/xor.h"
#include "monitors/expression_tree/unary/bool/not.h"
#include "monitors/expression_tree/unary/temporal/future.h"
#include "monitors/expression_tree/unary/temporal/globally.h"
#include "monitors/expression_tree/unary/temporal/historically.h"
#include "monitors/expression_tree/unary/temporal/next_strong.h"
#include "monitors/expression_tree/unary/temporal/next_weak.h"
#include "monitors/expression_tree/unary/temporal/once.h"
#include "monitors/expression_tree/unary/temporal/previously.h"
#include "monitors/parser/gen/FormulaBaseVisitor.h"

/**
 * Visitor for parsing.
 */
class Visitor : public FormulaBaseVisitor {
  public:
    /**
     * Returns type of an expression.
     * @param expr Parsed expression.
     * @return The expression's type.
     */
    static std::shared_ptr<Expression> getType(const antlrcpp::Any &expr) {
        // Atomic
        if (expr.is<std::shared_ptr<Atomic>>()) {
            return expr.as<std::shared_ptr<Atomic>>();
        }
        // Constant
        if (expr.is<std::shared_ptr<Constant>>()) {
            return expr.as<std::shared_ptr<Constant>>();
        }
        if (expr.is<std::shared_ptr<Predicate>>()) {
            return expr.as<std::shared_ptr<Predicate>>();
        }
        // Not
        if (expr.is<std::shared_ptr<Not>>()) {
            return expr.as<std::shared_ptr<Not>>();
        }
        // And
        if (expr.is<std::shared_ptr<And>>()) {
            return expr.as<std::shared_ptr<And>>();
        }
        // Or
        if (expr.is<std::shared_ptr<Or>>()) {
            return expr.as<std::shared_ptr<Or>>();
        }
        // Xor
        if (expr.is<std::shared_ptr<Xor>>()) {
            return expr.as<std::shared_ptr<Xor>>();
        }
        // Implication
        if (expr.is<std::shared_ptr<Implication>>()) {
            return expr.as<std::shared_ptr<Implication>>();
        }
        // Equivalence
        if (expr.is<std::shared_ptr<Equivalence>>()) {
            return expr.as<std::shared_ptr<Equivalence>>();
        }
        // Globally
        if (expr.is<std::shared_ptr<Globally>>()) {
            return expr.as<std::shared_ptr<Globally>>();
        }
        // Future
        if (expr.is<std::shared_ptr<Future>>()) {
            return expr.as<std::shared_ptr<Future>>();
        }
        // NextStrong
        if (expr.is<std::shared_ptr<NextStrong>>()) {
            return expr.as<std::shared_ptr<NextStrong>>();
        }
        // NextWeak
        if (expr.is<std::shared_ptr<NextWeak>>()) {
            return expr.as<std::shared_ptr<NextWeak>>();
        }
        // Release
        if (expr.is<std::shared_ptr<Release>>()) {
            return expr.as<std::shared_ptr<Release>>();
        }
        // Until
        if (expr.is<std::shared_ptr<Until>>()) {
            return expr.as<std::shared_ptr<Until>>();
        }
        // Previously
        if (expr.is<std::shared_ptr<Previously>>()) {
            return expr.as<std::shared_ptr<Previously>>();
        }
        // Once
        if (expr.is<std::shared_ptr<Once>>()) {
            return expr.as<std::shared_ptr<Once>>();
        }
        // Since
        if (expr.is<std::shared_ptr<Since>>()) {
            return expr.as<std::shared_ptr<Since>>();
        }
        // Historically
        if (expr.is<std::shared_ptr<Historically>>()) {
            return expr.as<std::shared_ptr<Historically>>();
        }

        throw std::runtime_error("Visitor::getType() No type found. Check your formula string.");
    }

    /**
     * Visits a formula's expression.
     * @param ctx Context.
     * @return Formula object with expression built by visitor.
     */
    antlrcpp::Any visitFormula(FormulaParser::FormulaContext *ctx) override {
        auto expr = getType(visit(ctx->expr()));
        return Formula(expr);
    }

    /**
     * Recursively visit the children and build equivalence with them.
     * @param ctx Context.
     * @return Equivalence.
     */
    antlrcpp::Any visitEquivalence(FormulaParser::EquivalenceContext *ctx) override {
        auto left = getType(visit(ctx->left));
        auto right = getType(visit(ctx->right));
        return std::make_shared<Equivalence>(left, right);
    }

    /**
     * Recursively visits the children and builds an Implication with them.
     * @param ctx Context.
     * @return Implication.
     */
    antlrcpp::Any visitImplication(FormulaParser::ImplicationContext *ctx) override {
        auto left = getType(visit(ctx->left));
        auto right = getType(visit(ctx->right));
        return std::make_shared<Implication>(left, right);
    }

    /**
     * Visit the expression encoded by xorExpr.
     * @param ctx Context.
     * @return Result of visiting expression behind xorExpr.
     */
    antlrcpp::Any visitXorExpr(FormulaParser::XorExprContext *ctx) override { return visit(ctx->xor_expr()); }

    /**
     * Builds an Xor by recursively visiting its children or goes deeper into the expression tree if expression is not
     * an Xor.
     * @param ctx Context.
     * @return Xor if actual xorExpr visited, result of visitChildren() otherwise.
     */
    antlrcpp::Any visitXor(FormulaParser::XorContext *ctx) override {
        // Not an xor -> continue with children
        if ((ctx->op == nullptr) || (ctx->op->getType() != FormulaParser::XOR)) {
            return visitChildren(ctx);
        }
        Xor::SetType_T xorSet;
        // Create multiset
        for (auto &child : ctx->children) {
            // Operators are children, ignore them
            if (child->getText() != "xor" && child->getText() != "^") {
                std::shared_ptr<Expression> newExpr = getType(visit(child));
                xorSet.merge(std::multiset<std::shared_ptr<Expression>>{newExpr});
            }
        }
        return std::make_shared<Xor>(xorSet);
    }

    /**
     * Builds an Or by recursively visiting its children or goes deeper into the expression tree if expression is not an
     * Or.
     * @param ctx Context.
     * @return Or if actual orExpr visited, result of visitChildren() otherwise.
     */
    antlrcpp::Any visitOr(FormulaParser::OrContext *ctx) override {
        if ((ctx->op == nullptr) || (ctx->op->getType() != FormulaParser::OR)) {
            return visitChildren(ctx);
        }
        Or::SetType_T orSet;
        // Create multiset
        for (auto &child : ctx->children) {
            if (child->getText() != "OR" && child->getText() != "or" && child->getText() != "|") {
                std::shared_ptr<Expression> newExpr = getType(visit(child));
                orSet.merge(std::multiset<std::shared_ptr<Expression>>{newExpr});
            }
        }
        return std::make_shared<Or>(orSet);
    }

    /**
     * Builds an And by recursively visiting its children or goes deeper into the expression tree if expression is not
     * an And.
     * @param ctx Context.
     * @return And if actual orExpr visited, result of visitChildren() otherwise.
     */
    antlrcpp::Any visitAnd(FormulaParser::AndContext *ctx) override {
        if ((ctx->op == nullptr) || (ctx->op->getType() != FormulaParser::AND)) {
            return visitChildren(ctx);
        }
        And::SetType_T andSet;
        // Create multiset
        for (auto &child : ctx->children) {
            if (child->getText() != "AND" && child->getText() != "and" && child->getText() != "&") {
                std::shared_ptr<Expression> newExpr = getType(visit(child));
                andSet.merge(std::multiset<std::shared_ptr<Expression>>{newExpr});
            }
        }
        return std::make_shared<And>(andSet);
    }

    /**
     * Recursively visits the children and builds a Release with them.
     * @param ctx Context.
     * @return Release.
     */
    antlrcpp::Any visitRelease(FormulaParser::ReleaseContext *ctx) override {
        auto left = getType(visit(ctx->left));
        auto right = getType(visit(ctx->right));
        return std::make_shared<Release>(left, right);
    }

    /**
     * Recursively visits the children and builds an Until with them.
     * @param ctx Context.
     * @return Until.
     */
    antlrcpp::Any visitUntil(FormulaParser::UntilContext *ctx) override {
        auto left = getType(visit(ctx->left));
        auto right = getType(visit(ctx->right));
        return std::make_shared<Until>(left, right);
    }

    /**
     * Recursively visits the children and builds an interval-based Release with them.
     * @param ctx Context.
     * @return Release.
     */
    antlrcpp::Any visitReleaseInterval(FormulaParser::ReleaseIntervalContext *ctx) override {
        auto left = getType(visit(ctx->left));
        auto right = getType(visit(ctx->right));
        auto iv = visit(ctx->iv).as<std::shared_ptr<Interval>>();
        return std::make_shared<Release>(left, right, iv);
    }

    /**
     * Recursively visits the children and builds an interval-based Until with them.
     * @param ctx Context.
     * @return Until.
     */
    antlrcpp::Any visitUntilInterval(FormulaParser::UntilIntervalContext *ctx) override {
        auto left = getType(visit(ctx->left));
        auto right = getType(visit(ctx->right));
        auto iv = visit(ctx->iv).as<std::shared_ptr<Interval>>();
        return std::make_shared<Until>(left, right, iv);
    }

    /**
     * Recursively visits the children and builds a Since with them.
     * @param ctx Context.
     * @return Until.
     */
    antlrcpp::Any visitSince(FormulaParser::SinceContext *ctx) override {
        auto left = getType(visit(ctx->left));
        auto right = getType(visit(ctx->right));
        return std::make_shared<Since>(left, right);
    }

    /**
     * Recursively visits the children and builds an interval-based Since with them.
     * @param ctx Context.
     * @return Until.
     */
    antlrcpp::Any visitSinceInterval(FormulaParser::SinceIntervalContext *ctx) override {
        auto left = getType(visit(ctx->left));
        auto right = getType(visit(ctx->right));
        auto iv = visit(ctx->iv).as<std::shared_ptr<Interval>>();
        return std::make_shared<Since>(left, right, iv);
    }

    /**
     * Visits a unary expression.
     * @param ctx Context
     * @return Result of visiting unary.
     */
    antlrcpp::Any visitUnaryExpr(FormulaParser::UnaryExprContext *ctx) override { return visit(ctx->unary()); }

    /**
     * Recursively visits the child and builds a Globally with it.
     * @param ctx Context.
     * @return Globally.
     */
    antlrcpp::Any visitGlobally(FormulaParser::GloballyContext *ctx) override {
        auto right = getType(visit(ctx->right));
        return std::make_shared<Globally>(right);
    }

    /**
     * Recursively visits the child and builds a Historically with it.
     * @param ctx Context.
     * @return Historically.
     */
    antlrcpp::Any visitHistorically(FormulaParser::HistoricallyContext *ctx) override {
        auto right = getType(visit(ctx->right));
        return std::make_shared<Historically>(right);
    }

    /**
     * Recursively visits the child and builds a Future with it.
     * @param ctx Context.
     * @return Future.
     */
    antlrcpp::Any visitFuture(FormulaParser::FutureContext *ctx) override {
        auto right = getType(visit(ctx->right));
        return std::make_shared<Future>(right);
    }

    /**
     * Recursively visits the child and builds a Once with it.
     * @param ctx Context.
     * @return Once.
     */
    antlrcpp::Any visitOnce(FormulaParser::OnceContext *ctx) override {
        auto right = getType(visit(ctx->right));
        return std::make_shared<Once>(right);
    }

    /**
     * Recursively visits the child and builds a Previously with it.
     * @param ctx Context.
     * @return Previously.
     */
    antlrcpp::Any visitPreviously(FormulaParser::PreviouslyContext *ctx) override {
        auto right = getType(visit(ctx->right));
        return std::make_shared<Previously>(right);
    }

    /**
     * Recursively visits the child and builds a NextWeak with it.
     * @param ctx Context.
     * @return NextWeak.
     */
    antlrcpp::Any visitNextWeak(FormulaParser::NextWeakContext *ctx) override {
        auto right = getType(visit(ctx->right));
        return std::make_shared<NextWeak>(right);
    }

    /**
     * Recursively visits the child and builds a NextStrong with it.
     * @param ctx Context.
     * @return NextStrong.
     */
    antlrcpp::Any visitNextStrong(FormulaParser::NextStrongContext *ctx) override {
        auto right = getType(visit(ctx->right));
        return std::make_shared<NextStrong>(right);
    }

    /**
     * Recursively visits the child and builds an interval-based Globally with it.
     * @param ctx Context.
     * @return Globally.
     */
    antlrcpp::Any visitGloballyInterval(FormulaParser::GloballyIntervalContext *ctx) override {
        auto right = getType(visit(ctx->right));
        auto iv = visit(ctx->iv).as<std::shared_ptr<Interval>>();
        return std::make_shared<Globally>(right, iv);
    }

    /**
     * Recursively visits the child and builds an interval-based Historically with it.
     * @param ctx Context.
     * @return Historically.
     */
    antlrcpp::Any visitHistoricallyInterval(FormulaParser::HistoricallyIntervalContext *ctx) override {
        auto right = getType(visit(ctx->right));
        auto iv = visit(ctx->iv).as<std::shared_ptr<Interval>>();
        return std::make_shared<Historically>(right, iv);
    }

    /**
     * Recursively visits the child and builds an interval-based Future with it.
     * @param ctx Context.
     * @return Future.
     */
    antlrcpp::Any visitFutureInterval(FormulaParser::FutureIntervalContext *ctx) override {
        auto right = getType(visit(ctx->right));
        auto iv = visit(ctx->iv).as<std::shared_ptr<Interval>>();
        return std::make_shared<Future>(right, iv);
    }

    /**
     * Recursively visits the child and builds an interval-based Once with it.
     * @param ctx Context.
     * @return Once.
     */
    antlrcpp::Any visitOnceInterval(FormulaParser::OnceIntervalContext *ctx) override {
        auto right = getType(visit(ctx->right));
        auto iv = visit(ctx->iv).as<std::shared_ptr<Interval>>();
        return std::make_shared<Once>(right, iv);
    }

    /**
     * Recursively visits the child and builds an interval-based Previously with it.
     * @param ctx Context.
     * @return Previously.
     */
    antlrcpp::Any visitPreviouslyInterval(FormulaParser::PreviouslyIntervalContext *ctx) override {
        auto right = getType(visit(ctx->right));
        auto iv = visit(ctx->iv).as<std::shared_ptr<Interval>>();
        return std::make_shared<Previously>(right, iv);
    }

    /**
     * Recursively visits the child and builds an interval-based NextWeak with it.
     * @param ctx Context.
     * @return NextWeak.
     */
    antlrcpp::Any visitNextWeakInterval(FormulaParser::NextWeakIntervalContext *ctx) override {
        auto right = getType(visit(ctx->right));
        auto iv = visit(ctx->iv).as<std::shared_ptr<Interval>>();
        return std::make_shared<NextWeak>(right, iv);
    }

    /**
     * Recursively visits the child and builds an interval-based NextStrong with it.
     * @param ctx Context.
     * @return NextStrong.
     */
    antlrcpp::Any visitNextStrongInterval(FormulaParser::NextStrongIntervalContext *ctx) override {
        auto right = getType(visit(ctx->right));
        auto iv = visit(ctx->iv).as<std::shared_ptr<Interval>>();
        return std::make_shared<NextStrong>(right, iv);
    }

    /**
     * Recursively visits the child and builds a Not with it.
     * @param ctx Context.
     * @return Not.
     */
    antlrcpp::Any visitNot(FormulaParser::NotContext *ctx) override {
        auto right = getType(visit(ctx->right));
        return std::make_shared<Not>(right);
    }

    /**
     * Recursively visits the child and builds a Not with it.
     * @param ctx Context.
     * @return Not.
     */
    antlrcpp::Any visitNotExpr(FormulaParser::NotExprContext *ctx) override {
        auto right = getType(visit(ctx->right));
        return std::make_shared<Not>(right);
    }

    /**
     * Visits the atom in atomExpr.
     * @param ctx Context.
     * @return Result if visiting the atom in atomExpr.
     */
    antlrcpp::Any visitAtomExpr(FormulaParser::AtomExprContext *ctx) override { return visit(ctx->atom()); }

    /**
     * Visits a constant and builds a Constant with it.
     * @param ctx Content.
     * @return Constant according to given value.
     */
    antlrcpp::Any visitBoolean(FormulaParser::BooleanContext *ctx) override { return visit(ctx->bool_()); }

    /**
     * Visits a true constant and builds a Constant with it.
     * @param ctx Content.
     * @return True constant.
     */
    virtual antlrcpp::Any visitTrueBoolean(FormulaParser::TrueBooleanContext *context) override {
        return std::make_shared<Constant>(true);
    }

    /**
     * Visits a false constant and builds a Constant with it.
     * @param ctx Content.
     * @return False constant.
     */
    virtual antlrcpp::Any visitFalseBoolean(FormulaParser::FalseBooleanContext *context) override {
        return std::make_shared<Constant>(false);
    }

    /**
     * Ignores the brackets and visits the content inside them.
     * @param ctx Context.
     * @return Result of visiting content inside the bracktes.
     */
    antlrcpp::Any visitBrackets(FormulaParser::BracketsContext *ctx) override { return visit(ctx->content); }

    /**
     * Builds an Atomic from a string.
     * @param ctx Context.
     * @return Atomic.
     */
    antlrcpp::Any visitAtomic(FormulaParser::AtomicContext *ctx) override {
        return std::make_shared<Atomic>(Atomic(ctx->getText()));
    }

    /**
     * Builds an Atomic from a string.
     * @param ctx Context.
     * @return Atomic.
     */
    antlrcpp::Any visitFiniteInterval(FormulaParser::FiniteIntervalContext *ctx) override {
        bool finite{false};
        if (visit(ctx->u).as<int>() >= 0)
            finite = true;
        auto lower = visit(ctx->l).as<int>();
        auto upper = visit(ctx->u).as<int>();
        if (finite)
            return std::make_shared<Interval>(lower, upper);
        else
            return std::make_shared<Interval>(lower);
    }

    /**
     * Builds an Atomic from a string.
     * @param ctx Context.
     * @return Atomic.
     */
    antlrcpp::Any visitInfiniteInterval(FormulaParser::InfiniteIntervalContext *ctx) override {
        auto lower = visit(ctx->l).as<int>();
        return std::make_shared<Interval>(lower);
    }

    /**
     * Builds an Atomic from a string.
     * @param ctx Context.
     * @return Atomic.
     */
    antlrcpp::Any visitDigit(FormulaParser::DigitContext *ctx) override { return std::stoi(ctx->getText()); }

    antlrcpp::Any visitPredicate(FormulaParser::PredicateContext *ctx) override {
        const std::vector<std::string> invalidLiterals{"(", ")", ","};
        std::vector<std::string> params;
        for (size_t idx{1}; idx < ctx->children.size(); ++idx) {
            auto child{ctx->children.at(idx)->getText()};
            if (std::any_of(invalidLiterals.begin(), invalidLiterals.end(),
                            [child](const std::string &lit) { return lit == child; }))
                continue;
            params.push_back(child);
        }
        std::string name{ctx->getText().substr(0, ctx->getText().find('('))};
        std::string symbol{name};
        for (const auto &par : params)
            symbol += par + "_";
        symbol = symbol.substr(0, symbol.size() - 1);
        return std::make_shared<Predicate>(Predicate(symbol, name, params));
    }
};
