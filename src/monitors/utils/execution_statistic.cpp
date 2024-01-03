#include "monitors/utils/execution_statistic.h"
#include <stdexcept>

using namespace std::chrono;

ExecutionStatistic::ExecutionStatistic() = default;

ExecutionStatistic::~ExecutionStatistic() = default;

std::chrono::high_resolution_clock::time_point ExecutionStatistic::start() { return high_resolution_clock::now(); }

long ExecutionStatistic::stop(std::chrono::high_resolution_clock::time_point startTime, size_t traceLength) {
    const auto time(high_resolution_clock::now());

    const auto diff = duration_cast<nanoseconds>(time - startTime).count();

    totalTime += diff;
    numExecutions++;
    totalTraceLength += traceLength;
    if (diff / traceLength < minTime)
        minTime = diff / traceLength;
    if (diff / traceLength > maxTime)
        maxTime = diff / traceLength;

    return diff;
}

void ExecutionStatistic::addTime(size_t nanoseconds) { totalTime += nanoseconds; }

double ExecutionStatistic::getMaxTime() const { return maxTime; }

double ExecutionStatistic::getMinTime() const { return minTime; }

size_t ExecutionStatistic::getNumExecutions() const { return numExecutions; }

size_t ExecutionStatistic::getTotalTraceLength() const { return totalTraceLength; }
