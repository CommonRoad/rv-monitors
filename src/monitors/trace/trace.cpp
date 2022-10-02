//
// Created by sebastian on 30.09.22.
//

#include "trace.h"

Event<bool> &Trace::getBooleanEvent(size_t index) { return booleanElements.at(index); }

Event<std::array<size_t, 2>> &Trace::getIntervalEvent(size_t index) { return intervalElements.at(index); }

size_t Trace::getLength() { return booleanElements.size(); }

const std::vector<Event<bool>> &Trace::getBooleanTrace() const { return booleanElements; }

std::string Trace::to_string() const {
    std::stringstream ret;
    for (const Event<bool> &event : booleanElements) {
        ret << event.to_string() << std::endl;
    }
    return ret.str();
}


bool Trace::isEquidistant() {
    size_t expTimeStamp{0};
    for (const Event<bool> &event : booleanElements) {
        if (event.getTimeStep() != expTimeStamp)
            return false;
        expTimeStamp++;
    }
    return true;
}

void Trace::setTerminalEvent(bool last) {
    if (last) {
        booleanElements.front().setTerminal(false);
        booleanElements.back().setTerminal(true);
    } else {
        booleanElements.front().setTerminal(true);
        booleanElements.back().setTerminal(false);
    }
}

std::vector<Event<bool>> & Trace::getTraceRef() { return booleanElements; }

const std::vector<Event<std::array<size_t, 2>>> &Trace::getIntervalTrace() const { return intervalElements; }
