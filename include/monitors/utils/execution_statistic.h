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
     * Adds the given amount of nanoseconds to the total time.
     * @param nanoseconds
     */
    void addTime(size_t nanoseconds);

    /**
     * Get total accumulated time.
     * @return Total time in nano seconds.
     */
    [[nodiscard]] size_t getTotalTime() const { return totalTime; }

    /**
     * Get maximum computation time averaged by trace length.
     * @return Max. time in nano seconds.
     */
    [[nodiscard]] double getMaxTime() const;

    /**
     * Get minimum computation time averaged by trace length.
     * @return Min. time in nano seconds.
     */
    [[nodiscard]] double getMinTime() const;

    /**
     * Total number of executions.
     * @return Total number of executions.
     */
    [[nodiscard]] size_t getNumExecutions() const;

    /**
     * Get total accumulated trace length.
     * @return Total trace length.
     */
    [[nodiscard]] size_t getTotalTraceLength() const;

  private:
    /**
     * Accumulated total time.
     */
    size_t totalTime{0};

    /**
     * Max. computation time (averaged by trace length).
     */
    double maxTime{DBL_MIN};

    /**
     * Min. computation time (averaged by trace length).
     */
    double minTime{DBL_MAX};

    /**
     * Total number of executions.
     */
    size_t numExecutions{0};

    /**
     * Total trace lengths.
     */
    size_t totalTraceLength{0};
};
