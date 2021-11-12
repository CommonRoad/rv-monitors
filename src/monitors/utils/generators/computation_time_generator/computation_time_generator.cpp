//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "computation_time_generator.h"
#include <chrono>

std::shared_ptr<Expression> ComputationTimeGenerator::generateAtomOrPredicate(size_t delay_index) {

    if (!m_predicates.empty() && !m_atoms.empty()){
        if (!m_predicates[delay_index].empty() && !m_atoms[delay_index].empty()){
            double coin;
            coin = rreal(0, m_sumWeightsAtomic[delay_index] + m_sumWeightsPredicate[delay_index]);
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
        }

        if (!m_predicates[delay_index].empty() && m_atoms[delay_index].empty()){
            double coin;
            coin = rreal(0, m_sumWeightsPredicate[delay_index]);
            double w = 0.0;
            for (auto &iter : m_predicates[delay_index]) {
                w += iter.m_weight;
                if (w >= coin)
                    return iter.m_predicate;
            }
        }

        if (!m_atoms[delay_index].empty() && m_predicates[delay_index].empty()){
            double coin;
            coin = rreal(0, m_sumWeightsAtomic[delay_index]);
            double w = 0.0;
            for (auto &iter : m_atoms[delay_index]) {
                w += iter.m_weight;
                if (w >= coin)
                    return iter.m_atomic;
            }
        }
    }


    if (!m_predicates.empty() && m_atoms.empty()){
        if (!m_predicates[delay_index].empty()){
            double coin;
            coin = rreal(0, m_sumWeightsPredicate[delay_index]);
            double w = 0.0;
            for (auto &iter : m_predicates[delay_index]) {
                w += iter.m_weight;
                if (w >= coin)
                    return iter.m_predicate;
            }
        }
    }

    if (!m_atoms.empty() && m_predicates.empty()){
        if (!m_atoms[delay_index].empty() && m_predicates[delay_index].empty()){
            double coin;
            coin = rreal(0, m_sumWeightsAtomic[delay_index]);
            double w = 0.0;
            for (auto &iter : m_atoms[delay_index]) {
                w += iter.m_weight;
                if (w >= coin)
                    return iter.m_atomic;
            }
        }
    }

    return nullptr;
}

void ComputationTimeGenerator::generateAtomsAndPredicates(size_t index_of_delay, size_t nrPreds,
                                                          std::vector<std::shared_ptr<Expression>> &preds) {
    for (int i = 0; i < nrPreds; ++i)
        preds.push_back(generateAtomOrPredicate(index_of_delay));
}

void ComputationTimeGenerator::createPredicatesOnDelays(
    std::vector<size_t> info, std::vector<std::vector<std::shared_ptr<Expression>> *> &allPreds) {
    size_t next_delay = 0;
    for (size_t i = 0; i < info.size(); i += 3) {
        size_t timeDelay = info[i];
        size_t minNrPreds = info[i + 1];
        size_t maxNrPreds = info[i + 2];
        size_t nrPreds = rint(minNrPreds, maxNrPreds);

        auto *preds = new std::vector<std::shared_ptr<Expression>>();
        // std::cout << "Delay " << next_delay + 1 << " started" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(timeDelay));
        generateAtomsAndPredicates(next_delay, nrPreds, *preds);

        // std::cout << "Predicates Generated: ";
        // for (auto &p : *preds)
           // std::cout << p->to_string() << " ";
        //std::cout << std::endl;

        next_delay++;
        allPreds.push_back(preds);
    }
}
