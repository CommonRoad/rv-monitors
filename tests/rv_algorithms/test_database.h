#pragma once

#include "monitors/formula/formula.h"
#include "monitors/parser/parser.h"
#include "monitors/trace/trace.h"
#include "monitors/utils/utils.h"

/**
 * Struct representing test formula with trace and expected solution.
 */
struct TestSample {
  public:
    /**
     * Constructor.
     *
     * @param testName Name of test sample.
     * @param formulaStr Formula as string.
     * @param expectedResult Expected result of trace evaluation given formula.
     * @param trueAtomics Satisfied atomic propositions per time step.
     * @param timeSteps Time steps of trace elements.
     * @param domains Collection of domains.
     */
    TestSample(const std::string &testName, const std::string &formulaStr, std::vector<Verdict> expectedResult,
               const std::vector<std::vector<std::string>> &trueAtomics, const std::vector<size_t> &timeSteps = {},
               const std::vector<std::shared_ptr<DomainStorage>> &domains = {},
               const std::vector<std::shared_ptr<IFunction<bool>>> &functions = {});

    std::string testName;                                //**< Trace which should be monitored. */
    std::shared_ptr<Formula> formula;                    //**< Pointer to formula object. */
    std::shared_ptr<Trace> trace;                        //**< Trace which should be monitored. */
    std::vector<Verdict> expectedResult;                 //**< Expected result per trace element. */
    std::vector<std::shared_ptr<DomainStorage>> domains; //**< Collection of domains. */
};

/**
 * Struct storing different test samples.
 */
struct TestDataBase {
    /**
     * Constructor.
     */
    explicit TestDataBase();

    /**
     * Extracts test samples which support specific formula/trace types.
     *
     * @param supportedFormulaTypes List of supported formula types.
     * @return List of test sampels.
     */
    std::vector<TestSample> getDatabaseEntries(const std::vector<MonitoringProperties> &supportedFormulaTypes);

    /**
     * Returns test sample with specified name.
     *
     * @param name Name of test sample.
     * @return Test sample.
     */
    TestSample getTestSample(const std::string &name);

    /**
     * Merges a list of verdicts to a single verdict.
     *
     * @param expectedResult List of verdicts.
     * @return Single verdict.
     */
    static Verdict mergeResult(const std::vector<Verdict> &expectedResult);

  private:
    std::vector<TestSample> testCases; //**< List of stored test samples. */

    /**
     * Creates expected result for test case with specific time steps for each trace element.
     *
     * @param baseVerdicts List tuples containing time step and booleans.
     * @return List of verdicts representing expected result.
     */
    static std::vector<Verdict> createExpectedResult(const std::vector<std::tuple<size_t, bool>> &baseVerdicts);

    /**
     * Creates expected result for test case with equidistant time steps.
     *
     * @param baseVerdicts List of booleans
     * @return List of verdicts representing expected result.
     */
    static std::vector<Verdict> createExpectedResult(const std::vector<bool> &baseVerdicts = {});
};
