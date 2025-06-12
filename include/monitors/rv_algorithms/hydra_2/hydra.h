#pragma once

#include "hydra_monitors/hydra_monitor.h"
#include "monitors/formula/formula.h"
#include "monitors/rv_algorithms/monitor_interface.h"
#include <monitors/trace/trace.h>

/**
 * Tree structure for visualizing Hydra2 monitors.
 */
struct TreeVis {
  private:
    std::map<size_t, std::string> nodeTypes;    //**< Mapping of monitor node IDs to logical representation. */
    std::map<size_t, std::vector<size_t>> tree; //**< Mapping of monitor node IDs to its child monitor IDs. */
    std::vector<size_t> rootIDs;                //**< List of root nodes. */

  public:
    /**
     * Constructor.
     *
     * @param nodeTypes Mapping of monitor node IDs to logical representation.
     * @param tree Mapping of monitor node IDs to its child monitor IDs.
     * @param rootIDs List of root nodes.
     */
    TreeVis(const std::map<size_t, std::string> &nodeTypes, const std::map<size_t, std::vector<size_t>> &tree,
            const std::vector<size_t> &rootIDs);

    /**
     * Getter for node type mapping.
     *
     * @return Mapping of monitor node IDs to logical representation.
     */
    [[nodiscard]] const std::map<size_t, std::string> &getNodeTypes() const;

    /**
     * Getter for tree structure mapping.
     *
     * @return Mapping of monitor node IDs to its child monitor IDs.
     */
    [[nodiscard]] const std::map<size_t, std::vector<size_t>> &getTree() const;

    /**
     * Getter for root node IDs.
     *
     * @return List of root nodes.
     */
    [[nodiscard]] const std::vector<size_t> &getRootIDs() const;
};

/**
 * Class for monitoring a trace using the Hydra2 MTL monitor.
 */
class Hydra2 : public MonitorInterface {
  public:
    /**
     * Constructor
     * @param formulas List of formulas which should be monitored.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     * @param reordering Boolean indicating whether the formula should be reordered based on the predicate costs.
     * @param shortCircuiting Boolean indicating whether short-circuiting should be used.
     * @param formulaEvaluationMode formulaEvaluationMode Indicates how to evaluate the formula.
     * @param domains List of domains.
     * @param predicateCosts Cost assigned to predicates consisting of evaluation cost in interval [0, 100],
     * satisfaction probability in interval [0, 1], and priority >= 0.
     */
    explicit Hydra2(const std::vector<Formula> &formulas, bool connectedEval = true, bool reordering = true,
                    bool shortCircuiting = true,
                    FormulaEvaluationMode formulaEvaluationMode = FormulaEvaluationMode::globally,
                    const std::vector<std::shared_ptr<DomainStorage>> &domains = {},
                    const std::map<std::string, std::array<double, 3>> &predicateCosts = {});

    /**
     * Evaluation function for monitor algorithms.
     *
     * @param trace Trace which should be evaluated.
     * @param domains List of domains.
     * @param formulaIDs List of formula IDs which should be evaluated.
     * @return List of verdicts for each time step of the trace. If the length of the vector is 1,
     * the runtime verification algorithm can only provide a global verdict for the complete trace.
     */
    std::map<std::string, std::vector<Verdict>>
    evaluate(const std::shared_ptr<Trace> &trace, const std::vector<std::shared_ptr<DomainStorage>> &domains = {},
             const std::vector<std::string> &formulaIDs = {}) override;

    /**
     * Creates string representation for corresponding monitors.
     *
     * @return String representation of Hydra.
     */
    [[nodiscard]] std::string to_string() const override;

    /**
     * Getter for predicate cost.
     *
     * @return Registered predicate costs.
     */
    std::map<std::string, std::array<double, 3>> &getPredicateCost() const;

    /**
     * Function to extract combination of violating domain values.
     *
     * @return Map containing violating domain values per rule and time step.
     */
    std::map<std::string, std::map<size_t, std::vector<std::vector<DomainValue>>>>
    getViolatingDomainValues() const override;

    /**
     * Extracts tree structure for visualization.
     *
     * @return Tree structure object.
     */
    TreeVis to_tree() const;

  private:
    std::vector<std::shared_ptr<Hydra2Monitor>>
        monitors; //**< List of monitors corresponding to the formulas which should be evaluated. */
    mutable std::map<std::string, std::array<double, 3>> predicateCosts; //**< Predicate costs. */
    size_t traceLength{0};                                               //**< Current trace length. */

    /**
     * Creates monitors for specified formulas based on provided configuration information.
     *
     * @param formulas List of formulas which should be monitored.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     * @param reordering Boolean indicating whether the formula should be reordered based on the predicate costs.
     * @param shortCircuiting Boolean indicating whether short-circuiting should be used.
     */
    void createMonitor(const std::vector<Formula> &formulas, bool connectedEval, bool reordering, bool shortCircuiting);

    /**
     * Resets monitor variables to default values.
     */
    void resetMonitors() const;

    /**
     * Setter for trace.
     *
     * @param tra Trace object.
     * @param domains List of domains.
     */
    void setTraceMonitors(const std::shared_ptr<Trace> &tra,
                          const std::vector<std::shared_ptr<DomainStorage>> &domains);

    /**
     * Evaluate complete trace.
     *
     * @param result Data structure for storing evaluation result.
     * @param formulaIDs List of formula IDs which should be evaluated
     */
    void evalCompleteTrace(std::map<std::string, std::vector<Verdict>> &result,
                           const std::vector<std::string> &formulaIDs) const;

    /**
     * Evaluates trace until verdict can be provided.
     *
     * @param result Data structure for storing evaluation result.
     * @param formulaIDs List of formula IDs which should be evaluated
     */
    void evalEarlyTermination(std::map<std::string, std::vector<Verdict>> &result,
                              const std::vector<std::string> &formulaIDs) const;
};
