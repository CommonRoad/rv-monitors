#pragma once
#include <cfloat>
#include <chrono>
/**
 * Taken from https://github.com/AutoPas/AutoPas/blob/ecbf799e8c1f828eb51e3507b76275afe98a8cb2/src/autopas/utils/Timer.h
 */
class ExecutionStatistic {
  public:
    ExecutionStatistic();

    virtual ~ExecutionStatistic();

    /**
     * Start the timer.
     * @return Start time as high resolution time point.
     */
    static std::chrono::high_resolution_clock::time_point start();

    /**
     * Stops the timer and returns the time elapsed in nanoseconds since the last call to start.
     * It also adds the duration to the total time.
     * @param Start time as high resolution time point.
     * @param traceLength Length of execution trace.
     * @return elapsed time in nanoseconds
     */
    long stop(std::chrono::high_resolution_clock::time_point startTime, size_t traceLength);

    /**
     * Get total accumulated time.
     * @return Total time in nano seconds.
     */
    [[nodiscard]] size_t getTotalTime() const { return totalTime; }

    /**
     * Total number of executions.
     * @return Total number of executions.
     */
    [[nodiscard]] size_t getNumExecutions() const;

    /**
     * Getter for computation times.
     *
     * @return Computation times.
     */
    [[nodiscard]] const std::vector<size_t> &getCompTimes() const;

    /**
     * Getter for trace lengths.
     *
     * @return List of trace lengths.
     */
    [[nodiscard]] const std::vector<size_t> &getTraceLengths() const;

  private:
    size_t totalTime{0};              //**< Accumulated total time. */
    std::vector<size_t> compTimes;    //**< List of computation times. */
    size_t numExecutions{0};          //**< Total number of executions. */
    std::vector<size_t> traceLengths; //**< List of trace lengths. */
};
