#include "monitors/trace/event_generator.h"
#include <algorithm>

EventGenerator::EventGenerator(size_t length) : event(length, false), numberOfTrue(0) {}

const std::vector<bool> &EventGenerator::getNext() {

    if (not std::next_permutation(event.begin(), event.end())) {
        if (numberOfTrue < event.size()) {
            event[event.size() - numberOfTrue - 1] = true;
        }
        ++numberOfTrue;
    }

    return event;
}

bool EventGenerator::hasNext() { return numberOfTrue <= event.size() and !event.empty(); }

// TODO change to real Event
const std::vector<bool> &EventGenerator::getEventVector() const { return event; }
