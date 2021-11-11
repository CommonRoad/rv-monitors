//
// Created by Evald Nexhipi on 23.10.21.
//
#include "trace_generator.h"
#include "monitors/trace/event.h"
#include <iostream>
Trace TraceGenerator ::GenerateTrace(const int length) {
    std::vector<Event> trace;
    double coin;

    for (int i = 0; i < length; ++i) {
        coin = rreal(0, m_sumWeightsAtomic + m_sumWeightsPredicate);
        double w = 0.0;
        std::map<std::string, lazy_bool> eventMap;
        for (auto &iter : m_atoms) {
            w += iter.m_weight;
            if (w >= coin) {
                /* Atom to be chosen to be true */
                std::shared_ptr<Atomic> a = iter.m_atomic;
                eventMap.insert(std::pair<std::string, lazy_bool>(a->to_string(), true));
            } else {
                /* Atom to be chosen to be false */
                std::shared_ptr<Atomic> a = iter.m_atomic;
                eventMap.insert(std::pair<std::string, lazy_bool>(a->to_string(), false));
            }
        }
        for (auto &iter : m_predicates) {
            w += iter.m_weight;

            if (w >= coin) {
                /* Predicate to be chosen to be true */
                std::shared_ptr<Predicate> p = iter.m_predicate;
                eventMap.insert(std::pair<std::string, lazy_bool>(p->to_string(), true));
            } else {
                /* Predicate to be chosen to be false */
                std::shared_ptr<Predicate> p = iter.m_predicate;
                eventMap.insert(std::pair<std::string, lazy_bool>(p->to_string(), false));
            }
        }
        bool terminal = false;
        if (i == length - 1)
            terminal = true;
        Event e(eventMap, i, terminal);
        trace.push_back(e);
    }
    return Trace(trace);
}
