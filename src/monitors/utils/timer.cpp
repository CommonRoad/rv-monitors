#include "timer.h"
#include <stdexcept>

using namespace std::chrono;

timer::timer() : startTime{} {}

timer::~timer() = default;

void timer::start() {
    if (currentlyRunning) {
        throw std::runtime_error("Timer::start() Timer is already running.");
    }
    currentlyRunning = true;
    startTime = high_resolution_clock::now();
}

long timer::stop() {
    const auto time(high_resolution_clock::now());

    if (not currentlyRunning) {
        throw std::runtime_error("Timer::stop() Timer did not even start.");
    }
    currentlyRunning = false;

    const auto diff = duration_cast<nanoseconds>(time - startTime).count();

    totalTime += diff;

    return diff;
}

void timer::addTime(long nanoseconds) { totalTime += nanoseconds; }