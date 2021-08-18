#pragma once

#include "monitors/formula/formula.h"
#include "monitors/trace/trace.h"
#include <vector>

struct Verdict {
    size_t ts;
    bool b;
    bool operator==(const Verdict &bv) const { return ts == bv.ts && b == bv.b; }
};

/**
 * Utils Several functions for working with formulas, traces, and events.
 */
namespace Utils {

/**
 * Computes how a boolean @param expr (containing only n-ary operators and not) evaluates on a given @param eventMap
 *
 * @param expr Expression to be evaluated.
 * @param eventMap Event.
 * @return True if @param expr is true in @param eventMap, false otherwise.
 */
[[nodiscard]] bool evaluateBooleanExpression(const std::shared_ptr<Expression> &expr,
                                             std::map<std::string, lazy_bool> &eventMap);

/**
 * Creates a Trace.
 *
 * @param atomics All possible predicates as strings.
 * @param trueAtomics Vector of vectors containing predicates that are true in each event.
 * @return Trace.
 */
[[nodiscard]] Trace createTrace(const std::set<std::string> &atomics,
                                std::vector<std::vector<std::string>> trueAtomics);

/**
 * Creates a Trace with lazy evaluation of the predicates.
 * @param atomicsAndFunctions map containing lazy predicates
 * @param timeSteps time steps for which predicates should be evaluated
 * @return
 */
[[nodiscard]] Trace createLazyTrace(const std::map<std::string, std::function<bool(size_t)>> &atomicsAndFunctions,
                                    const std::vector<size_t> &timeSteps);

/**
 * Creates a string set of all atomics in a formula. Wrapper for getAtomics().
 *
 * @param formula Formula.
 * @return string set of all predicates in @param formula.
 */
[[nodiscard]] std::set<std::string> getAtomics(const Formula &formula);

/**
 * Creates a string set of all predicates in a formula. Wrapper for getPredicates().
 *
 * @param formula Formula.
 * @return string set of all predicates in @param formula.
 */
[[nodiscard]] std::set<std::string> getPredicates(const Formula &formula);

/**
 * Creates a string set of all predicates in a formula. Wrapper for getPredicates().
 *
 * @param formula Formula.
 * @return string set of all predicates in @param formula.
 */
[[nodiscard]] std::set<std::string> getAtomicsAndPredicates(const Formula &formula);

/**
 * Creates a string set of all atomics in an expression. Wrapper for getAtomics().
 *
 * @param expr Expression.
 * @return string set of all predicates in @param expr.
 */
[[nodiscard]] std::set<std::string> getAtomics(const std::shared_ptr<Expression> &expr);

/**
 * Creates a string set of all predicates in an expression. Wrapper for getPredicates().
 *
 * @param expr Expression.
 * @return string set of all predicates in @param expr.
 */
[[nodiscard]] std::set<std::string> getPredicates(const std::shared_ptr<Expression> &expr);

/**
 * Creates a string set of all atomics in an expression.
 *
 * @param expr Expression.
 * @param atomics Set of all atomics that is built recursively.
 * @return string set of all atomics in @param expr.
 */
[[nodiscard]] std::set<std::string> getAtomics(const std::shared_ptr<Expression> &expr, std::set<std::string> atomics);

/**
 * Creates a string set of all predicates in an expression.
 *
 * @param expr Expression.
 * @param atomics Set of all predicates that is built recursively.
 * @return string set of all predicates in @param expr.
 */
[[nodiscard]] std::set<std::string> getPredicates(const std::shared_ptr<Expression> &expr,
                                                  std::set<std::string> predicates);

/**
 * Creates a string set of all atomics and predicates in an expression.
 *
 * @param expr Expression.
 * @param atomics Set of all atomics and predicates that is built recursively.
 * @return string set of all atomics and predicates in @param expr.
 */
[[nodiscard]] std::set<std::string> getAtomicsAndPredicates(const std::shared_ptr<Expression> &expr,
                                                            const std::set<std::string> &atomics);

/**
 * Creates a string set of all atomics and predicates in an expression.
 *
 * @param expr Expression.
 * @return string set of all atomics and predicates in @param expr.
 */
[[nodiscard]] std::set<std::string> getAtomicsAndPredicates(const std::shared_ptr<Expression> &expr);

/**
 * Creates a string set of all predicates in a formula. Wrapper for getPredicates().
 *
 * @param formula Formula.
 * @return string set of all predicates in @param formula.
 */
[[nodiscard]] std::map<std::string, std::tuple<std::string, std::vector<std::string>>>
getPredicatesAndParameters(const Formula &formula);

std::map<std::string, std::tuple<std::string, std::vector<std::string>>> getPredicatesAndParameters(
    const std::shared_ptr<Expression> &expr,
    std::map<std::string, std::tuple<std::string, std::vector<std::string>>> predicatesWithParameters);

/**
 * Computes the number of predicates in @param expr.
 *
 * @param expr Expression.
 * @return Number of predicates.
 */
[[nodiscard]] size_t getNumberOfAtomics(const std::shared_ptr<Expression> &expr);

/**
 * Computes the number of predicates and atomics in @param expr.
 *
 * @param expr Expression.
 * @return Number of predicates and atomics.
 */
[[nodiscard]] size_t getNumberOfAtomicsAndPredicates(const std::shared_ptr<Expression> &expr);

/**
 * Creates event mapping from predicates (=names) to values (=bools) and wraps it into a trace of length 1.
 *
 * @param atomics Names of predicates.
 * @param vector Values of predicates.
 * @param terminal Whether or not the event resulting should be marked as terminal.
 * @return Trace containing only the event defined by the input
 */
[[nodiscard]] Trace boolVectorToTrace(const std::set<std::string> &atomics, const std::vector<bool> &vector,
                                      bool terminal);

/**
 * Creates event mapping from predicates (=names) to values (=bools).
 *
 * @param atomics Names of predicates.
 * @param vector Values of predicates.
 * @param terminal Whether or not the event resulting should be marked as terminal.
 * @return Event containing passed event mapping.
 */
[[nodiscard]] Event boolVectorToEvent(const std::set<std::string> &atomics, const std::vector<bool> &vector,
                                      bool terminal);

/**
 * Creates a formula through conjunction of predicates in @param event.
 *
 * @param event Event.
 * @return Formula with conjunction of predicates.
 */
[[nodiscard]] Formula eventToFormula(Event &event);

/**
 * Creates a vector with all subformulas in @param formula including the formula itself with a BFS.
 *
 * @param formula Formula.
 * @return Vector of subformulas.
 */
[[nodiscard]] std::vector<std::shared_ptr<Expression>> subformulasBreadthFirstOrder(const Formula &formula);

/**
 * Creates a string representation of an iterable container @param arr.
 *
 * @tparam T Container type.
 * @tparam F Type of print function
 * @param arr Iterable container.
 * @param f function to transform the dereferenced iterator to a string.
 * @return String representation of arr.
 */
template <class T, class F>
[[nodiscard]] std::string iterableToString(const T &arr, F f,
                                           const std::pair<std::string, std::string> &delimiter = {"[", "]"},
                                           const std::string &separator = ", ") {
    auto it = std::begin(arr);
    const auto end = std::end(arr);
    if (it == end) {
        return delimiter.first + delimiter.second;
    }
    std::ostringstream strStream;
    strStream << delimiter.first << f(*it);
    for (++it; it != end; ++it) {
        strStream << separator << f(*it);
    }
    strStream << delimiter.second;
    return strStream.str();
}

/**
 *
 *
 * @tparam T Container type.
 * @param arr Iterable container.
 * @return String representation of arr.
 */
template <class T>[[nodiscard]] std::string iterableToString(const T &arr) {
    return iterableToString(arr, [](const auto &x) { return x; });
}

/**
 * Splits a string by multiple delimiters.
 *
 * @param SearchString.
 * @param Delimiters
 * @return Set of substrings.
 */
std::vector<std::string> splitString(const std::string &stringOriginal, const std::string &delimiters);

/**
 * Flips a pair pair.
 * @tparam A First type.
 * @tparam B Second type.
 * @param pair Pair to be flipped.
 * @return Flipped pair.
 */
template <typename A, typename B> std::pair<B, A> flip_pair(const std::pair<A, B> &pair) {
    return std::pair<B, A>(pair.second, pair.first);
}

/**
 * Flips a map.
 * @tparam A Key type.
 * @tparam B Value type.
 * @tparam Map Map type.
 * @tparam Args Additional arguments, optional.
 * @param map Map to be flipped.
 * @return Multimap with flipped key value pairs.
 */
template <typename A, typename B, template <class, class, class...> class Map, class... Args>
std::multimap<B, A> flip_map(const Map<A, B, Args...> &map) {
    std::multimap<B, A> flippedMap;
    std::transform(map.begin(), map.end(), std::inserter(flippedMap, flippedMap.begin()), flip_pair<A, B>);
    return flippedMap;
}

} // namespace Utils
