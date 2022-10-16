#include "event.h"

bool Event::getValue(const std::string &atomic) const { return eventMap.at(atomic).get(); }

event_map_t Event::getEventMap() const { return this->eventMap; }

event_map_t &Event::getEventMapRef() { return this->eventMap; }

bool Event::getValue(const Atomic &atomic) const { return getValue(atomic.to_string()); }

bool Event::getValue(const Predicate &predicate) const { return getValue(predicate.to_string()); }

bool Event::isTerminal() const { return terminal; }

void Event::setEventMap(const event_map_t &newEventMap) { Event::eventMap = newEventMap; }

void Event::setTerminal(bool isTerminal) { Event::terminal = isTerminal; }

std::string Event::to_string() const {
    std::stringstream res;
    res << "{";
    for (const auto &[key, value] : eventMap) {
        res << key << ": " << value << "; ";
    }
    res << timeStep << "}";
    return res.str();
}

size_t Event::getTimeStep() const { return timeStep; }

void Event::setTimeStep(size_t newTimeStep) { Event::timeStep = newTimeStep; }

bool Event::hasAtomic(const Atomic &atomic) const {
    return eventMap.find(atomic.to_string()) != eventMap.end();
}

bool Event::hasPredicate(const std::string &proposition) const {
    return eventMap.find(proposition) != eventMap.end();
}
