#include "monitors/trace/event.h"
#include "monitors/expression_tree/atomic/predicate.h"
#include <utility>

bool Event::getValue(const std::string &atomic) const { return eventMap.at(atomic).get(); }

event_map_t Event::getEventMap() const { return this->eventMap; }

event_map_t &Event::getEventMapRef() { return this->eventMap; }

bool Event::getValue(const Atomic &atomic) const { return getValue(atomic.to_string()); }

bool Event::getValue(const Predicate &predicate) const { return getValue(predicate.to_string()); }

bool Event::isTerminal() const { return terminal; }

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

Verdict Event::getVerdict(const std::string &name) { return {timeStep, eventMap.at(name).get()}; }

void Event::addEventValueFunction(const std::string &name, EventValue<bool> func) { eventMap[name] = std::move(func); }
