#include "event.h"

bool Event::getValue(const std::string &atomic) { return eventMap.at(atomic).get(); }

std::map<std::string, lazy_bool> Event::getEventMap() const { return this->eventMap; }

std::map<std::string, lazy_bool> &Event::getEventMapRef() { return this->eventMap; }

bool Event::getValue(const Atomic &atomic) { return getValue(atomic.to_string()); }

bool Event::isTerminal() const { return terminal; }

void Event::setEventMap(const std::map<std::string, lazy_bool> &eventMap) { Event::eventMap = eventMap; }

void Event::setTerminal(bool terminal) { Event::terminal = terminal; }

std::string Event::to_string() const {
    std::stringstream res;
    res << "{";
    for (auto &[key, value] : eventMap) {
        res << key << ": " << value << "; ";
    }
    res << terminal << "}";
    return res.str();
}

int Event::getTimeStep() const { return timeStep; }

void Event::setTimeStep(int timeStep) { Event::timeStep = timeStep; }
