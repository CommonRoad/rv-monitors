#include "monitors/parser/visitor.h"

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
#include "monitors/expression_tree/unary/first_order/counting_quantifier.h"
#include "monitors/expression_tree/unary/first_order/existential_quantifier.h"
#include "monitors/expression_tree/unary/first_order/quote_quantifier.h"
#include "monitors/expression_tree/unary/first_order/universal_quantifier.h"
#include "monitors/expression_tree/unary/temporal/future.h"
#include "monitors/expression_tree/unary/temporal/globally.h"
#include "monitors/expression_tree/unary/temporal/historically.h"
#include "monitors/expression_tree/unary/temporal/next_strong.h"
#include "monitors/expression_tree/unary/temporal/next_weak.h"
#include "monitors/expression_tree/unary/temporal/once.h"
#include "monitors/expression_tree/unary/temporal/previously.h"

std::shared_ptr<Expression> Visitor::getType(const std::any &expr) {
    // Atomic
    std::string exprTypeName{expr.type().name()};
    if (exprTypeName == typeid(std::shared_ptr<Atomic>).name()) {
        return std::any_cast<std::shared_ptr<Atomic>>(expr);
    }
    // Constant
    if (exprTypeName == typeid(std::shared_ptr<Constant>).name()) {
        return std::any_cast<std::shared_ptr<Constant>>(expr);
    }
    if (exprTypeName == typeid(std::shared_ptr<Predicate>).name()) {
        return std::any_cast<std::shared_ptr<Predicate>>(expr);
    }
    // Not
    if (exprTypeName == typeid(std::shared_ptr<Not>).name()) {
        return std::any_cast<std::shared_ptr<Not>>(expr);
    }
    // And
    if (exprTypeName == typeid(std::shared_ptr<And>).name()) {
        return std::any_cast<std::shared_ptr<And>>(expr);
    }
    // Or
    if (exprTypeName == typeid(std::shared_ptr<Or>).name()) {
        return std::any_cast<std::shared_ptr<Or>>(expr);
    }
    // Xor
    if (exprTypeName == typeid(std::shared_ptr<Xor>).name()) {
        return std::any_cast<std::shared_ptr<Xor>>(expr);
    }
    // Implication
    if (exprTypeName == typeid(std::shared_ptr<Implication>).name()) {
        return std::any_cast<std::shared_ptr<Implication>>(expr);
    }
    // Equivalence
    if (exprTypeName == typeid(std::shared_ptr<Equivalence>).name()) {
        return std::any_cast<std::shared_ptr<Equivalence>>(expr);
    }
    // Globally
    if (exprTypeName == typeid(std::shared_ptr<Globally>).name()) {
        return std::any_cast<std::shared_ptr<Globally>>(expr);
    }
    // Future
    if (exprTypeName == typeid(std::shared_ptr<Future>).name()) {
        return std::any_cast<std::shared_ptr<Future>>(expr);
    }
    // NextStrong
    if (exprTypeName == typeid(std::shared_ptr<NextStrong>).name()) {
        return std::any_cast<std::shared_ptr<NextStrong>>(expr);
    }
    // NextWeak
    if (exprTypeName == typeid(std::shared_ptr<NextWeak>).name()) {
        return std::any_cast<std::shared_ptr<NextWeak>>(expr);
    }
    // Release
    if (exprTypeName == typeid(std::shared_ptr<Release>).name()) {
        return std::any_cast<std::shared_ptr<Release>>(expr);
    }
    // Until
    if (exprTypeName == typeid(std::shared_ptr<Until>).name()) {
        return std::any_cast<std::shared_ptr<Until>>(expr);
    }
    // Previously
    if (exprTypeName == typeid(std::shared_ptr<Previously>).name()) {
        return std::any_cast<std::shared_ptr<Previously>>(expr);
    }
    // Once
    if (exprTypeName == typeid(std::shared_ptr<Once>).name()) {
        return std::any_cast<std::shared_ptr<Once>>(expr);
    }
    // Since
    if (exprTypeName == typeid(std::shared_ptr<Since>).name()) {
        return std::any_cast<std::shared_ptr<Since>>(expr);
    }
    // Historically
    if (exprTypeName == typeid(std::shared_ptr<Historically>).name()) {
        return std::any_cast<std::shared_ptr<Historically>>(expr);
    }
    // Universal
    if (exprTypeName == typeid(std::shared_ptr<UniversalQuantifier>).name()) {
        return std::any_cast<std::shared_ptr<UniversalQuantifier>>(expr);
    }
    // Existential
    if (exprTypeName == typeid(std::shared_ptr<ExistentialQuantifier>).name()) {
        return std::any_cast<std::shared_ptr<ExistentialQuantifier>>(expr);
    }
    // Quote
    if (exprTypeName == typeid(std::shared_ptr<QuoteQuantifier>).name()) {
        return std::any_cast<std::shared_ptr<QuoteQuantifier>>(expr);
    }
    // Counting
    if (exprTypeName == typeid(std::shared_ptr<CountingQuantifier>).name()) {
        return std::any_cast<std::shared_ptr<CountingQuantifier>>(expr);
    }
    // Domain
    if (exprTypeName == typeid(std::shared_ptr<Domain>).name()) {
        return std::any_cast<std::shared_ptr<Domain>>(expr);
    }

    throw std::runtime_error("Visitor::getType() No type found. Check your formula string.");
}

