#include "monitors/utils/generators/trace_generator/trace_generator.h"
#include "monitors/expression_tree/atomic/predicate.h"
#include "monitors/trace/event.h"
#include <iostream>
std::shared_ptr<Trace> TraceGenerator ::generateTrace(const size_t length) {
    std::vector<Event> trace;
    double coin;

    for (size_t i = 0; i < length; ++i) {
        coin = rReal(0, m_sumWeightsAtomic + m_sumWeightsPredicate);
        double weight = 0.0;
        event_map_t eventMap;
        for (auto &iter : m_atoms) {
            weight += iter.m_weight;
            if (weight >= coin) {
                /* Atom to be chosen to be true */
                std::shared_ptr<Atomic> atomic = iter.m_atomic;
                // toString()
                eventMap.insert(std::pair<std::string, EventValue<bool>>(atomic->to_string(), true));
            } else {
                /* Atom to be chosen to be false */
                std::shared_ptr<Atomic> atomic = iter.m_atomic;
                eventMap.insert(std::pair<std::string, EventValue<bool>>(atomic->to_string(), false));
            }
        }
        for (auto &iter : m_predicates) {
            weight += iter.m_weight;

            if (weight >= coin) {
                /* Predicate to be chosen to be true */
                std::shared_ptr<Predicate> predicate = iter.m_predicate;
                eventMap.insert(std::pair<std::string, EventValue<bool>>(predicate->to_string(), true));
            } else {
                /* Predicate to be chosen to be false */
                std::shared_ptr<Predicate> predicate = iter.m_predicate;
                eventMap.insert(std::pair<std::string, EventValue<bool>>(predicate->to_string(), false));
            }
        }
        bool terminal = false;
        if (i == length - 1)
            terminal = true;
        Event eve(eventMap, i, terminal);
        trace.push_back(eve);
    }
    return std::make_shared<Trace>(Trace(trace));
}
