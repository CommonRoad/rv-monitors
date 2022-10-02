#include "event.h"
#include "monitors/expression_tree/atomic/atomic.h"
#include "monitors/expression_tree/atomic/predicate.h"



template <class T>
event_map_t<T> Event<T>::getEventMap() const { return this->eventMap; }

template<>
event_map_t<bool> &Event<bool>::getEventMapRef() { return this->eventMap; }

template <class T>
T Event<T>::getValue(const std::string &name) const { return eventMap.at(name).get(); }

template<> bool Event<bool>::getValue(const Atomic &atomic) const { return getValue(atomic.to_string()); }

template <>
bool Event<bool>::getValue(const Predicate &predicate) const { return getValue(predicate.to_string()); }

template <class T>
bool Event<T>::isTerminal() const { return terminal; }

template <class T>
void Event<T>::setTerminal(bool isTerminal) { Event::terminal = isTerminal; }

template <>
std::string Event<bool>::to_string() const {
    std::stringstream res;
    res << "{";
    for (const auto &[key, value] : eventMap) {
        res << key << ": " << value.to_string() << "; ";
    }
    res << timeStep << "}";
    return res.str();
}

template <class T>
size_t Event<T>::getTimeStep() const { return timeStep; }

template class Event<bool>; //required otherwise compiler won't instantiate template
template class Event<std::array<size_t, 2>>; //required otherwise compiler won't instantiate template

