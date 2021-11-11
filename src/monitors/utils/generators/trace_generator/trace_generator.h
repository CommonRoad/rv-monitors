//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//

#pragma once

#include "monitors/trace/trace.h"
class TraceGenerator {
  public:
    TraceGenerator(void) : m_sumWeightsAtomic(0.0), m_sumWeightsPredicate(0.0) {}

    virtual ~TraceGenerator(void) {}

    /**
     * Initializes the sequence of pseudo-random numbers generated when the rand function is called.
     */
    static void rseed(void) { srand(time(NULL)); }

    /**
     * Generates a real number
     * @return returns a pseudo-random real number
     */
    static double rreal(void) { return ((double)rand()) / ((double)RAND_MAX); }

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
        const size_t x = min + (size_t)rreal(0, max - min + 1);
        return x > max ? max : x;
    }

    /**
     * Adds atoms to the Formula Generator
     * @param a Atom to be added
     * @param w Associated weight (larger than 0)
     */
    virtual void addAtomic(std::shared_ptr<Atomic> a, const double w) {
        AtomicInfo ai;
        ai.m_atomic = a;
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
    Trace generateTrace(const size_t length);

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
    double m_sumWeightsAtomic;
    /**
     * @var m_sumWeightsPredicate total sum of predicate weights
     */
    double m_sumWeightsPredicate;

    /**
     * @var m_atoms vector containing atoms info
     */
    std::vector<AtomicInfo> m_atoms;
    /**
     * @var m_predicates vector containing predicates info
     */
    std::vector<PredicateInfo> m_predicates;
};