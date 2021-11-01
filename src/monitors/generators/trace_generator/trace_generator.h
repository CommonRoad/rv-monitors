//
// Created by Evald Nexhipi on 23.10.21.
//

#ifndef RVMONITORS_TRACE_GENERATOR_H
#define RVMONITORS_TRACE_GENERATOR_H

#include "monitors/trace/trace.h"
class TraceGenerator
{
  public:
    TraceGenerator(void) : m_sumWeightsAtomic(0.0), m_sumWeightsPredicate(0.0)
    {

    }

    virtual ~TraceGenerator(void) {

    }

    /**
     * Initializes the sequence of pseudo-random numbers generated when the rand function is called.
     */
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

    /**
     * Adds atoms to the Formula Generator
     * @param a Atom to be added
     * @param w Associated weight
     */
    virtual void AddAtomic(std::shared_ptr<Atomic> a, const double w)
    {
        AtomicInfo ai;
        ai.m_atomic = a;
        ai.m_weight = w;
        m_atoms.push_back(ai);
        m_sumWeightsAtomic += w;
    }

    /**
     * Adds predicates to the Formula Generator
     * @param p Predicate to be added
     * @param w Associated weight
     */
    virtual void AddPredicate(std::shared_ptr<Predicate> p, const double w)
    {
        PredicateInfo pi;
        pi.m_predicate = p;
        pi.m_weight = w;
        m_predicates.push_back(pi);
        m_sumWeightsPredicate += w;
    }

    /**
     * Trace to be pseudo-randomly generated
     * @param length of the trace
     * @return returns a pseudo-random Trace
     */
    virtual Trace GenerateTrace(const int length);

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


    int m_nrOperators;
    double m_sumWeightsAtomic;
    double m_sumWeightsPredicate;

    std::vector<AtomicInfo> m_atoms;
    std::vector<PredicateInfo> m_predicates;
};

#endif // RVMONITORS_TRACE_GENERATOR_H