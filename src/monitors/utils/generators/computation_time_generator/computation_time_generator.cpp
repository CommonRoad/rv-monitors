#include "monitors/utils/generators/computation_time_generator/computation_time_generator.h"
#include "monitors/expression_tree/atomic/atomic.h"
#include "monitors/expression_tree/atomic/predicate.h"
#include <chrono>

std::shared_ptr<Expression> ComputationTimeGenerator::generateAtom(size_t delay_index) {
    double coin = rReal(0, m_sumWeightsAtomic[delay_index]);
    double w = 0.0;
    for (auto &iter : m_atoms[delay_index]) {
        w += iter.m_weight;
        if (w >= coin)
            return iter.m_atomic;
    }
    return nullptr;
}

std::shared_ptr<Expression> ComputationTimeGenerator::generatePredicate(size_t delay_index) {
    double coin = rReal(0, m_sumWeightsPredicate[delay_index]);
    double w = 0.0;
    for (auto &iter : m_predicates[delay_index]) {
        w += iter.m_weight;
        if (w >= coin)
            return iter.m_predicate;
    }
    return nullptr;
}
std::shared_ptr<Expression> ComputationTimeGenerator::generateBoth(size_t delay_index) {
    double coin = rReal(0, m_sumWeightsAtomic[delay_index] + m_sumWeightsPredicate[delay_index]);
    double w = 0.0;
    for (auto &iter : m_atoms[delay_index]) {
        w += iter.m_weight;
        if (w >= coin)
            return iter.m_atomic;
    }
    for (auto &iter : m_predicates[delay_index]) {
        w += iter.m_weight;
        if (w >= coin)
            return iter.m_predicate;
    }
    return nullptr;
}

std::shared_ptr<Expression> ComputationTimeGenerator::generateAtomOrPredicate(size_t delay_index) {

    if (!m_predicates.empty() && !m_atoms.empty()) {
        if (!m_predicates[delay_index].empty() && !m_atoms[delay_index].empty())
            return generateBoth(delay_index);

        if (!m_predicates[delay_index].empty() && m_atoms[delay_index].empty())
            return generatePredicate(delay_index);

        if (!m_atoms[delay_index].empty() && m_predicates[delay_index].empty())
            return generateAtom(delay_index);
    }

    if (!m_predicates.empty() && m_atoms.empty())
        return generatePredicate(delay_index);

    if (!m_atoms.empty() && m_predicates.empty())
        return generateAtom(delay_index);

    return nullptr;
}

void ComputationTimeGenerator::generateAtomsAndPredicates(size_t index_of_delay, size_t nrPredicates,
                                                          std::vector<std::shared_ptr<Expression>> &predicates) {
    for (size_t i = 0; i < nrPredicates; ++i)
        predicates.push_back(generateAtomOrPredicate(index_of_delay));
}

void ComputationTimeGenerator::createPredicatesOnDelays(
    std::vector<size_t> info, std::vector<std::vector<std::shared_ptr<Expression>> *> &predicates) {
    size_t next_delay = 0;
    for (size_t i = 0; i < info.size(); i += 3) {
        size_t timeDelay = info[i];
        size_t minNrPredicates = info[i + 1];
        size_t maxNrPredicates = info[i + 2];
        size_t nrPredicates = rInt(minNrPredicates, maxNrPredicates);

        auto *predicatesTmp = new std::vector<std::shared_ptr<Expression>>();

        std::this_thread::sleep_for(std::chrono::milliseconds(timeDelay));
        generateAtomsAndPredicates(next_delay, nrPredicates, *predicatesTmp);

        next_delay++;
        predicates.push_back(predicatesTmp);
    }
}