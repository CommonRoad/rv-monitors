#pragma once

#include "monitors/expression_tree/binary/binary.h"
#include "monitors/expression_tree/nary/nary_interface.h"
#include "monitors/expression_tree/unary/unary.h"
#include "monitors/trace/event.h"
#include <optional>
#include <utility>

/**
 * Class representing a formula specified in temporal logic.
 */
class Formula {
  private:
    std::string id;
    std::shared_ptr<Expression> expr;
    std::shared_ptr<Event> event = nullptr;

  public:
    /**
     * Constructor.
     * @param expr Expression as root of the expression tree.
     */
    explicit Formula(std::shared_ptr<Expression> expr) : expr(std::move(expr)) {}

    /**
     * Constructor.
     * @param expr Expression as root of the expression tree.
     */
    explicit Formula(std::shared_ptr<Expression> expr, std::string id) : id(std::move(id)), expr(std::move(expr)) {}

    /**
     * Getter for expression.
     * @return The formula's expression.
     */
    [[nodiscard]] std::shared_ptr<Expression> getExpr() const;

    /**
     * Setter for expression.
     * @param expression Expression.
     */
    void setExpr(const std::shared_ptr<Expression> &expression);

    /**
     * Getter for event.
     * @return event.
     */
    [[nodiscard]] const std::shared_ptr<Event> &getEvent() const;

    /**
     * Setter for event.
     * @param ev Event.
     */
    void setEvent(std::shared_ptr<Event> ev);

    /**
     * Transforms a formula to its negative normal form (NNF).
     */
    void toNnf();

    /**
     * to_string() for Formula.
     * @return String representation of this formula.
     */
    [[nodiscard]] std::string to_string() const;

    /**
     * Checks if specified atomic is in this formula.
     * @param atomic Atomic.
     * @return True if atomic is in formula, false otherwise.
     */
    [[nodiscard]] bool containsAtomic(const std::shared_ptr<Atomic> &atomic) const;

    /**
     * Checks if two expression trees are identical.
     * @param before First tree.
     * @param after Second tree.
     * @return True if they are identical, false otherwise.
     */
    [[nodiscard]] static bool sameTree(const std::shared_ptr<Expression> &before,
                                       const std::shared_ptr<Expression> &after);

    /**
     * Getter for formula id.
     * @param id Formula identifier
     */
    void setId(const std::string &id);

    /**
     * Creates a deep copy of the given expression.
     * @tparam T Type of expression.
     * @param exprOrig Expression to be deep copied.
     * @return Deep copy of exprOrig.
     */
    template <class T>[[nodiscard]] static std::shared_ptr<T> deepCopy(const std::shared_ptr<T> &exprOrig) {
        if (exprOrig) {
            auto ret = exprOrig->clone();
            // Nary
            if (auto nary = std::dynamic_pointer_cast<NaryInterface>(ret)) {
                if (auto curXor = std::dynamic_pointer_cast<Xor>(nary)) {
                    std::multiset<std::shared_ptr<Expression>> newSet;
                    for (const auto &itr : *nary)
                        newSet.emplace(deepCopy(itr->clone()));
                    nary->setExpressions(newSet.begin(), newSet.end());
                } else {
                    std::set<std::shared_ptr<Expression>> newSet;
                    for (const auto &itr : *nary)
                        newSet.emplace(deepCopy(itr->clone()));
                    nary->setExpressions(newSet.begin(), newSet.end());
                }
                ret = nary;
            }

            // Binary
            else if (auto binary = std::dynamic_pointer_cast<Binary>(ret)) {
                auto tmpLeft = deepCopy(binary->getLeft()->clone());
                binary->setLeft(tmpLeft);
                auto tmpRight = deepCopy(binary->getRight()->clone());
                binary->setRight(tmpRight);
                ret = binary;
            }

            // Unary
            else if (auto unary = std::dynamic_pointer_cast<Unary>(ret)) {
                auto tmpLeft = deepCopy(unary->getLeft()->clone());
                unary->setLeft(tmpLeft);
                ret = unary;
            }

            return ret;
        }
        return nullptr;
    }

    /**
     * Creates a deep copy of this formula' expression.
     * @return
     */
    [[nodiscard]] Formula deepCopy() const;

    /**
     * Getter for unique id/name;
     * @return ID/name of formula.
     */
    const std::string &getId() const;

    /**
     * Adjusted ostream operator.
     * @param os Stream.
     * @param formula Formula.
     * @return ostream.
     */
    friend std::ostream &operator<<(std::ostream &os, const Formula &formula);
};

/**
 * Custom < operator for comparing two formulas f1 and f2 by their string representations.
 * @param f1 Formula.
 * @param f2 Formula.
 * @return True if f1 < f2, false otherwise.
 */
bool operator<(const Formula &f1, const Formula &f2);

/**
 * Custom == operator for comparing two formulas f1 and f2 by their string representations.
 * @param f1 Formula.
 * @param f2 Formula.
 * @return True if f1 == f2, false otherwise.
 */
bool operator==(const Formula &f1, const Formula &f2);
