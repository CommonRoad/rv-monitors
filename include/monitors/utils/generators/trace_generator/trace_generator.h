#pragma once

#include <utility>

#include "monitors/trace/trace.h"
/**
 * Class to pseudo-randomly generate a trace
 */
class TraceGenerator {
  public:
    /**
     * Initializes the sequence of pseudo-random numbers generated when the rand function is called.
     */
    static void rSeed() { srand(static_cast<unsigned int>(time(nullptr))); }

    /**
     * Generates a real number
     * @return returns a pseudo-random real number
     */
    static double rReal() { return ((double)rand()) / ((double)RAND_MAX); }

    /**
     * Generates a real number within an interval
     * @param min minimum number to be generated
     * @param max maximum number to be generated
     * @return a real number within (min, max) interval
     */
    static double rReal(const double min, const double max) { return min + (max - min) * rReal(); }

    /**
     * Generates an integer within an interval
     * @param min minimum number to be generated
     * @param max maximum number to be generated
     * @return an integer within (min, max) interval
     */
    static inline size_t rInt(const size_t min, const size_t max) {
        const size_t x = min + (size_t)rReal(0, static_cast<double>(max - min + 1));
        return x > max ? max : x;
    }

    /**
     * Adds atoms to the Formula Generator
     * @param a Atom to be added
     * @param w Associated weight (larger than 0)
     */
    virtual void addAtomic(std::shared_ptr<Atomic> a, const double w) {
        AtomicInfo ai;
        ai.m_atomic = std::move(a);
        ai.m_weight = w;
        m_atoms.push_back(ai);
        m_sumWeightsAtomic += w;
    }

    /**
     * Adds predicates to the Formula Generator
     * @param p Predicate to be added
     * @param w Associated weight (larger than 0)
     */
    virtual void addPredicate(std::shared_ptr<Predicate> p, const double w) {
        PredicateInfo pi;
        pi.m_predicate = std::move(p);
        pi.m_weight = w;
        m_predicates.push_back(pi);
        m_sumWeightsPredicate += w;
    }

    /**
     * Trace to be pseudo-randomly generated
     * @param length of the trace
     * @return returns a pseudo-random Trace
     */
    std::shared_ptr<Trace> generateTrace(size_t length);

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
     * @var m_sumWeightsAtomic total sum of atoms weights
     */
    double m_sumWeightsAtomic{0.0};
    /**
     * @var m_sumWeightsPredicate total sum of predicate weights
     */
    double m_sumWeightsPredicate{0.0};

    /**
     * @var m_atoms vector containing atoms info
     */
    std::vector<AtomicInfo> m_atoms;
    /**
     * @var m_predicates vector containing predicates info
     */
    std::vector<PredicateInfo> m_predicates;
};
