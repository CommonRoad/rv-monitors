//
// Created by Evald Nexhipi on 28.10.21.
//

#ifndef RVMONITORS_COMPUTATION_TIME_GENERATOR_H
#define RVMONITORS_COMPUTATION_TIME_GENERATOR_H
#include <iostream>
#include <memory>
#include <thread>
#include <monitors/expression_tree/expression.h>
#include "monitors/expression_tree/atomic/atomic.h"
#include "monitors/expression_tree/atomic/predicate.h"

#include <vector>
class ComputationTimeGenerator
{
  public:
    ComputationTimeGenerator(void) : m_sumWeightsAtomic(0.0), m_sumWeightsPredicate(0.0)
    {

    }

    virtual ~ComputationTimeGenerator(void) {

    }

    static void rseed(void)
    {
        srand (time(NULL));
    }

    static  double rreal(void)
    {
        return ((double) rand()) / ((double) RAND_MAX);
    }

    static double rreal(const double min, const double max)
    {
        return min + (max - min) * rreal();
    }

    static inline long rint(const long min, const long max)
    {
        const long x = min + (long)  rreal(0, max - min + 1);
        return x > max ? max : x;
    }

    virtual void AddAtomsDistribution(std::vector<std::shared_ptr<Atomic>> atoms, const std::vector<double> weights)
    {
        //std::vector<std::shared_ptr<AtomicInfo>> atomsInfo;
        std::vector<AtomicInfo> atomsInfo;
        double total_weights = 0;
        for (int i=0; i < atoms.size(); ++i)
        {
            AtomicInfo ai;
            ai.m_atomic = atoms[i];
            ai.m_weight = weights[i];
            atomsInfo.push_back(ai);
            total_weights+=weights[i];
            //std::cout<<ai->m_atomic<<std::endl;
        }

        m_atoms.push_back(atomsInfo);
        m_sumWeightsAtomic.push_back(total_weights);
    }


    virtual void AddPredicateDistribution(std::vector<std::shared_ptr<Predicate>> predicates, const std::vector<double> weights)
    {
        //std::vector<std::shared_ptr<PredicateInfo>> predicatesInfo;
        std::vector<PredicateInfo> predicatesInfo;
        double total_weights = 0;
        for (int i=0; i < predicates.size(); ++i)
        {
            PredicateInfo pi;
            pi.m_predicate = predicates[i];
            pi.m_weight = weights[i];
            predicatesInfo.push_back(pi);
            total_weights+=weights[i];
        }

        m_predicates.push_back(predicatesInfo);
        m_sumWeightsPredicate.push_back(total_weights);
    }

    virtual std::shared_ptr<Expression> generateAtomOrPredicate(int delay_index);

    virtual void generateAtomsAndPredicates (int index_of_delay, int nrPreds, std::vector< std::shared_ptr<Expression> > & preds);

    void createPredicatesOnDelays (std::vector<int> info,  std::vector< std::vector<std::shared_ptr<Expression>>*> &preds);

  protected:
    struct AtomicInfo
    {
        std::shared_ptr<Atomic> m_atomic;
        double m_weight;
    };

    struct PredicateInfo
    {
        std::shared_ptr<Predicate> m_predicate;
        double m_weight;
    };

    std::vector<double> m_sumWeightsAtomic;
    std::vector<double> m_sumWeightsPredicate;

    //std::vector<std::vector<std::shared_ptr<AtomicInfo>>> m_atoms;
    std::vector<std::vector<AtomicInfo>> m_atoms;
    //std::vector<std::vector<std::shared_ptr<PredicateInfo>>> m_predicates;
    std::vector<std::vector<PredicateInfo>> m_predicates;
};


#endif // RVMONITORS_COMPUTATION_TIME_GENERATOR_H
