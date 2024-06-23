#pragma once
#include "deque"
#include "monitors/collections/domain_value.h"
#include "monitors/utils/utils.h"

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
    HeadPosHydra2(size_t timeStep, int traceIdx) : timeStep(timeStep), traceIdx(traceIdx){};

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
 * Hydra2 monitor interface.
 */
class Hydra2Monitor {
  private:
    /**
     * Extracts maximum trace idx of monitor given all parent monitors (required for connected evaluation).
     *
     * @return Trace index.
     */
    size_t getMaxTraceIdx();

    /**
     * Extracts minimum trace idx of monitor given all parent monitors (required for connected evaluation).
     *
     * @return Trace index.
     */
    size_t getMinTraceIdx();

    /**
     * Evaluates whether another parent monitor has the same trace index.
     *
     * @param idx Trace index which should be used for comparison.
     * @return Boolean indicating whether another parent monitor is at the same trace index.
     */
    bool traceIdxUnique(size_t idx);

    /**
     * Decrements trace indices for all parent monitors except for the provided ID. Applied when first element of
     * verdictHistory is updated.
     *
     * @param parentID Parent monitor ID where index should not be decremented.
     */
    void decrementTraceIdx(size_t parentID);

    /**
     * Executes monitor until desired step number is reached.
     *
     * @param parentId Id of parent monitor.
     * @param stepNumber Last step number which should be performed.
     * @return Verdict of desired step number.
     */
    Verdict simulateVerdict(size_t parentId, size_t stepNumber);

  public:
    /**
     * Destructor.
     */
    virtual ~Hydra2Monitor() = default;

    /**
     * Base constructor for hydra monitor.
     *
     * @param formula Formula to monitor.
     * @param monitorId ID of monitor.
     * @param connectedEval Boolean whether connected evaluation should be performed.
     * @param evalCost Evaluation cost assigned to monitor.
     * @param satisfactionProbability Satisfaction probability of monitor.
     */
    Hydra2Monitor(Formula formula, size_t monitorId, bool connectedEval = true, double evalCost = 0,
                  double satisfactionProbability = 1.0);

    /**
     * General step function of hydra monitor.
     *
     * @param parentId Id of parent monitor.
     * @param stepNumber Number of step which should be performed.
     * @param singleStep Boolean indicating whether only one step for provided step number should be executed.
     * @return Boolean verdict.
     */
    Verdict step(size_t parentId, size_t stepNumber, bool singleStep = false);

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
    virtual void setTrace(const Trace &tra, const std::vector<std::shared_ptr<DomainStorage>> &domains = {}) = 0;

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
     * @return New shared_ptr of this monitor.
     */
    virtual std::shared_ptr<Hydra2Monitor> clone() = 0;

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

  protected:
    Formula formula;                     //**< Monitored formula. */
    std::string formulaId;               //**< ID of formula */
    size_t monitorId;                    //**< ID of monitor */
    std::deque<Verdict> verdictHistory;  //**< History of verdicts belonging to monitor */
    std::map<size_t, size_t> traceIdx;   //**< Index of current trace element (even) */
    bool connectedEval;                  //**< Boolean indicating whether connected evaluation should be performed */
    double evalCost{0.0};                //**< Evaluation cost of monitor */
    double satisfactionProbability{1.0}; //**< Satisfaction probability of monitor */
    size_t verdictHistoryPopCounter{0}; //**< Counter storing how often pop operation was applied on verdict history. */
};
