#include "event.h"

bool Event::getValue(const std::string &atomic) { return eventMap.at(atomic).get(); }

std::map<std::string, lazy_bool> Event::getEventMap() const { return this->eventMap; }

std::map<std::string, lazy_bool> &Event::getEventMapRef() { return this->eventMap; }

bool Event::getValue(const Atomic &atomic) { return getValue(atomic.to_string()); }

bool Event::getValue(const Predicate &predicate) { return getValue(predicate.to_string()); }

bool Event::isTerminal() const { return terminal; }

void Event::setEventMap(const std::map<std::string, lazy_bool> &newEventMap) { Event::eventMap = newEventMap; }

void Event::setTerminal(bool isTerminal) { Event::terminal = isTerminal; }

std::string Event::to_string() const {
    std::stringstream res;
    res << "{";
    for (const auto &[key, value] : eventMap) {
        res << key << ": " << value << "; ";
    }
    res << terminal << "}";
    return res.str();
}

size_t Event::getTimeStep() const { return timeStep; }

void Event::setTimeStep(size_t newTimeStep) { Event::timeStep = newTimeStep; }
