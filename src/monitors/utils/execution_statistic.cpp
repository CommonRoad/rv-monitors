#include "monitors/utils/execution_statistic.h"

using namespace std::chrono;

ExecutionStatistic::ExecutionStatistic() = default;

ExecutionStatistic::~ExecutionStatistic() = default;

std::chrono::high_resolution_clock::time_point ExecutionStatistic::start() { return high_resolution_clock::now(); }

long ExecutionStatistic::stop(std::chrono::high_resolution_clock::time_point startTime, size_t traceLength) {
    const auto time(high_resolution_clock::now());

    const auto diff = duration_cast<nanoseconds>(time - startTime).count();

    totalTime += (size_t)diff;
    numExecutions++;
    traceLengths.push_back(traceLength);
    compTimes.push_back((size_t)diff);

    return diff;
}

size_t ExecutionStatistic::getNumExecutions() const { return numExecutions; }

const std::vector<size_t> &ExecutionStatistic::getCompTimes() const { return compTimes; }

const std::vector<size_t> &ExecutionStatistic::getTraceLengths() const { return traceLengths; }
