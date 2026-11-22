#pragma once
#include "monitors/collections/domain_value.h"
#include "monitors/utils/utils.h"

#include <optional>
#include <set>

/**
 * Representation of Hydra2 monitor head position consisting of time step and trace position.
 */
struct HeadPosHydra2 {
    size_t timeStep;   //**< Time step of monitor head position. */
    int traceIdx = -1; //**< Trace index of monitor head position. Initial invalid index. */

    /**
     * Default constructor.
     */
    HeadPosHydra2() = default;

    /**
     * Constructor of head position.
     *
     * @param timeStep Time step of monitor head position.
     * @param traceIdx Trace index of monitor head position.
     */
    HeadPosHydra2(size_t timeStep, int traceIdx) : timeStep(timeStep), traceIdx(traceIdx) {};

    /**
     * Sets new time step and increments index by one.
     *
     * @param new_ts New time step.
     */
    void update(size_t new_ts) {
        timeStep = new_ts;
        traceIdx++;
    }

    /**
     * Reset head to default values.
     */
    void reset() {
        timeStep = 0;
        traceIdx = -1;
    }
};

/**
 * How far back verdicts of a monitor can still be requested, relative to the step evaluated by the roots: a verdict at
 * step k with time stamp t_k can be requested at root step i with time stamp t_i if k + steps >= i or t_k + time >=
 * t_i.
 */
struct VerdictRetention {
    size_t steps{0}; //**< Number of steps. */
    size_t time{0};  //**< Time difference. */

    /**
     * Checks whether this retention covers another one.
     *
     * @param other Other retention.
     * @return Boolean indicating whether this retention is at least as long as the other one in steps and time.
     */
    [[nodiscard]] bool covers(const VerdictRetention &other) const {
        return steps >= other.steps and time >= other.time;
    }
};

/**
 * Hydra2 monitor interface.
 */
class Hydra2Monitor {
  protected:
    /**
     * Checks whether step number higher than stored step number.
     *
     * @param stepNumber Step number which should be set.
     * @param parentId ID of calling parent monitor.
     * @return Boolean indicating whether step number can be updated.
     */
    bool stepNumberValid(size_t stepNumber, size_t parentId);

    /**
     * Checks whether all non-constant domain values are valid at a time stamp.
     *
     * @param domainValues Domain values to check.
     * @param timeStamp Time stamp of interest.
     * @return Boolean indicating whether all domain values are valid.
     */
    static bool validAt(const std::vector<DomainValue> &domainValues, size_t timeStamp);

    /**
     * Returns the child monitors.
     *
     * @return Child monitors.
     */
    [[nodiscard]] virtual std::vector<std::shared_ptr<Hydra2Monitor>> getChildren() const { return {}; }

    /**
     * Computes how far back the verdicts of the children can be requested given the retention of this monitor.
     * Operators that look into the past, i.e., previous and since, extend the retention of their children.
     *
     * @param own Retention of this monitor.
     * @return Retention of the children.
     */
    [[nodiscard]] virtual VerdictRetention childRetention(const VerdictRetention &own) const { return own; }

    /**
     * Checks whether the monitor requests the verdict of a child at the same step several times, i.e., whether it is an
     * until or since operator, which scan a window of steps. The children of such a monitor store their verdicts.
     *
     * @return Boolean indicating whether verdicts of the children are requested repeatedly.
     */
    [[nodiscard]] virtual bool requestsChildVerdictsRepeatedly() const { return false; }

  private:
    /**
     * Extracts maximum trace idx of monitor given all parent monitors (required for connected evaluation).
     *
     * @return Trace index.
     */
    size_t getMaxTraceIdx();

  public:
    /**
     * Destructor.
     */
    virtual ~Hydra2Monitor() = default;

    /** Enables/disables violation collection (and thereby disables/enables short-circuiting) of top-level universal
     * monitors. No-op by default. */
    virtual void setStoreVariableResults(bool /*store*/) {}

    /**
     * Base constructor for hydra monitor.
     *
     * @param formula Formula to monitor.
     * @param monitorId ID of monitor.
     * @param evalCost Evaluation cost assigned to monitor.
     * @param satisfactionProbability Satisfaction probability of monitor.
     */
    Hydra2Monitor(Formula formula, size_t monitorId, double evalCost = 0, double satisfactionProbability = 1.0);

    /**
     * General step function of hydra monitor.
     *
     * @param parentId Id of parent monitor.
     * @param stepNumber Number of step which should be performed.
     * @return Boolean verdict.
     */
    Verdict step(size_t parentId, size_t stepNumber);

    /**
     * Monitor specific step function.
     *
     * @param parentId Id of parent monitor.
     * @param stepNumber Number of step which should be performed.
     * @return Boolean verdict.
     */
    virtual Verdict concreteStep(size_t parentId, size_t stepNumber) = 0;

    /**
     * Setter for trace.
     *
     * @param tra Trace object.
     * @param domains List of relevant domains.
     */
    virtual void setTrace(const std::shared_ptr<Trace> &tra,
                          const std::vector<std::shared_ptr<DomainStorage>> &domains = {}) = 0;

    /**
     * Resets monitor variables to default values.
     */
    virtual void reset();

    /**
     * Adds domain value to function/predicate or calls sub-monitors to do it.
     *
     * @param vals Domain values which should be added.
     */
    virtual void addDomainValues(const std::vector<DomainValue> &vals) = 0;

