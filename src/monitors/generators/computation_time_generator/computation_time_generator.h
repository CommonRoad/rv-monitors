//
// Created by Evald Nexhipi on 28.10.21.
//

#ifndef RVMONITORS_COMPUTATION_TIME_GENERATOR_H
#define RVMONITORS_COMPUTATION_TIME_GENERATOR_H
#include "monitors/expression_tree/atomic/atomic.h"
#include "monitors/expression_tree/atomic/predicate.h"
#include <iostream>
#include <memory>
#include <monitors/expression_tree/expression.h>
#include <thread>

#include <vector>
class ComputationTimeGenerator {
  public:
    ComputationTimeGenerator(void) : m_sumWeightsAtomic(0.0), m_sumWeightsPredicate(0.0) {}

    virtual ~ComputationTimeGenerator(void) {}

    /**
     * Initializes the sequence of pseudo-random numbers generated when the rand function is called.
     */
    static void rseed(void) { srand(time(NULL)); }

    static double rreal(void) { return ((double)rand()) / ((double)RAND_MAX); }

    static double rreal(const double min, const double max) { return min + (max - min) * rreal(); }

    static inline long rint(const long min, const long max) {
        const long x = min + (long)rreal(0, max - min + 1);
        return x > max ? max : x;
    }

    /**
     * Adds atoms distribution for each delay
     * @param atoms atoms to be added
     * @param weights associated weights
     */
    virtual void AddAtomsDistribution(std::vector<std::shared_ptr<Atomic>> atoms, const std::vector<double> weights) {
        std::vector<AtomicInfo> atomsInfo;
        double total_weights = 0;
        for (int i = 0; i < atoms.size(); ++i) {
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
     * @param weights associated weights
     */
    virtual void AddPredicateDistribution(std::vector<std::shared_ptr<Predicate>> predicates,
                                          const std::vector<double> weights) {
        std::vector<PredicateInfo> predicatesInfo;
        double total_weights = 0;
        for (int i = 0; i < predicates.size(); ++i) {
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
    virtual std::shared_ptr<Expression> generateAtomOrPredicate(int delay_index);

    /**
     * Generate a specific number of Atoms or Predicates
     * @param index_of_delay index of current delay / duration
     * @param nrPreds number of Atoms or Predicates to be generated
     * @param preds generated Atoms / Predicates
     */
    virtual void generateAtomsAndPredicates(int index_of_delay, int nrPreds,
                                            std::vector<std::shared_ptr<Expression>> &preds);

    /**
     * Atoms and Predicates generated for a delay / duration
     * @param info vector of form {duration_in_miliseconds, min_number_of_predicates, max_number_of_predicates, ...}
     * @param preds generated predicates
     */
    void createPredicatesOnDelays(std::vector<int> info,
                                  std::vector<std::vector<std::shared_ptr<Expression>> *> &preds);

  protected:
    struct AtomicInfo {
        std::shared_ptr<Atomic> m_atomic;
        double m_weight;
    };

    struct PredicateInfo {
        std::shared_ptr<Predicate> m_predicate;
        double m_weight;
    };

    std::vector<double> m_sumWeightsAtomic;
    std::vector<double> m_sumWeightsPredicate;

    std::vector<std::vector<AtomicInfo>> m_atoms;
    std::vector<std::vector<PredicateInfo>> m_predicates;
};

#endif // RVMONITORS_COMPUTATION_TIME_GENERATOR_H