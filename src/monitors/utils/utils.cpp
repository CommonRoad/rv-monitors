#include <algorithm>
#include <iostream>
#include <queue>
#include <set>

#include "monitors/expression_tree/atomic/constant.h"
#include "monitors/expression_tree/binary/binary.h"
#include "monitors/expression_tree/nary/bool/and.h"
#include "monitors/expression_tree/nary/bool/or.h"
#include "monitors/expression_tree/nary/bool/xor.h"
#include "monitors/expression_tree/nary/nary.h"
#include "monitors/expression_tree/unary/bool/not.h"
#include "monitors/expression_tree/unary/unary.h"
#include "monitors/expression_tree/atomic/predicate.h"
#include "monitors/expression_tree/atomic/atomic.h"
#include "monitors/parser/parser.h"
#include "monitors/trace/trace.h"
#include "utils.h"

bool Utils::evaluateBooleanExpression(const std::shared_ptr<Expression> &expr, event_map_t<bool> &eventMap) {
    // Atomic
    if (auto atomicExpr = std::dynamic_pointer_cast<Atomic>(expr)) {
        return eventMap.at(atomicExpr->to_string());
    }
    // Predicate
    if (auto predicateExpr = std::dynamic_pointer_cast<Predicate>(expr)) {
        return eventMap.at(predicateExpr->to_string());
    }
    // Constant
    if (auto constantExpr = std::dynamic_pointer_cast<Constant>(expr)) {
        return constantExpr->getValue();
    }
    // Not
    if (auto notExpr = std::dynamic_pointer_cast<Not>(expr)) {
        return not evaluateBooleanExpression(notExpr->getLeft(), eventMap);
    }
    // And
    if (auto andExpr = std::dynamic_pointer_cast<And>(expr)) {
        auto res = true;
        for (const auto &childExpr : *andExpr) {
            res and_eq evaluateBooleanExpression(childExpr, eventMap);
            // Short-circuit: if anything in the And is false the whole expression must be false.
            if (not res) {
                break;
            }
        }
        return res;
    }

    // Or
    if (auto orExpr = std::dynamic_pointer_cast<Or>(expr)) {
        auto res = false;
        for (const auto &childExpr : *orExpr) {
            res or_eq evaluateBooleanExpression(childExpr, eventMap);
            // Short-circuit: if anything in the Or is true the whole expression must be true.
            if (res) {
                break;
            }
        }

        return res;
    }

    // Xor
    if (auto xorExpr = std::dynamic_pointer_cast<Xor>(expr)) {
        auto res = evaluateBooleanExpression(*xorExpr->begin(), eventMap);
        for (auto itr = ++xorExpr->begin(); itr != xorExpr->end(); ++itr) {
            res xor_eq static_cast<int>(evaluateBooleanExpression(*itr, eventMap));
        }

        return res;
    }
    return false;
}

Trace Utils::createTrace(const std::vector<std::vector<std::string>> &trueAtomics, const std::vector<size_t> &timeSteps,
                         const std::shared_ptr<Formula> &formula) {
    std::set<std::string> atomics;
    for (const auto &step : trueAtomics)
        for (const auto &elem : step)
            atomics.emplace(elem);
    auto formulaAtomics{getAtomicsAndPredicates(formula->getExpr())};
    std::set<std::string> mergedAtomics;
    std::set_union(atomics.begin(), atomics.end(), formulaAtomics.begin(), formulaAtomics.end(),
                   std::inserter(mergedAtomics, mergedAtomics.begin()));
    return createTrace(mergedAtomics, trueAtomics, timeSteps);
}

// Wrapper
std::set<std::string> Utils::getAtomics(const Formula &formula) { return getAtomics(formula.getExpr(), {}); }

// Wrapper
std::set<std::string> Utils::getPredicates(const Formula &formula) { return getPredicates(formula.getExpr(), {}); }

// Wrapper
std::map<std::string, std::tuple<std::string, std::vector<std::string>>>
Utils::getPredicatesAndParameters(const Formula &formula) {
    return getPredicatesAndParameters(formula.getExpr(), {});
}

// Wrapper
std::set<std::string> Utils::getAtomics(const std::shared_ptr<Expression> &expr) { return getAtomics(expr, {}); }

std::set<std::string> Utils::getAtomics(const std::shared_ptr<Expression> &expr, std::set<std::string> atomics) {
    if (!expr) {
        return atomics;
    }

    // Nary
    if (auto nary = std::dynamic_pointer_cast<NaryInterface>(expr)) {
        for (const auto &itr : *nary) {
            atomics = getAtomics(itr, atomics);
        }
    }

    // Binary
    else if (auto binary = std::dynamic_pointer_cast<Binary>(expr)) {
        atomics = getAtomics(binary->getLeft(), atomics);
        atomics = getAtomics(binary->getRight(), atomics);
    }

    // Unary
    else if (auto unary = std::dynamic_pointer_cast<Unary>(expr)) {
        atomics = getAtomics(unary->getLeft(), atomics);
    }

    // Atomic
    else if (auto atomic = std::dynamic_pointer_cast<Atomic>(expr)) {
        atomics.emplace(atomic->to_string());
    }

    // Don't count constants

    return atomics;
}

