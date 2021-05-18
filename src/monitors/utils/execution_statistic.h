#pragma once
#include <chrono>
#include <cfloat>
/**
 * Taken from https://github.com/AutoPas/AutoPas/blob/ecbf799e8c1f828eb51e3507b76275afe98a8cb2/src/autopas/utils/Timer.h
 */
class ExecutionStatistic {
  public:
    ExecutionStatistic();

    virtual ~ExecutionStatistic();

    /**
     * Start the timer.
     */
    void start();

    /**
     * Stops the timer and returns the time elapsed in nanoseconds since the last call to start.
     * It also adds the duration to the total time.
     * @param traceLength Length of execution trace.
     * @return elapsed time in nanoseconds
     */
    long stop(size_t traceLength);

    /**
     * Adds the given amount of nanoseconds to the total time.
     * @param nanoseconds
     */
    void addTime(long nanoseconds);

    /**
     * Get total accumulated time.
     * @return Total time in nano seconds.
     */
    [[nodiscard]] long getTotalTime() const { return totalTime; }

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
    [[nodiscard]] long getNumExecutions() const;

    /**
     * Get total accumulated trace length.
     * @return Total trace length.
     */
    [[nodiscard]] double getTotalTraceLength() const;

  private:
    /**
     * Time point of last call of start().
     */
    std::chrono::high_resolution_clock::time_point startTime;

    /**
     * Accumulated total time.
     */
    long totalTime{0};

    /**
     * Indicator if this timer currently is measuring.
     */
    bool currentlyRunning{false};

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
    long numExecutions{0};

    /**
     * Total trace lengths.
     */
    long totalTraceLength{0};
};
