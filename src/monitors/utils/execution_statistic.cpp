#include "execution_statistic.h"
#include <stdexcept>

using namespace std::chrono;

ExecutionStatistic::ExecutionStatistic() : startTime{} {}

ExecutionStatistic::~ExecutionStatistic() = default;

void ExecutionStatistic::start() {
    if (currentlyRunning) {
        throw std::runtime_error("Timer::start() ExecutionStatistic is already running.");
    }
    currentlyRunning = true;
    startTime = high_resolution_clock::now();
}

long ExecutionStatistic::stop(size_t traceLength) {
    const auto time(high_resolution_clock::now());

    if (not currentlyRunning) {
        throw std::runtime_error("Timer::stop() ExecutionStatistic did not even start.");
    }
    currentlyRunning = false;

    const auto diff = duration_cast<nanoseconds>(time - startTime).count();

    totalTime += diff;
    numExecutions++;
    totalTraceLength += traceLength;
    if(diff/traceLength < minTime)
        minTime = diff/traceLength;
    if(diff/traceLength > maxTime)
        maxTime = diff/traceLength;

    return diff;
}

void ExecutionStatistic::addTime(long nanoseconds) { totalTime += nanoseconds; }

double ExecutionStatistic::getMaxTime() const { return maxTime; }

double ExecutionStatistic::getMinTime() const { return minTime; }

long ExecutionStatistic::getNumExecutions() const { return numExecutions; }

double ExecutionStatistic::getTotalTraceLength() const { return totalTraceLength; }