std::set<std::string> Utils::getPredicates(const std::shared_ptr<Expression> &expr, std::set<std::string> predicates) {
    if (!expr) {
        return predicates;
    }

    // Nary
    if (auto nary = std::dynamic_pointer_cast<NaryInterface>(expr)) {
        for (const auto &itr : *nary) {
            predicates = getPredicates(itr, predicates);
        }
    }

    // Binary
    else if (auto binary = std::dynamic_pointer_cast<Binary>(expr)) {
        predicates = getPredicates(binary->getLeft(), predicates);
        predicates = getPredicates(binary->getRight(), predicates);
    }

    // Unary
    else if (auto unary = std::dynamic_pointer_cast<Unary>(expr)) {
        predicates = getPredicates(unary->getLeft(), predicates);
    }

    // Predicate
    else if (auto predicate = std::dynamic_pointer_cast<Predicate>(expr)) {
        predicates.emplace(predicate->to_string());
    }

    // Don't count constants and atomics

    return predicates;
}

std::set<std::string> Utils::getAtomicsAndPredicates(const std::shared_ptr<Expression> &expr,
                                                     const std::set<std::string> &atomics) {
    auto collectedAtomics{getAtomics(expr, atomics)};
    auto predicates{getPredicates(expr, atomics)};
    std::set<std::string> combinedSet;
    std::set_union(collectedAtomics.begin(), collectedAtomics.end(), predicates.begin(), predicates.end(),
                   std::inserter(combinedSet, combinedSet.begin()));
    return combinedSet;
}

std::set<std::string> Utils::getAtomicsAndPredicates(const std::shared_ptr<Expression> &expr) {
    return getAtomicsAndPredicates(expr, {});
}

std::set<std::string> Utils::getAtomicsAndPredicates(const Formula &formula) {
    return getAtomicsAndPredicates(formula.getExpr());
}

std::map<std::string, std::tuple<std::string, std::vector<std::string>>> Utils::getPredicatesAndParameters(
    const std::shared_ptr<Expression> &expr,
    std::map<std::string, std::tuple<std::string, std::vector<std::string>>> predicatesWithParameters) {
    if (!expr) {
        return predicatesWithParameters;
    }

    // Nary
    if (auto nary = std::dynamic_pointer_cast<NaryInterface>(expr)) {
        for (const auto &itr : *nary) {
            predicatesWithParameters = getPredicatesAndParameters(itr, predicatesWithParameters);
        }
    }
    // Binary
    else if (auto binary = std::dynamic_pointer_cast<Binary>(expr)) {
        predicatesWithParameters = getPredicatesAndParameters(binary->getLeft(), predicatesWithParameters);
        predicatesWithParameters = getPredicatesAndParameters(binary->getRight(), predicatesWithParameters);
    }
    // Unary
    else if (auto unary = std::dynamic_pointer_cast<Unary>(expr)) {
        predicatesWithParameters = getPredicatesAndParameters(unary->getLeft(), predicatesWithParameters);
    }
    // Predicate
    else if (auto predicate = std::dynamic_pointer_cast<Predicate>(expr)) {
        predicatesWithParameters.insert({predicate->to_string(), {predicate->getName(), predicate->getParameters()}});
    }

    // Don't count constants and atomics

    return predicatesWithParameters;
}

size_t Utils::getNumberOfAtomics(const std::shared_ptr<Expression> &expr) { return getAtomics(expr, {}).size(); }

size_t Utils::getNumberOfAtomicsAndPredicates(const std::shared_ptr<Expression> &expr) {
    return getAtomics(expr, {}).size() + getPredicates(expr, {}).size();
}

Trace Utils::boolVectorToTrace(const std::set<std::string> &atomics, const std::vector<bool> &vector, bool terminal) {
    Event event = boolVectorToEvent(atomics, vector, terminal);
    return Trace{{event}};
}

// TODO terminal not needed (additional Event constructor)
Event<bool> Utils::boolVectorToEvent(const std::set<std::string> &atomics, const std::vector<bool> &vector, bool terminal) {
    event_map_t<bool> eventMap;
    size_t idx{0};
    for (const auto &atomic : atomics) {
        eventMap.emplace(atomic, LazyFunction(vector[idx]));
        ++idx;
    }
    return Event(std::move(eventMap), 0, terminal); // TODO check whether other/correct time step should be used
}

Formula Utils::eventToFormula(Event<bool> &event) {
    event_map_t<bool> eventMap = event.getEventMap();
    std::shared_ptr<Expression> firstExpr, secondExpr;

    And::SetType_T newSet;
    for (auto &[atomicName, atomicValue] : eventMap) {
        if (not atomicValue) {
            newSet.emplace(std::make_shared<Not>(std::make_shared<Atomic>(atomicName)));
        } else {
            newSet.emplace(std::make_shared<Atomic>(atomicName));
        }
    }
    return Formula(std::make_shared<And>(newSet));
}

