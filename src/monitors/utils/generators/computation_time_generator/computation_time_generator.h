//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//

#pragma once
#include "monitors/expression_tree/atomic/atomic.h"
#include "monitors/expression_tree/atomic/predicate.h"
#include <iostream>
#include <memory>
#include <monitors/expression_tree/expression.h>
#include <thread>

#include <vector>
class ComputationTimeGenerator {
  public:
    ComputationTimeGenerator() : m_sumWeightsAtomic(0.0), m_sumWeightsPredicate(0.0) {}

    /**
     * Initializes the sequence of pseudo-random numbers generated when the rand function is called.
     */
    static void rseed() { srand(time(NULL)); }

    /**
     * Generates a real number
     * @return returns a pseudo-random real number
     */
    static double rreal() { return ((double)rand()) / ((double)RAND_MAX); }

    /**
     * Generates a real number within an interval
     * @param min minimum number to be generated
     * @param max maximum number to be generated
     * @return a real number within (min, max) interval
     */
    static double rreal(const double min, const double max) { return min + (max - min) * rreal(); }

    /**
     * Generates an integer within an interval
     * @param min minimum number to be generated
     * @param max maximum number to be generated
     * @return an integer within (min, max) interval
     */
    static inline size_t rint(const size_t min, const size_t max) {
        const size_t x = min + (size_t)rreal(0, static_cast<double>(max - min + 1));
        return x > max ? max : x;
    }

    /**
     * Adds atoms distribution for each delay
     * @param atoms atoms to be added
     * @param weights associated weights (larger than 0)
     */
    virtual void addAtomsDistribution(std::vector<std::shared_ptr<Atomic>> atoms, const std::vector<double> &weights) {
        std::vector<AtomicInfo> atomsInfo;
        double total_weights = 0;
        for (size_t i = 0; i < atoms.size(); ++i) {
            AtomicInfo ai;
            ai.m_atomic = atoms[i];
            ai.m_weight = weights[i];
            atomsInfo.push_back(ai);
            total_weights += weights[i];
        }

        m_atoms.push_back(atomsInfo);
        m_sumWeightsAtomic.push_back(total_weights);
    }

    /**
     * Adds predicates distribution for each delay
     * @param predicates predicates to be added
     * @param weights associated weights (larger than 0)
     */
    virtual void addPredicateDistribution(std::vector<std::shared_ptr<Predicate>> predicates,
                                          const std::vector<double> &weights) {
        std::vector<PredicateInfo> predicatesInfo;
        double total_weights = 0;
        for (size_t i = 0; i < predicates.size(); ++i) {
            PredicateInfo pi;
            pi.m_predicate = predicates[i];
            pi.m_weight = weights[i];
            predicatesInfo.push_back(pi);
            total_weights += weights[i];
        }

        m_predicates.push_back(predicatesInfo);
        m_sumWeightsPredicate.push_back(total_weights);
    }

    /**
     * Generates an atom or predicate for a specific delay / duration
     * @param delay_index index of current delay / duration
     * @return an atom or predicate
     */
    std::shared_ptr<Expression> generateAtomOrPredicate(size_t delay_index);

    /**
     * Generate a specific number of Atoms or Predicates
     * @param index_of_delay index of current delay / duration
     * @param nrPreds number of Atoms or Predicates to be generated
     * @param preds generated Atoms / Predicates
     */
    void generateAtomsAndPredicates(size_t index_of_delay, size_t nrPreds,
                                    std::vector<std::shared_ptr<Expression>> &preds);

    /**
     * Atoms and Predicates generated for a delay / duration in [ms] (larger than 0)
     * @param info vector of form {duration_in_miliseconds, min_number_of_predicates, max_number_of_predicates, ...}
     * @param preds generated predicates
     */
    void createPredicatesOnDelays(std::vector<size_t> info,
                                  std::vector<std::vector<std::shared_ptr<Expression>> *> &preds);

  protected:
    /**
     * Structure to store the association of a weight to an Atom
     * @var m_atomic atoms to be stored in the structure
     * @var m_weight respective weights for the atoms
     */
    struct AtomicInfo {
        std::shared_ptr<Atomic> m_atomic;
        double m_weight;
    };

    /**
     * Structure to store the association of a weight to a Predicate
     * @var m_atomic atoms to be stored in the structure
     * @var m_weight respective weights for the predicates
     */
    struct PredicateInfo {
        std::shared_ptr<Predicate> m_predicate;
        double m_weight;
    };

    /**
     * @var m_sumWeightsAtomic vector containing atoms total weights on each delay
     */
    std::vector<double> m_sumWeightsAtomic;
    /**
     * @var m_sumWeightsPredicate vector containing predicates total weights on each delay
     */
    std::vector<double> m_sumWeightsPredicate;

    /**
     * @var m_atoms vector containing atoms info
     */
    std::vector<std::vector<AtomicInfo>> m_atoms;
    /**
     * @var m_predicates vector containing predicates info
     */
    std::vector<std::vector<PredicateInfo>> m_predicates;
};