    /**
     * Registers parent monitor.
     *
     * @param parentId ID of parent monitor.
     */
    void registerParent(size_t parentId);

    /**
     * Determines which verdicts the monitor and its descendants store and for how long. A monitor only stores its
     * verdicts if they can be requested several times, i.e., if it has several parents or a parent is an until or
     * since operator. Called for the root monitors after their creation.
     *
     * @param retention How far back the verdicts of this monitor can be requested.
     */
    void configureVerdictStorage(const VerdictRetention &retention = {});

    /**
     * Removes the stored verdicts of the monitor and its descendants which can no longer be requested after the roots
     * evaluated the given step. A removed verdict that is requested anyway, e.g., when other formulas are evaluated on
     * the same trace later, is recomputed.
     *
     * @param stepNumber Step evaluated by the roots.
     * @param timeStamp Time stamp of the step.
     */
    void pruneVerdictHistory(size_t stepNumber, size_t timeStamp);

    /**
     * Returns the number of stored verdicts of the monitor (without descendants).
     *
     * @return Number of stored verdicts.
     */
    [[nodiscard]] size_t getVerdictHistorySize() const;

    /**
     * Returns the number of stored verdicts of the monitor and its descendants; shared monitors are counted once.
     *
     * @param visited Monitors that were already counted.
     * @return Number of stored verdicts.
     */
    [[nodiscard]] size_t getNumStoredVerdicts(std::set<const Hydra2Monitor *> &visited) const;

    /**
     * Returns the number of verdicts computed by the monitor and its descendants since the last reset, i.e., the number
     * of steps that were not answered from the stored verdicts; shared monitors are counted once.
     *
     * @param visited Monitors that were already counted.
     * @return Number of computed verdicts.
     */
    [[nodiscard]] size_t getNumEvaluations(std::set<const Hydra2Monitor *> &visited) const;

    /**
     * Getter for ID of formula which is monitored.
     * @return
     */
    [[nodiscard]] const std::string &getFormulaId() const;

    /**
     * Sets ID of formula which is monitored.
     *
     * @param formula_id Formula ID as string.
     */
    void setFormulaId(const std::string &formula_id);

    /**
     * Getter for evaluation cost.
     *
     * @return Evaluation cost of monitor in interval [0, 100].
     */
    [[nodiscard]] double getEvalCost() const;

    /**
     * Getter for satisfaction probability.
     *
     * @return Satisfaction probability of monitor in interval [0, 1].
     */
    [[nodiscard]] double getSatisfactionProbability() const;

    /**
     * Clones this monitor.
     *
     * @param domValNames List of domain value names.
     * @return New shared_ptr of this monitor.
     */
    virtual std::shared_ptr<Hydra2Monitor> clone(const std::vector<std::string> &domValNames);

    /**
     * Creates string representation of monitor.
     *
     * @return String representing monitor.
     */
    virtual std::string to_string() = 0;

    /**
     * Function to extract combination of violating domain values.
     *
     * @return Map containing violating domain values per rule and time step.
     */
    [[nodiscard]] virtual std::map<size_t, std::vector<std::vector<DomainValue>>> getViolatingDomainValues() const;

    /**
     * Sets step number of monitor.
     * Triggered by quantification monitor to set step number accordingly in case quantified monitor is created later.
     *
     * @param stepNumber Desired step number.
     * @param parentId ID of parent monitor.
     */
    virtual void setStepNumber(size_t stepNumber, size_t parentId);

    /**
     * Extracts tree structure for visualization.
     *
     * @param tree Mapping of monitor ID to its logical representation and child nodes.
     */
    virtual void to_tree(std::map<size_t, std::tuple<std::string, std::vector<size_t>>> &tree);

    /**
     * Getter for monitor ID.
     *
     * @return Monitor ID.
     */
    [[nodiscard]] size_t getMonitorID() const;

    /**
     * Resets cloned monitor.
     *
     * @param clonedMonitor New cloned monitor.
     * @param domValNames Domain values for which monitor is created.
     * @return Updated monitor.
     */
    std::shared_ptr<Hydra2Monitor> resetClonedMonitor(std::shared_ptr<Hydra2Monitor> clonedMonitor,
                                                      const std::vector<std::string> &domValNames);

  protected:
    std::weak_ptr<Trace> trace;                //**< Trace which should be monitored. */
    Formula formula;                           //**< Monitored formula. */
    std::string formulaId;                     //**< ID of formula */
    size_t monitorId;                          //**< ID of monitor */
    std::string subID;                         //**< Sub ID considering domain values */
    std::map<size_t, Verdict> verdictHistory;  //**< History of verdicts belonging to monitor */
    size_t numParents{0};                      //**< Number of registered parent monitors. */
    bool storeVerdicts{false};                 //**< Whether verdicts are stored (see configureVerdictStorage). */
    std::optional<VerdictRetention> retention; //**< How far back stored verdicts can be requested. */
    std::optional<size_t> lastPruneStep;       //**< Step of the last pruning (pruning is done once per step). */
    size_t numEvaluations{0};                  //**< Number of computed verdicts since the last reset. */
    std::map<size_t, size_t> traceIdx;         //**< Index of current trace element (even) */
    double evalCost{0.0};                      //**< Evaluation cost of monitor */
    double satisfactionProbability{1.0};       //**< Satisfaction probability of monitor */
    std::vector<std::tuple<std::vector<std::string>, std::shared_ptr<Hydra2Monitor>>>
        allCopies; /** List of created copies/clones of monitor. */
};