std::vector<std::shared_ptr<Expression>> Utils::subformulasBreadthFirstOrder(const Formula &formula) {
    std::queue<std::shared_ptr<Expression>> queue;
    std::vector<std::shared_ptr<Expression>> subformulas;
    if (!formula.getExpr()) {
        return subformulas;
    }
    queue.push(formula.getExpr());
    while (!queue.empty()) {
        auto currExpr = queue.front();
        queue.pop();
        //        // Only add if not yet in subformulas
        //        if (std::find_if(subformulas.begin(), subformulas.end(),
        //                         [&](const std::shared_ptr<Expression> &expr) { return currExpr->to_string() ==
        //                         expr->to_string(); }) ==
        //            subformulas.end()) {
        subformulas.push_back(currExpr);
        //}

        // Nary
        if (auto nary = std::dynamic_pointer_cast<NaryInterface>(currExpr)) {
            for (const auto &itr : *nary) {
                queue.push(itr);
            }
        }

        // Binary
        else if (auto binary = std::dynamic_pointer_cast<Binary>(currExpr)) {
            queue.push(binary->getLeft());
            queue.push(binary->getRight());
        }

        // Unary
        else if (auto unary = std::dynamic_pointer_cast<Unary>(currExpr)) {
            queue.push(unary->getLeft());
        }
    }
    //    for (auto f:subformulas) {
    //        std::cout << f->to_string() << std::endl;
    //    }
    return subformulas;
}

std::vector<std::string> Utils::splitString(const std::string &stringOriginal, const std::string &delimiters) {
    std::vector<std::string> exprVector;

    std::size_t previous = 0, pos;
    while ((pos = stringOriginal.find_first_of(delimiters, previous)) != std::string::npos) {
        if (pos > previous) {
            exprVector.push_back(stringOriginal.substr(previous, pos - previous));
        }
        previous = pos + 1;
    }
    if (previous < stringOriginal.length()) {
        exprVector.push_back(stringOriginal.substr(previous, std::string::npos));
    }

    return exprVector;
}

Trace Utils::createLazyTrace(const std::map<std::string, std::function<bool(size_t)>> &atomicsAndFunctions,
                             const std::vector<size_t> &timeSteps) {
    std::vector<Event<bool>> finalTrace;
    size_t length = timeSteps.size();
    finalTrace.reserve(length);
    size_t initialTimeStep{timeSteps.at(0)};

    for (auto const idx : timeSteps) {
        event_map_t<bool> event;

        event.reserve(atomicsAndFunctions.size());

        for (auto const &[atomic, valueFetcher] : atomicsAndFunctions) {
            auto valueFetcherBound = std::bind(valueFetcher, idx);
            event.emplace(atomic, LazyFunction<bool>(std::move(valueFetcherBound)));
        }
        finalTrace.emplace_back(std::move(event), idx, idx == length + initialTimeStep - 1);
    }
    return Trace{std::move(finalTrace)};
}

Trace Utils::createTrace(const std::set<std::string> &atomics,
                         const std::vector<std::vector<std::string>> &trueAtomics,
                         const std::vector<size_t> &timeSteps,
                         const std::map<std::string, std::function<bool(size_t)>> &atomicsAndFunctions,
                         const std::map<std::string, std::function<std::array<size_t , 2>(size_t)>> &dynamicInterval){
    if (!timeSteps.empty() and trueAtomics.size() != timeSteps.size())
        throw std::runtime_error("Utils::createTrace: Number time steps does not correspond to number of atomics.");

    std::vector<Event<bool>> boolEvents;
    std::vector<Event<std::array<size_t, 2>>> intervalEvents;
    size_t length = timeSteps.size();
    boolEvents.reserve(length);
    intervalEvents.reserve(length);
    size_t initialTimeStep{timeSteps.at(0)};



    for (size_t idx{0}; idx < length; ++idx) {
        event_map_t<bool> eventBool;
        event_map_t<std::array<size_t , 2>> eventInterval;

        eventBool.reserve(atomicsAndFunctions.size());
        eventInterval.reserve(dynamicInterval.size());

        for (auto const &[atomic, valueFetcher] : atomicsAndFunctions) {
            auto valueFetcherBound = std::bind(valueFetcher, idx);
            eventBool.emplace(atomic, LazyFunction<bool>(std::move(valueFetcherBound)));
        }
        for (auto const &atomic : atomics) {
            // Check if atomic exists in vector at current position
            bool atomicValue = trueAtomics[idx].end() != std::find(trueAtomics[idx].begin(), trueAtomics[idx].end(), atomic);
            eventBool[atomic] = LazyFunction(atomicValue);
        }


        for (const auto &[atomic, valueFetcher] : dynamicInterval) {
            auto valueFetcherBound = std::bind(valueFetcher, idx);
            eventInterval.emplace(atomic, LazyFunction<std::array<size_t , 2>>(std::move(valueFetcherBound)));
        }

        boolEvents.emplace_back(std::move(eventBool), idx, idx == length + initialTimeStep - 1);
        intervalEvents.emplace_back(std::move(eventInterval), idx, idx == length + initialTimeStep - 1);
    }



    return {boolEvents, intervalEvents};
}