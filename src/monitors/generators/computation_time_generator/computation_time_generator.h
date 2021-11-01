//
// Created by Evald Nexhipi on 28.10.21.
//

#ifndef RVMONITORS_COMPUTATION_TIME_GENERATOR_H
#define RVMONITORS_COMPUTATION_TIME_GENERATOR_H
#include <iostream>
#include <memory>
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

    virtual void AddAtomic(std::shared_ptr<Atomic> a, const double w)
    {
        AtomicInfo ai;
        ai.m_atomic = a;
        ai.m_weight = w;
        m_atoms.push_back(ai);
        m_sumWeightsAtomic += w;
    }


    virtual void AddPredicate(std::shared_ptr<Predicate> p, const double w)
    {
        PredicateInfo pi;
        pi.m_predicate = p;
        pi.m_weight = w;
        m_predicates.push_back(pi);
        m_sumWeightsPredicate += w;
    }

    virtual std::shared_ptr<Expression> generateAtomOrPredicate();

    virtual void generateAtomsAndPredicates (int nrPreds, std::vector< std::shared_ptr<Expression> > & preds);

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

    double m_sumWeightsAtomic;
    double m_sumWeightsPredicate;

    std::vector<AtomicInfo> m_atoms;
    std::vector<PredicateInfo> m_predicates;
};


#endif // RVMONITORS_COMPUTATION_TIME_GENERATOR_H
