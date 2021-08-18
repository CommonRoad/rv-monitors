#include "multi_transition.h"
#include "monitors/utils/utils.h"

MultiTransition::MultiTransition(std::map<Formula, size_t> transitions) : transitions(std::move(transitions)) {}

std::map<Formula, size_t> &MultiTransition::getTransitions() { return transitions; }

const std::map<Formula, size_t> &MultiTransition::getTransitions() const { return transitions; }

std::string MultiTransition::to_string() const {
    std::stringstream ret;
    for (const auto &[formula, state] : transitions) {
        ret << formula.to_string() + " -> ";
        ret << state << std::endl;
    }
    return ret.str();
}

size_t MultiTransition::evaluate(std::map<std::string, lazy_bool> &eventMap) const {
    for (const auto &[formula, state] : transitions) {
        if (Utils::evaluateBooleanExpression(formula.getExpr(), eventMap)) {
            return state;
        }
    }
    throw std::runtime_error("MultiTransition::evaluate() Event could not be mapped to any state.");
}

std::map<std::string, size_t> MultiTransition::getAtomicAndPredicateFrequencies() const {
    std::map<std::string, size_t> frequencies;
    for (const auto &[formula, state] : transitions) {
        // Formula of transition
        auto predicates = Utils::getAtomicsAndPredicates(formula);
        for (const auto &predicate : predicates) {
            // Adds key if it is not yet in map
            frequencies[predicate]++;
        }
    }
    return frequencies;
}

// TODO use Predicates.h for costs
std::multimap<double, std::shared_ptr<Atomic>> MultiTransition::getAtomicAndPredicateWeight() const {
    std::map<std::shared_ptr<Atomic>, double> precedences;
    auto numberOfTransitions = transitions.size();
    auto frequencies = getAtomicAndPredicateFrequencies();
    for (const auto &[predicateString, freq] : frequencies) {
        auto predicate = std::make_shared<Atomic>(predicateString);
        precedences[predicate] = 1 / (double(freq) / static_cast<double>(numberOfTransitions));
    }
    return createWeightMultimap(precedences);
}

std::multimap<double, std::shared_ptr<Atomic>>
MultiTransition::createWeightMultimap(const std::map<std::shared_ptr<Atomic>, double> &weights) const {
    std::vector<std::shared_ptr<Atomic>> atomics;
    // Is now ordered by weights
    return Utils::flip_map(weights);
}