std::any Visitor::visitFormula(FormulaParser::FormulaContext *ctx) {
    auto expr = getType(visit(ctx->expr()));
    return Formula(expr);
}

std::any Visitor::visitEquivalence(FormulaParser::EquivalenceContext *ctx) {
    auto left = getType(visit(ctx->left));
    auto right = getType(visit(ctx->right));
    return std::make_shared<Equivalence>(left, right);
}

std::any Visitor::visitImplication(FormulaParser::ImplicationContext *ctx) {
    auto left = getType(visit(ctx->left));
    auto right = getType(visit(ctx->right));
    return std::make_shared<Implication>(left, right);
}

std::any Visitor::visitXorExpr(FormulaParser::XorExprContext *ctx) { return visit(ctx->xor_expr()); }

std::any Visitor::visitXor(FormulaParser::XorContext *ctx) {
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

std::any Visitor::visitOr(FormulaParser::OrContext *ctx) {
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

std::any Visitor::visitAnd(FormulaParser::AndContext *ctx) {
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

std::any Visitor::visitRelease(FormulaParser::ReleaseContext *ctx) {
    auto left = getType(visit(ctx->left));
    auto right = getType(visit(ctx->right));
    return std::make_shared<Release>(left, right);
}

std::any Visitor::visitUntil(FormulaParser::UntilContext *ctx) {
    auto left = getType(visit(ctx->left));
    auto right = getType(visit(ctx->right));
    return std::make_shared<Until>(left, right);
}

std::any Visitor::visitReleaseInterval(FormulaParser::ReleaseIntervalContext *ctx) {
    auto left = getType(visit(ctx->left));
    auto right = getType(visit(ctx->right));
    auto inv = std::any_cast<std::shared_ptr<Interval>>(visit(ctx->iv));
    return std::make_shared<Release>(left, right, inv);
}

std::any Visitor::visitUntilInterval(FormulaParser::UntilIntervalContext *ctx) {
    auto left = getType(visit(ctx->left));
    auto right = getType(visit(ctx->right));
    auto inv = std::any_cast<std::shared_ptr<Interval>>(visit(ctx->iv));
    return std::make_shared<Until>(left, right, inv);
}

std::any Visitor::visitSince(FormulaParser::SinceContext *ctx) {
    auto left = getType(visit(ctx->left));
    auto right = getType(visit(ctx->right));
    return std::make_shared<Since>(left, right);
}

std::any Visitor::visitSinceInterval(FormulaParser::SinceIntervalContext *ctx) {
    auto left = getType(visit(ctx->left));
    auto right = getType(visit(ctx->right));
    auto inv = std::any_cast<std::shared_ptr<Interval>>(visit(ctx->iv));
    return std::make_shared<Since>(left, right, inv);
}

std::any Visitor::visitUnaryExpr(FormulaParser::UnaryExprContext *ctx) { return visit(ctx->unary()); }

std::any Visitor::visitGlobally(FormulaParser::GloballyContext *ctx) {
    auto right = getType(visit(ctx->right));
    return std::make_shared<Globally>(right);
}

std::any Visitor::visitHistorically(FormulaParser::HistoricallyContext *ctx) {
    auto right = getType(visit(ctx->right));
    return std::make_shared<Historically>(right);
}

std::any Visitor::visitFuture(FormulaParser::FutureContext *ctx) {
    auto right = getType(visit(ctx->right));
    return std::make_shared<Future>(right);
}

std::any Visitor::visitOnce(FormulaParser::OnceContext *ctx) {
    auto right = getType(visit(ctx->right));
    return std::make_shared<Once>(right);
}

std::any Visitor::visitPreviously(FormulaParser::PreviouslyContext *ctx) {
    auto right = getType(visit(ctx->right));
    return std::make_shared<Previously>(right);
}

std::any Visitor::visitNextWeak(FormulaParser::NextWeakContext *ctx) {
    auto right = getType(visit(ctx->right));
    return std::make_shared<NextWeak>(right);
}

std::any Visitor::visitNextStrong(FormulaParser::NextStrongContext *ctx) {
    auto right = getType(visit(ctx->right));
    return std::make_shared<NextStrong>(right);
}

std::any Visitor::visitGloballyInterval(FormulaParser::GloballyIntervalContext *ctx) {
    auto right = getType(visit(ctx->right));
    auto inv = std::any_cast<std::shared_ptr<Interval>>(visit(ctx->iv));
    return std::make_shared<Globally>(right, inv);
}

std::any Visitor::visitHistoricallyInterval(FormulaParser::HistoricallyIntervalContext *ctx) {
    auto right = getType(visit(ctx->right));
    auto inv = std::any_cast<std::shared_ptr<Interval>>(visit(ctx->iv));
    return std::make_shared<Historically>(right, inv);
}

std::any Visitor::visitFutureInterval(FormulaParser::FutureIntervalContext *ctx) {
    auto right = getType(visit(ctx->right));
    auto inv = std::any_cast<std::shared_ptr<Interval>>(visit(ctx->iv));
    return std::make_shared<Future>(right, inv);
}

std::any Visitor::visitOnceInterval(FormulaParser::OnceIntervalContext *ctx) {
    auto right = getType(visit(ctx->right));
    auto inv = std::any_cast<std::shared_ptr<Interval>>(visit(ctx->iv));
    return std::make_shared<Once>(right, inv);
}

std::any Visitor::visitPreviouslyInterval(FormulaParser::PreviouslyIntervalContext *ctx) {
    auto right = getType(visit(ctx->right));
    auto inv = std::any_cast<std::shared_ptr<Interval>>(visit(ctx->iv));
    return std::make_shared<Previously>(right, inv);
}

std::any Visitor::visitNextWeakInterval(FormulaParser::NextWeakIntervalContext *ctx) {
    auto right = getType(visit(ctx->right));
    auto inv = std::any_cast<std::shared_ptr<Interval>>(visit(ctx->iv));
    return std::make_shared<NextWeak>(right, inv);
}

std::any Visitor::visitNextStrongInterval(FormulaParser::NextStrongIntervalContext *ctx) {
    auto right = getType(visit(ctx->right));
    auto inv = std::any_cast<std::shared_ptr<Interval>>(visit(ctx->iv));
    return std::make_shared<NextStrong>(right, inv);
}

std::any Visitor::visitNot(FormulaParser::NotContext *ctx) {
    auto right = getType(visit(ctx->right));
    return std::make_shared<Not>(right);
}

std::any Visitor::visitNotExpr(FormulaParser::NotExprContext *ctx) {
    auto right = getType(visit(ctx->right));
    return std::make_shared<Not>(right);
}

std::any Visitor::visitAtomExpr(FormulaParser::AtomExprContext *ctx) { return visit(ctx->atom()); }

std::any Visitor::visitBoolean(FormulaParser::BooleanContext *ctx) { return visit(ctx->bool_()); }

std::any Visitor::visitTrueBoolean(FormulaParser::TrueBooleanContext *context) {
    return std::make_shared<Constant>(true);
}

std::any Visitor::visitFalseBoolean(FormulaParser::FalseBooleanContext *context) {
    return std::make_shared<Constant>(false);
}

std::any Visitor::visitBrackets(FormulaParser::BracketsContext *ctx) { return visit(ctx->content); }

std::any Visitor::visitAtomic(FormulaParser::AtomicContext *ctx) {
    return std::make_shared<Atomic>(Atomic(ctx->getText()));
}

std::any Visitor::visitFiniteInterval(FormulaParser::FiniteIntervalContext *ctx) {
    bool finite{false};
    if (std::any_cast<int>(visit(ctx->u)) >= 0)
        finite = true;
    auto lower = std::any_cast<int>(visit(ctx->l));
    auto upper = std::any_cast<int>(visit(ctx->u));
    if (finite)
        return std::make_shared<Interval>(lower, upper);
    else
        return std::make_shared<Interval>(lower);
}

std::any Visitor::visitInfiniteInterval(FormulaParser::InfiniteIntervalContext *ctx) {
    auto lower = std::any_cast<int>(visit(ctx->l));
    return std::make_shared<Interval>(lower);
}

std::any Visitor::visitDigit(FormulaParser::DigitContext *ctx) { return std::stoi(ctx->getText()); }

std::any Visitor::visitPredicate(FormulaParser::PredicateContext *ctx) {
    auto params = extractFunctionParameters(ctx);
    std::string name{ctx->getText().substr(0, ctx->getText().find('('))};
    std::string symbol{name + "_" + std::get<2>(params)};
    return std::make_shared<Predicate>(Predicate(symbol, name, std::get<0>(params), std::get<1>(params)));
}

std::tuple<std::vector<std::string>, std::vector<std::string>, std::string>
Visitor::extractFunctionParameters(FormulaParser::UnaryContext *ctx) {
    const std::vector<std::string> invalidLiterals{"(", ")", ","};
    std::vector<std::string> paramsVar;
    std::vector<std::string> allParams;
    std::string name;
    for (size_t idx{1}; idx < ctx->children.size(); ++idx) {
        auto child{ctx->children.at(idx)->getText()};
        if (std::any_of(invalidLiterals.begin(), invalidLiterals.end(),
                        [child](const std::string &lit) { return lit == child; }))
            continue;
        if (child[0] == '\'' and child[child.length() - 1] == '\'') {
            child = child.substr(1, child.length() - 2);
            allParams.push_back(child);
        } else {
            allParams.push_back(child);
            paramsVar.push_back(child);
        }
        name += "_" + child;
    }
    return {allParams, paramsVar, name};
}

std::any Visitor::visitUniversal(FormulaParser::UniversalContext *ctx) {
    auto right = getType(visit(ctx->right));
    auto dva = std::any_cast<std::vector<Variable>>(visit(ctx->dv));
    return std::make_shared<UniversalQuantifier>(right, dva);
}

std::any Visitor::visitExistential(FormulaParser::ExistentialContext *ctx) {
    auto right = getType(visit(ctx->right));
    auto dva = std::any_cast<std::vector<Variable>>(visit(ctx->dv));
    return std::make_shared<ExistentialQuantifier>(right, dva);
}

std::any Visitor::visitQuote(FormulaParser::QuoteContext *ctx) {
    auto piv = std::any_cast<QuoteInterval>(visit(ctx->piv));
    auto right = getType(visit(ctx->right));
    auto dva = std::any_cast<std::vector<Variable>>(visit(ctx->dv));
    return std::make_shared<QuoteQuantifier>(right, dva, piv);
}

std::any Visitor::visitCounting(FormulaParser::CountingContext *ctx) {
    auto opStr = ctx->cop->getText();
    CountingQuantifier::Operator ope;
    if (opStr == "<=")
        ope = CountingQuantifier::LESS_EQ;
    else if (opStr == "=")
        ope = CountingQuantifier::EQ;
    else
        ope = CountingQuantifier::GREATER_EQ;
    auto num = std::stol(ctx->num->getText(), nullptr, 10);
    auto right = getType(visit(ctx->right));
    auto dva = std::any_cast<std::vector<Variable>>(visit(ctx->dv));
    return std::make_shared<CountingQuantifier>(right, dva, ope, num);
}

std::any Visitor::visitFiniteQuoteInterval(FormulaParser::FiniteQuoteIntervalContext *ctx) {
    auto lower = std::any_cast<float>(visit(ctx->l));
    auto upper = std::any_cast<float>(visit(ctx->u));

    return QuoteInterval{lower, upper};
}

std::any Visitor::visitDomainVariables(FormulaParser::DomainVariablesContext *ctx) {
    std::vector<Variable> variables;

    const std::vector<std::string> invalidLiterals{"(", ")", ","};

    std::vector<std::string> names;
    for (size_t idx = 0; idx < ctx->children.size(); idx++) {
        auto child = ctx->children.at(idx)->getText();
        if (std::any_of(invalidLiterals.begin(), invalidLiterals.end(),
                        [child](const std::string &lit) { return lit == child; }))
            continue;

        if (child == "in") {
            std::shared_ptr<Domain> domain{std::any_cast<std::shared_ptr<Domain>>(visit(ctx->children.at(idx + 1)))};
            for (const auto &name : names)
                variables.emplace_back(name, *domain);
            names.clear();
            idx++;
            continue;
        }

        names.push_back(child);
    }
    return variables;
}

std::any Visitor::visitStringDomain(FormulaParser::StringDomainContext *ctx) {
    return std::make_shared<Domain>(Domain(ctx->getText(), ctx->getText(), {}, {}));
}

std::any Visitor::visitFunctionDomain(FormulaParser::FunctionDomainContext *ctx) {
    auto params = extractFunctionParameters(reinterpret_cast<FormulaParser::UnaryContext *>(ctx));
    std::string name{ctx->getText().substr(0, ctx->getText().find('('))};
    std::string symbol{name + "_" + std::get<2>(params)};
    return std::make_shared<Domain>(Domain(symbol, name, std::get<0>(params), std::get<1>(params)));
}

std::any Visitor::visitQuoteDigit(FormulaParser::QuoteDigitContext *ctx) { return std::stof(ctx->getText()); }
